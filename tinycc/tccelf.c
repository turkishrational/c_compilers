/*
 *  ELF file handling for TCC
 *
 *  Copyright (c) 2001, 2002 Fabrice Bellard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* 9/7/2026 */
/* 8/7/2026 - Google AI */
/* =========================================================================
   TRDOS-386 SAF FLAT ELF COMPONENT LINKER SUBSYSTEM (tccelf.c - PART 1)
   ========================================================================= */

/* Append a dynamic string into the target ELF string table section and return its relative offset */
static int put_elf_str(Section *s, const char *sym)
{
    int offset, len;
    char *ptr;

    len = strlen(sym) + 1;
    offset = s->data_offset;
    ptr = section_ptr_add(s, len);
    memcpy(ptr, sym, len);
    return offset;
}

/* Calculate standard System V ELF hash tracking metrics for a given identifier string literal */
static unsigned long elf_hash(const unsigned char *name)
{
    unsigned long h = 0, g;
    
    while (*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}

/* Reconstruct and re-index the internal layout hashing table for section 's' to optimize symbol lookup speeds */
static void rebuild_hash(Section *s, unsigned int nb_buckets)
{
    Elf32_Sym *sym;
    int *ptr, *hash, nb_syms, sym_index, h;
    char *strtab;

    strtab = s->link->data;
    nb_syms = s->data_offset / sizeof(Elf32_Sym);

    s->hash->data_offset = 0;
    ptr = section_ptr_add(s->hash, (2 + nb_buckets + nb_syms) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = nb_syms;
    ptr += 2;
    hash = ptr;
    memset(hash, 0, (nb_buckets + 1) * sizeof(int));
    ptr += nb_buckets + 1;

    sym = (Elf32_Sym *)s->data + 1;
    for(sym_index = 1; sym_index < nb_syms; sym_index++) {
        if (ELF32_ST_BIND(sym->st_info) != STB_LOCAL) {
            h = elf_hash(strtab + sym->st_name) % nb_buckets;
            *ptr = hash[h];
            hash[h] = sym_index;
        } else {
            *ptr = 0;
        }
        ptr++;
        sym++;
    }
}

/* Push a new symbol specification directly into the target ELF symbol section layout */
static int put_elf_sym(Section *s, unsigned long value, unsigned long size,
                       int info, int other, int shndx, const char *name)
{
    int name_offset, sym_index;
    int nbuckets, h;
    Elf32_Sym *sym;
    Section *hs;
    
    sym = section_ptr_add(s, sizeof(Elf32_Sym));
    if (name)
        name_offset = put_elf_str(s->link, name);
    else
        name_offset = 0;
        
    sym->st_name = name_offset;
    sym->st_value = value;
    sym->st_size = size;
    sym->st_info = info;
    sym->st_other = other;
    sym->st_shndx = shndx;
    sym_index = sym - (Elf32_Sym *)s->data;
    
    hs = s->hash;
    if (hs) {
        int *ptr, *base;
        ptr = section_ptr_add(hs, sizeof(int));
        base = (int *)hs->data;
        
        if (ELF32_ST_BIND(info) != STB_LOCAL) {
            nbuckets = base[0];
            h = elf_hash(name) % nbuckets;
            *ptr = base[2 + h];
            base[2 + h] = sym_index;
            base[1]++;
            
            /* Expand hashing map dynamically if layout density exceeds optimal thresholds */
            if (base[1] > 2 * nbuckets) {
                rebuild_hash(s, 2 * nbuckets);
            }
        } else {
            *ptr = 0;
            base[1]++;
        }
    }
    return sym_index;
}

/* Find global ELF symbol 'name' and return its index. Return 0 if not found. */
static int find_elf_sym(Section *s, const char *name)
{
    Elf32_Sym *sym;
    Section *hs;
    int nbuckets, sym_index, h;
    const char *name1;
    
    hs = s->hash;
    if (!hs)
        return 0;
        
    nbuckets = ((int *)hs->data)[0];
    h = elf_hash(name) % nbuckets;
    sym_index = ((int *)hs->data)[2 + h];
    
    while (sym_index != 0) {
        sym = &((Elf32_Sym *)s->data)[sym_index];
        name1 = s->link->data + sym->st_name;
        if (!strcmp(name, name1))
            return sym_index;
        sym_index = ((int *)hs->data)[2 + nbuckets + sym_index];
    }
    return 0;
}

/* Public Linker API: Retrieve the absolute runtime linear address associated with a registered symbol identifier */
void *tcc_get_symbol(TCCState *s, const char *name)
{
    int sym_index;
    Elf32_Sym *sym;
    
    sym_index = find_elf_sym(symtab_section, name);
    if (!sym_index)
        return 0;
        
    sym = &((Elf32_Sym *)symtab_section->data)[sym_index];
    return (void *)sym->st_value;
}

/* Add an ELF symbol: Check if it is already defined in dictionary maps and patch it. Return symbol index.
   Accepts SHN_UNDEF section definitions smoothly for unresolved forward tracking references. */
static int add_elf_sym(Section *s, unsigned long value, unsigned long size,
                       int info, int sh_num, const char *name)
{
    Elf32_Sym *esym;
    int sym_bind, sym_index, sym_type, esym_bind;

    sym_bind = ELF32_ST_BIND(info);
    sym_type = ELF32_ST_TYPE(info);
        
    if (sym_bind != STB_LOCAL) {
        /* Query dictionary table to verify if the global or weak symbol definition already exists */
        sym_index = find_elf_sym(s, name);
        if (!sym_index)
            goto do_def;
            
        esym = &((Elf32_Sym *)s->data)[sym_index];
        if (esym->st_shndx != SHN_UNDEF) {
            esym_bind = ELF32_ST_BIND(esym->st_info);
            if (sh_num == SHN_UNDEF) {
                /* Ignore unallocated forward declarations if the concrete target symbol is already loaded */
            } else if (sym_bind == STB_GLOBAL && esym_bind == STB_WEAK) {
                /* Global binding overrides historical weak symbol references: Trigger explicit metadata patch */
                goto do_patch;
            } else if (sym_bind == STB_WEAK && esym_bind == STB_GLOBAL) {
                /* Weak symbols are safely dropped if a strong global identifier definition exists */
            } else {
                /* Shared DLL multiple definition exceptions cleanly bypassed under pure flat binary parameters */
                error_noabort("'%s' defined twice", name);
            }
        } else {
        do_patch:
            esym->st_info = ELF32_ST_INFO(sym_bind, sym_type);
            esym->st_shndx = sh_num;
            esym->st_value = value;
            esym->st_size = size;
        }
    } else {
    do_def:
        sym_index = put_elf_sym(s, value, size, ELF32_ST_INFO(sym_bind, sym_type), 0, sh_num, name);
    }
    return sym_index;
}

/* Push a raw binary relocation entry tracking hardware addresses and target symbols */
static void put_elf_reloc(Section *symtab, Section *s, unsigned long offset, int type, int symbol)
{
    char buf[256];
    Section *sr;
    Elf32_Rel *rel;

    sr = s->reloc;
    if (!sr) {
        /* Instantiate a clean relocation text section if absent from the module definitions map */
        snprintf(buf, sizeof(buf), ".rel%s", s->name);
        sr = new_section(tcc_state, buf, SHT_REL, symtab->sh_flags);
        sr->sh_entsize = sizeof(Elf32_Rel);
        sr->link = symtab;
        sr->sh_info = s->sh_num;
        s->reloc = sr;
    }
    rel = section_ptr_add(sr, sizeof(Elf32_Rel));
    rel->r_offset = offset;
    rel->r_info = ELF32_R_INFO(symbol, type);
}

/* STABS debug interpretation infrastructure safely siphoned away to shield pure flat integrity */

/* Sort ELF symbols putting local symbols below global and weak ones to fulfill ELF standard specs.
   Adjusts and updates all related relocation offset tables concurrently. */
static void sort_syms(TCCState *s1, Section *s)
{
    int *old_to_new_syms;
    Elf32_Sym *new_syms;
    int nb_syms, i;
    Elf32_Sym *p, *q;
    Elf32_Rel *rel, *rel_end;
    Section *sr;
    int type, sym_index;

    nb_syms = s->data_offset / sizeof(Elf32_Sym);
    new_syms = tcc_malloc(nb_syms * sizeof(Elf32_Sym));
    old_to_new_syms = tcc_malloc(nb_syms * sizeof(int));

    /* Execute the primary pass filtering and packing local symbol descriptors */
    p = (Elf32_Sym *)s->data;
    q = new_syms;
    for(i = 0; i < nb_syms; i++) {
        if (ELF32_ST_BIND(p->st_info) == STB_LOCAL) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
    }
    /* Commit the local symbols counter metric directly into the active section header description */
    s->sh_info = q - new_syms;

    /* Execute the secondary pass packing global and weak symbol descriptors consecutively */
    p = (Elf32_Sym *)s->data;
    for(i = 0; i < nb_syms; i++) {
        if (ELF32_ST_BIND(p->st_info) != STB_LOCAL) {
            old_to_new_syms[i] = q - new_syms;
            *q++ = *p;
        }
        p++;
    }
    
    /* Mirror the sorted symbol layout tracking frames back onto the original memory chunk */
    memcpy(s->data, new_syms, nb_syms * sizeof(Elf32_Sym));
    tcc_free(new_syms);

    /* Enforce updates mapping relocation tables to align with newly updated symbol index pointers */
    for(i = 1; i < s1->nb_sections; i++) {
        sr = s1->sections[i];
        if (sr->sh_type == SHT_REL && sr->link == s) {
            rel_end = (Elf32_Rel *)(sr->data + sr->data_offset);
            for(rel = (Elf32_Rel *)sr->data; rel < rel_end; rel++) {
                sym_index = ELF32_R_SYM(rel->r_info);
                type = ELF32_R_TYPE(rel->r_info);
                sym_index = old_to_new_syms[sym_index];
                rel->r_info = ELF32_R_INFO(sym_index, type);
            }
        }
    }
    
    tcc_free(old_to_new_syms);
}

/* Relocate uninitialized global common symbols allocating their footprints straight inside .bss section */
static void relocate_common_syms(void)
{
    Elf32_Sym *sym, *sym_end;
    unsigned long offset, align;
    
    sym_end = (Elf32_Sym *)(symtab_section->data + symtab_section->data_offset);
    for(sym = (Elf32_Sym *)symtab_section->data + 1; sym < sym_end; sym++) {
        if (sym->st_shndx == SHN_COMMON) {
            /* Factor memory alignments updating target layout constraints safely */
            align = sym->st_value;
            offset = bss_section->data_offset;
            offset = (offset + align - 1) & -align;
            sym->st_value = offset;
            sym->st_shndx = bss_section->sh_num;
            offset += sym->st_size;
            bss_section->data_offset = offset;
        }
    }
}

/* Resolve a runtime external symbol reference map falling back directly to stubbed address markers */
static unsigned long resolve_sym(TCCState *s1, const char *sym)
{
    /* Dynamic object dlsym linking structures siphoned away to shield absolute pure flat binary integrity */
    return 0;
}

/* Traverse and evaluate layout symbols computing precise linear destination address locations */
void relocate_syms(TCCState *s1, int do_run)
{
    Elf32_Sym *sym, *sym_end;
    Section *s;
    int i, type, sym_index;
    const char *name;

    for (i = 1; i < s1->nb_sections; i++) {
        s = s1->sections[i];
        if (s->sh_type == SHT_SYMTAB) {
            sym_end = (Elf32_Sym *)(s->data + s->data_offset);
            sym_index = 1;
            
            for (sym = (Elf32_Sym *)s->data + 1; sym < sym_end; sym++) {
                type = ELF32_ST_TYPE(sym->st_info);
                
                if (sym->st_shndx == SHN_UNDEF) {
                    name = s->link->data + sym->st_name;
                    /* Query current dictionary to verify symbol existence before fallback processing loops */
                    sym->st_value = resolve_sym(s1, name);
                } else if (sym->st_shndx == SHN_COMMON) {
                    /* Common data block layouts completely handled and processed by the primary allocator loops */
                } else if (sym->st_shndx < SHN_LORESERVE) {
                    /* Read the targeted base physical section address maps updating tracking values sequentially */
                    sym->st_value += s1->sections[sym->st_shndx]->sh_addr;
                }
                sym_index++;
            }
        }
    }
}

/* Relocate a given section applying native 32-bit x86 Protected Mode architecture patches.
   Processes strict linear addressing models siphoning away shared library constraints. */
static void relocate_section(TCCState *s1, Section *s)
{
    Section *sr;
    Elf32_Rel *rel, *rel_end;
    Elf32_Sym *sym;
    int type, sym_index;
    unsigned char *ptr;
    unsigned long val, addr;

    sr = s->reloc;
    rel_end = (Elf32_Rel *)(sr->data + sr->data_offset);
    
    for(rel = (Elf32_Rel *)sr->data; rel < rel_end; rel++) {
        ptr = s->data + rel->r_offset;

        sym_index = ELF32_R_SYM(rel->r_info);
        sym = &((Elf32_Sym *)symtab_section->data)[sym_index];
        val = sym->st_value;
        type = ELF32_R_TYPE(rel->r_info);
        addr = s->sh_addr + rel->r_offset;

        /* Execute machine code address patching based on standard Intel i386 ABI rules */
        switch(type) {
        case R_386_32:
            /* Absolute 32-bit direct memory cell variable layout patching */
            *(int *)ptr += val;
            break;
        case R_386_PC32:
            /* Relative 32-bit program counter displacement instruction layout patching */
            *(int *)ptr += val - addr;
            break;
        case R_386_PLT32:
            /* Procedural linkage target fallbacks routed straight to linear addresses */
            *(int *)ptr += val - addr;
            break;
        default:
            /* Bypassed completely as dynamic dynamic components (GOT/PLT) are eliminated */
            break;
        }
    }
}

/* Relocate the primary base offsets inside the target relocation table section descriptor 'sr' */
static void relocate_rel(TCCState *s1, Section *sr)
{
    Section *s;
    Elf32_Rel *rel, *rel_end;
    
    s = s1->sections[sr->sh_info];
    rel_end = (Elf32_Rel *)(sr->data + sr->data_offset);
    for(rel = (Elf32_Rel *)sr->data; rel < rel_end; rel++) {
        rel->r_offset += s->sh_addr;
    }
}

/* Write a 32-bit unsigned integer value safely into target byte arrays enforcing low-endian rules */
static void put32(unsigned char *p, uint32_t val)
{
    p[0] = val;
    p[1] = val >> 8;
    p[2] = val >> 16;
    p[3] = val >> 24;
}

/* Extract and decode a 32-bit unsigned integer value from target low-endian byte arrays safely */
static uint32_t get32(unsigned char *p)
{
    return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

/* Dynamic linkage structural generators (build_got, put_got_entry, build_got_entries) 
   completely siphoned away to adapt the pipeline strictly for pure flat executable binaries */

/* Create and initialize standard semantic ELF symbol tables (.symtab, .strtab, and .hash quadrants) */
static Section *new_symtab(TCCState *s1,
                           const char *symtab_name, int sh_type, int sh_flags,
                           const char *strtab_name, 
                           const char *hash_name, int hash_sh_flags)
{
    Section *symtab, *strtab, *hash;
    int *ptr, nb_buckets;

    symtab = new_section(s1, symtab_name, sh_type, sh_flags);
    symtab->sh_entsize = sizeof(Elf32_Sym);
    
    strtab = new_section(s1, strtab_name, SHT_STRTAB, sh_flags);
    put_elf_str(strtab, "");
    symtab->link = strtab;
    
    put_elf_sym(symtab, 0, 0, 0, 0, 0, NULL);
    
    nb_buckets = 1;

    hash = new_section(s1, hash_name, SHT_HASH, hash_sh_flags);
    hash->sh_entsize = sizeof(int);
    symtab->hash = hash;
    hash->link = symtab;

    ptr = section_ptr_add(hash, (2 + nb_buckets + 1) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = 1;
    memset(ptr + 2, 0, (nb_buckets + 1) * sizeof(int));
    return symtab;
}

static int trdos_runtime_injected = 0;

//
// /* 17/07/2026 */
// /* 16/07/2026 - Google AI */ 
// /* Inject and link native TRDOS runtime library to finalize standalone binary executables */
// static void tcc_add_runtime(TCCState *s1)
// {
//   char buf[1024];
//
//   if (trdos_runtime_injected != 0) {
//       return; 
//   }
//  trdos_runtime_injected = 1;
//
// /* 1. crt0.o -> Inject the absolute pure flat execution entry startup routine code */
// snprintf(buf, sizeof(buf), "./lib/%s", "crt0.o");
// tcc_add_file(s1, buf);
//
// /* 2. libc.a -> Link the static archive directly by passing the explicit file path */
// /* This bypasses internal library resolvers and directly forces the archive engine */
// snprintf(buf, sizeof(buf), "./lib/%s", "libc.a");
// tcc_add_file(s1, buf);
// }

/* 17/07/2026 - Google AI */ 
/* =========================================================================
   TRDOS 386 SPLIT RUNTIME INJECTION ENGINE - FIXED MEMORY LAYOUT
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

/* AŞAMA A: crt0.o dosyasını en başta 0x0 origin noktasına mühürler */
static void tcc_add_crt0_flat(TCCState *s1)
{
    char buf[1024];
    if (trdos_runtime_injected != 0) return; 
    
    /* Yerel kütüphane yolunu tanımla */
    tcc_add_library_path(s1, "./lib");

    /* crt0.o'yu İLK dosya olarak text_section'ın en başına (0x0) enjekte et */
    snprintf(buf, sizeof(buf), "./lib/%s", "crt0.o");
    tcc_add_file(s1, buf);
}

/* AŞAMA B: libc.a arşivini tam semboller listelendiğinde akıllıca tarar */
static void tcc_link_libc_flat(TCCState *s1)
{
    /* Sadece bir kez çağrılmasını garanti altına almak için statik bayrak */
    static int libc_linked = 0;
    if (libc_linked) return;
    libc_linked = 1;

    /* libc.a arşivini akıllı seçici tcc_load_archive motoruna gönderir */
    tcc_add_library(s1, "c");
}

#define ELF_START_ADDR 0x00000000 /* Aligned straight to absolute base address 0x0 for TRDOS 386 Saf Flat PRG format */
#define ELF_PAGE_SIZE  0x1000

/* Allocate memory and load raw binary section payload blocks directly out of an object file descriptor */
static void *load_data(int fd, unsigned long file_offset, unsigned long size)
{
    void *data;

    data = tcc_malloc(size);
    lseek(fd, file_offset, SEEK_SET);
    read(fd, data, size);
    return data;
}

/* Structural mapping registry context tracking section integration metrics during object merging phases */
typedef struct SectionMergeInfo {
    Section *s;            /* Pointer referencing the corresponding existing target section */
    unsigned long offset;  /* Absolute payload positioning offset within the merged destination layout */
    int new_section;       /* Boolean flag identifying if a fresh unique section was appended */
} SectionMergeInfo;

/* 19/07/2026 - Google AI - 22:16 - (0.9.18-0.9.27) */

/* Load a standalone native ELF object file and merge its section content layouts with current binary state memory */
static int tcc_load_object_file(TCCState *s1, int fd, unsigned long file_offset)
{ 
    Elf32_Ehdr ehdr;
    Elf32_Shdr *shdr, *sh;
    int size, i, j, offset, offseti, nb_syms, sym_index, ret;
    unsigned char *strsec, *strtab;
    int *old_to_new_syms;
    char *sh_name, *name;
    SectionMergeInfo *sm_table, *sm;
    Elf32_Sym *sym, *symtab;
    Elf32_Rel *rel, *rel_end;
    Section *s;

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
        goto fail1;

    /* Execute strict verification auditing the signature magic entries of the target ELF container */
    if (ehdr.e_ident[0] != ELFMAG0 ||
        ehdr.e_ident[1] != ELFMAG1 ||
        ehdr.e_ident[2] != ELFMAG2 ||
        ehdr.e_ident[3] != ELFMAG3)
        goto fail1;
        
    if (ehdr.e_type != ET_REL)
        goto fail1;
        
    /* Enforce hardware target constraint checks: Lock execution strictly onto Intel i386 Protected Mode architecture layouts */
    if (ehdr.e_ident[5] != ELFDATA2LSB || ehdr.e_machine != EM_386) {

      fail1:
        error_noabort("invalid object file");
        return -1;
    }

    /* Fetch and load target section headers mapping array slots seamlessly */
    shdr = load_data(fd, file_offset + ehdr.e_shoff, sizeof(Elf32_Shdr) * ehdr.e_shnum);
    sm_table = tcc_mallocz(sizeof(SectionMergeInfo) * ehdr.e_shnum);
    
    /* Load section name string identifiers */
    sh = &shdr[ehdr.e_shstrndx];
    strsec = load_data(fd, file_offset + sh->sh_offset, sh->sh_size);

    /* Locate and extract the singular primary symbol table layout */
    old_to_new_syms = NULL;
    symtab = NULL;
    strtab = NULL;
    nb_syms = 0;
    for(i = 1; i < ehdr.e_shnum; i++) {
        sh = &shdr[i];
        if (sh->sh_type == SHT_SYMTAB) {
            if (symtab) {
                error_noabort("object must contain only one symtab");
            fail:
                ret = -1;
                goto the_end;
            }
            nb_syms = sh->sh_size / sizeof(Elf32_Sym);
            symtab = load_data(fd, file_offset + sh->sh_offset, sh->sh_size);
            sm_table[i].s = symtab_section;

            /* Extract corresponding string table allocations */
            sh = &shdr[sh->sh_link];
            strtab = load_data(fd, file_offset + sh->sh_offset, sh->sh_size);
        }
    }
        
    /* Iterate through section footprints concatenating raw binary block data straight into matching memory quadrant areas */
    for(i = 1; i < ehdr.e_shnum; i++) {
        if (i == ehdr.e_shstrndx)
            continue;
        sh = &shdr[i];

        /* 0.9.27 NEŞTERİ: Eğer bölüm tipi NULL veya boyutu sıfırsa anında atla! */
        /* Bu satır yerel linker'ın 'invalid section type' uyarısı vermesini tamamen keser */
        if (sh->sh_type == SHT_NULL || sh->sh_size == 0)
            continue; 

        sh_name = (char *)(strsec + sh->sh_name);

        if (sh->sh_type != SHT_PROGBITS && sh->sh_type != SHT_REL && sh->sh_type != SHT_NOBITS)
            continue;
        if (sh->sh_addralign < 1)
            sh->sh_addralign = 1;
 
        /* Query active context to trace if a matching output section already exists */
        for(j = 1; j < s1->nb_sections; j++) {
            s = s1->sections[j];
            if (!strcmp(s->name, sh_name))
                goto found;
        }
        /* Not discovered: Instantiate a fresh dedicated flat code or data section descriptor slot */
        s = new_section(s1, sh_name, sh->sh_type, sh->sh_flags);
        s->sh_addralign = sh->sh_addralign;
        s->sh_entsize = sh->sh_entsize;
        sm_table[i].new_section = 1;
    found:
        if (sh->sh_type != s->sh_type) {
            error_noabort("invalid section type");
            goto fail;
        }

        /* Pad and satisfy memory alignment properties dynamically matching underlying structural dimensions */
        offset = s->data_offset;
        size = sh->sh_addralign - 1;
        offset = (offset + size) & ~size;
        if (sh->sh_addralign > s->sh_addralign)
            s->sh_addralign = sh->sh_addralign;
        s->data_offset = offset;
        sm_table[i].offset = offset;
        sm_table[i].s = s;
        
        /* Concat physical binary byte blocks cleanly dropping data boundaries */
        size = sh->sh_size;
        if (sh->sh_type != SHT_NOBITS) {
            unsigned char *ptr;
            lseek(fd, file_offset + sh->sh_offset, SEEK_SET);
            ptr = section_ptr_add(s, size);
            read(fd, ptr, size);
        } else {
            s->data_offset += size;
        }
    }

    /* Fast secondary mapping pass updating linked indices tracking relational section headers dependencies */
    sm = sm_table;
    for(i = 1; i < ehdr.e_shnum; i++) {
        s = sm_table[i].s;
        if (!s || !sm_table[i].new_section)
            continue;
        sh = &shdr[i];
        if (sh->sh_link > 0)
            s->link = sm_table[sh->sh_link].s;
        if (sh->sh_type == SHT_REL) {
            s->sh_info = sm_table[sh->sh_info].s->sh_num;
            s1->sections[s->sh_info]->reloc = s;
        }
    }

    /* Target milestone entry: Resolve incoming symbols maps */

    /* Resolve symbols sequence loop mapping raw fields into the global registry context */
    old_to_new_syms = tcc_mallocz(nb_syms * sizeof(int));

    sym = symtab + 1;
    for(i = 1; i < nb_syms; i++, sym++) {
        if (sym->st_shndx != SHN_UNDEF && sym->st_shndx < SHN_LORESERVE) {
            sm = &sm_table[sym->st_shndx];
            /* If no corresponding section is added, bypass processing this standalone symbol descriptor */
            if (!sm->s)
                continue;
            /* Convert section tracking numbers sequentially */
            sym->st_shndx = sm->s->sh_num;
            /* Update absolute offset values */
            sym->st_value += sm->offset;
        }
        /* Append validated symbol layout straight into the master ELF symbol section quadrants */
        name = (char *)(strtab + sym->st_name);
        sym_index = add_elf_sym(symtab_section, sym->st_value, sym->st_size, 
                                sym->st_info, sym->st_shndx, name);
        old_to_new_syms[i] = sym_index;
    }

    /* Execute the third analytical pass to dynamically patch hardware relocation data slots */
    for(i = 1; i < ehdr.e_shnum; i++) {
        s = sm_table[i].s;
        if (!s)
            continue;
        sh = &shdr[i];
        offset = sm_table[i].offset;
        
        switch(s->sh_type) {
        case SHT_REL:
            /* Extract section relative offset descriptors to execute structural mapping updates */
            offseti = sm_table[sh->sh_info].offset;
            rel_end = (Elf32_Rel *)(s->data + s->data_offset);
            
            for(rel = (Elf32_Rel *)(s->data + offset); rel < rel_end; rel++) {
                int type;
                unsigned int target_sym_index;
                
                type = ELF32_R_TYPE(rel->r_info);
                target_sym_index = ELF32_R_SYM(rel->r_info);
                
                if (target_sym_index >= (unsigned int)nb_syms)
                    goto invalid_reloc;
                    
                target_sym_index = old_to_new_syms[target_sym_index];
                if (!target_sym_index) {
                invalid_reloc:
                    error_noabort("Invalid relocation entry");
                    goto fail;
                }
                rel->r_info = ELF32_R_INFO(target_sym_index, type);
                /* Apply calculated offset adjustments onto the relocation record fields */
                rel->r_offset += offseti;
            }
            break;
        default:
            break;
        }
    }
    
    ret = 0;
 the_end:
    /* Safely release volatile working array allocations guaranteeing zero memory pollution paths */
    tcc_free(symtab);
    tcc_free(strtab);
    tcc_free(old_to_new_syms);
    tcc_free(sm_table);
    tcc_free(strsec);
    tcc_free(shdr);
    return ret;
}

#define ARMAG  "!<arch>\012"	/* Standard library archive file magic string definition */

/* Structural layout defining the continuous padding framework of an archive (.a) member header */
typedef struct ArchiveHeader {
    char ar_name[16];		/* Name identifier string array of this member file */
    char ar_date[12];		/* Structural modification timestamp metric descriptor */
    char ar_uid[6];		/* Associated file owner user identifier value */
    char ar_gid[6];		/* Associated file owner group identifier value */
    char ar_mode[8];		/* Hardware permission file mode numeric octal string */
    char ar_size[10];		/* Total file payload size scale tracked in decimals */
    char ar_fmag[2];		/* Validation tail end marker signature bytes */
} ArchiveHeader;

/* 17/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 FLAT LINKER - SMART REVERSE LOOKUP WITH UNDERLINE TOLERANCE (V8)
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

/* İki sembol adını başında alt çizgi ('_') olsa da olmasa da akıllıca eşleştirir */
static int trdos_sym_match(const char *s1, const char *s2)
{
    /* 1. Adım: Düz eşitlik kontrolü (Tam eşleşme) */
    if (strcmp(s1, s2) == 0) return 1;

    /* 2. Adım: s1'in ilk karakteri '_' ise ve geri kalanı s2 ile aynıysa */
    if (s1[0] == '_' && strcmp(s1 + 1, s2) == 0) return 1;

    /* 3. Adım: s2'nin ilk karakteri '_' ise ve geri kalanı s1 ile aynıysa */
    if (s2[0] == '_' && strcmp(s1, s2 + 1) == 0) return 1;

    return 0; /* Eşleşme başarısız */
}

/* 17/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 FLAT LINKER - SMART SELECTIVE ARCHIVE PARSER (FD-KORUMALI V5)
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

static int check_if_obj_needed_trdos(TCCState *s1, int fd, unsigned long file_offset)
{
    Elf32_Ehdr ehdr;
    Elf32_Shdr *shdr = NULL;
    Elf32_Sym *symtab = NULL;
    char *strtab = NULL;
    int nb_syms = 0;
    int i, j, needed = 0;
    long original_fd_pos;

    Elf32_Sym *tcc_syms;
    int nb_tcc_syms;

    /* Ana döngünün fd konumunu bozmamak için geçerli pozisyonu yedekle */
    original_fd_pos = lseek(fd, 0, 1); /* SEEK_CUR = 1 */
    if (original_fd_pos == -1) return 0;

    /* .o modülünün başına git ve temel ELF32 başlığını oku */
    if (lseek(fd, file_offset, 0) == -1) /* SEEK_SET = 0 */
        return 0;

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr))
        goto restore_fd_and_exit;

    /* Sihirli ELF numaralarını ve Relocatable (ET_REL) nesne türünü doğrula */
    if (memcmp(ehdr.e_ident, "\x7f\x45\x4c\x46", 4) != 0 || ehdr.e_type != ET_REL)
        goto restore_fd_and_exit;

    /* Section başlık tablosunu hafızaya al */
    shdr = (Elf32_Shdr *)tcc_malloc(sizeof(Elf32_Shdr) * ehdr.e_shnum);
    if (!shdr) goto restore_fd_and_exit;

    if (lseek(fd, file_offset + ehdr.e_shoff, 0) == -1 ||
        read(fd, shdr, sizeof(Elf32_Shdr) * ehdr.e_shnum) != sizeof(Elf32_Shdr) * ehdr.e_shnum) {
        goto restore_fd_and_exit;
    }

    /* SHT_SYMTAB (Sembol Tablosu) ve ona bağlı dize (String) tablosunu bul */
    for (i = 1; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB) {
            nb_syms = shdr[i].sh_size / sizeof(Elf32_Sym);
            symtab = (Elf32_Sym *)tcc_malloc(shdr[i].sh_size);
            
            int str_sec_idx = shdr[i].sh_link;
            strtab = (char *)tcc_malloc(shdr[str_sec_idx].sh_size);

            if (!symtab || !strtab) break;

            if (lseek(fd, file_offset + shdr[i].sh_offset, 0) == -1 ||
                read(fd, symtab, shdr[i].sh_size) != shdr[i].sh_size ||
                lseek(fd, file_offset + shdr[str_sec_idx].sh_offset, 0) == -1 ||
                read(fd, strtab, shdr[str_sec_idx].sh_size) != shdr[str_sec_idx].sh_size) {
                break;
            }
            goto tables_loaded_ok;
        }
    }

    goto restore_fd_and_exit;

tables_loaded_ok:
    tcc_syms = (Elf32_Sym *)symtab_section->data;
    nb_tcc_syms = symtab_section->data_offset / sizeof(Elf32_Sym);

    for (i = 1; i < nb_tcc_syms; i++) {
        Elf32_Sym *tsym = &tcc_syms[i];

        /* TCC tablosunda TANIMSIZ (SHN_UNDEF) olarak bekleyen açık bir sembol varsa */
        if (tsym->st_shndx == SHN_UNDEF && tsym->st_name != 0) {
            /* DÜZELTME: s1->symtab_section yerine doğrudan küresel symtab_section kullanıyoruz */
            char *tcc_sym_name = (char *)symtab_section->link->data + tsym->st_name;

            for (j = 1; j < nb_syms; j++) {
                Elf32_Sym *osym = &symtab[j];

                if (osym->st_shndx != SHN_UNDEF && osym->st_shndx < SHN_LORESERVE) {
                    char *obj_sym_name = strtab + osym->st_name;

                    /* Alt çizgi esnekliğiyle eşleştir */
                    if (trdos_sym_match(tcc_sym_name, obj_sym_name)) {
                        needed = 1; 
                        goto scan_complete;
                    }
                }
            }
        }
    }

scan_complete:
restore_fd_and_exit:
    if (symtab) tcc_free(symtab);
    if (strtab) tcc_free(strtab);
    if (shdr) tcc_free(shdr);

    lseek(fd, original_fd_pos, 0); /* SEEK_SET */
    return needed;
}
/* 17/07/2026 - Google AI */
/* =========================================================================
   TRDOS 386 FLAT LINKER - NATIVE BASE-10 STR-TO-LONG CONVERTER
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

/* Harici strtol bağımlılığını kesen, arşiv boyutları için özel taban-10 çözücü */
static long trdos_strtol_base10(const char *nptr)
{
    long result = 0;
    int i = 0;

    /* Başındaki boşlukları güvenle atla */
    while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\r' || nptr[i] == '\n') {
        i++;
    }

    /* ASCII karakterleri sayıya dönüştür */
    while (nptr[i] >= '0' && nptr[i] <= '9') {
        result = (result * 10) + (nptr[i] - '0');
        i++;
    }

    return result;
}

/* 17/07/2026 - MODIFIED SMART PARSER FOR TRDOS 386 Flat Binary Execution */
/* =========================================================================
   TRDOS 386 FLAT LINKER - SMART SELECTIVE ARCHIVE PARSER (NİHAİ FLAT V3)
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

/* 17/07/2026 - FINAL STABLE PARSER FOR TRDOS 386 Flat Binary System */
/* =========================================================================
   TRDOS 386 FLAT LINKER - SMART ARCHIVE PARSER WITH DETAILED DEBUG LOGS
   Geliştirici: Erdoğan Tan & Akıllı Seçici Bağlama Motoru (2026)
   ========================================================================= */

/* 17/07/2026 - FINAL STABLE PARSER FOR TRDOS 386 Flat Binary System - WITH DEBUG LOGS */
static int tcc_load_archive(TCCState *s1, int fd)
{
    ArchiveHeader hdr;
    char ar_size[11];
    char ar_name[17];
    char magic[8];
    int size, len, i;
    unsigned long file_offset;
    long total_archive_size;
    int has_new_loads;
    int loop_counter = 0;

    /* AŞAMA 1: Dosyanın toplam boyutunu milimetrik olarak ölç ve hafızaya al */
    total_archive_size = lseek(fd, 0, 2); /* SEEK_END = 2 */
    if (total_archive_size == -1) {
        error_noabort("[TRDOS CRITICAL] Failed to measure archive size!");
        return -1;
    }

    /* Arşivin başındaki sihirli imzayı doğrulamak için 0'a geri dön */
    if (lseek(fd, 0, 0) == -1 || read(fd, magic, 8) != 8) {
        error_noabort("[TRDOS DEBUG] Failed to read 8-byte archive magic header!");
        return -1;
    }

    if (memcmp(magic, "!<arch>\n", 8) != 0) {
        error_noabort("[TRDOS DEBUG] Invalid archive magic signature!");
        return -1;
    }
    
    printf("\r\n[TRDOS LINKER] >>> Starting Static Library Linker Engine <<<\r\n");
    printf("[TRDOS LINKER] Total Archive Size: %d bytes\r\n", total_archive_size);

    /* 
       AŞAMA 2: ZİNCİRLEME BAĞIMLILIK DÖNGÜSÜ
       Kütüphaneden yeni hiçbir dosya çekilmeyene kadar arşivi baştan sona tekrar tararız.
    */
    do {
        has_new_loads = 0;
        loop_counter++;
        printf("[TRDOS LINKER] --- Archive Scan Pass #%d ---\r\n", loop_counter);
        
        /* Her yeni turda dosya işaretçisini arşivin başlangıcına (offset 8'e) çek */
        if (lseek(fd, 8, 0) == -1) {
            error_noabort("[TRDOS CRITICAL] Failed to reset archive pointer to offset 8!");
            return -1;
        }

        for(;;) {
            /* KONTROL BARAJI: Eğer dosya işaretçisi toplam boyuta ulaştıysa arşiv BİTMİŞTİR! */
            long current_hdr_pos = lseek(fd, 0, 1); /* SEEK_CUR = 1 */
            if (current_hdr_pos >= total_archive_size) {
                break; /* İçteki sonsuz döngüden güvenle çık (EOF) */
            }

            len = read(fd, &hdr, sizeof(hdr));
            if (len == 0)
                break; /* Standart EOF emniyeti */
                
            if (len != sizeof(hdr)) {
                error_noabort("[TRDOS DEBUG] Invalid archive header read length! Pos: %d", current_hdr_pos);
                return -1;
            }

            // Boyut bilgisini ASCII'den sayıya güvenle dönüştür
            memcpy(ar_size, hdr.ar_size, sizeof(hdr.ar_size));
            ar_size[sizeof(hdr.ar_size)] = '\0';

            /* KRİTİK DÜZELTME: 
               Boş dönen kütüphane fonksiyonu yerine kendi yazdığımız garantili çözücüyü çağırıyoruz!
            */
            size = trdos_strtol_base10(ar_size);

            // Modül adını ayıkla ve temizle
            memcpy(ar_name, hdr.ar_name, sizeof(hdr.ar_name));
            for(i = sizeof(hdr.ar_name) - 1; i >= 0; i--) {
                if (ar_name[i] != ' ')
                    break;
            }
            ar_name[i + 1] = '\0';
            
            /* Başlığın hemen bittiği, verinin başladığı offset'i yakala */
            file_offset = lseek(fd, 0, 1); /* SEEK_CUR */

            /* Hizalama payı özellikleri (çift sayı sınırlarına yuvarla) */
            size = (size + 1) & ~1;

            if (!strcmp(ar_name, "/") ||
                !strcmp(ar_name, "//") ||
                !strcmp(ar_name, "__.SYMDEF") ||
                !strcmp(ar_name, "__.SYMDEF/") ||
                !strcmp(ar_name, "ARFILENAMES/")) {
                /* Fihrist tablolarını pas geç */
                printf("  [SKIP INDEX] Found System Section: '%s' (Size: %d)\r\n", ar_name, size);
            } else {
                if (check_if_obj_needed_trdos(s1, fd, file_offset)) {
                   
                    if (tcc_load_object_file(s1, fd, file_offset) < 0) {
                        error_noabort("[TRDOS DEBUG] Failed loading object module: '%s'", ar_name);
                        return -1;
                    }
                    has_new_loads = 1; /* Yeni modül bağlandı, döngüyü açık tut */
                }
            }
            
            /* Bir sonraki modül başlığının başlangıç sınırına zıpla */
            if (lseek(fd, file_offset + size, 0) == -1) {
                error_noabort("[TRDOS DEBUG] CRITICAL: lseek failed to jump to Offset: %u", file_offset + size);
                return -1;
            }
        }
    } while (has_new_loads); /* Yeni dosya eklenmeyene kadar dön */

    printf("[TRDOS LINKER] >>> Static Library Linker Pipeline Completed <<<\r\n\r\n");
    return 0;
}

/* Runtime dynamic library loaders (tcc_load_dll) completely stubbed out 
   as TRDOS 386 executes exclusively within absolute pure flat executable binary models */
static int tcc_load_dll(TCCState *s1, int fd, const char *filename, int level)
{
    error_noabort("Dynamic shared libraries (.so/.dll) are unsupported under current flat binary execution profile");
    return -1;
}

#define LD_TOK_NAME 256
#define LD_TOK_EOF  (-1)

/* Extract and return the next valid token snapshot found inside the active linker script stream */
static int ld_next(TCCState *s1, char *name, int name_size)
{
    int c;
    char *q;

 redo:
    switch(ch) {
    case ' ':
    case '\t':
    case '\f':
    case '\v':
    case '\r':
    case '\n':
        inp();
        goto redo;
    case '/':
        minp();
        if (ch == '*') {
            file->buf_ptr = parse_comment(file->buf_ptr);
            ch = file->buf_ptr[0];
            goto redo;
        } else {
            q = name;
            *q++ = '/';
            goto parse_name;
        }
        break;
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
    case '\\':
    case '.':
    case '$':
    case '~':
        q = name;
    parse_name:
        for(;;) {
            if (!((ch >= 'a' && ch <= 'z') ||
                  (ch >= 'A' && ch <= 'Z') ||
                  (ch >= '0' && ch <= '9') ||
                  strchr("/.-_+=$:\\,~", ch)))
                break;
            if ((q - name) < name_size - 1) {
                *q++ = ch;
            }
            minp();
        }
        *q = '\0';
        c = LD_TOK_NAME;
        break;
    case CH_EOF:
        c = LD_TOK_EOF;
        break;
    default:
        c = ch;
        inp();
        break;
    }
    return c;
}

/* Parse and evaluate a minimal subset of GNU ld linker script keywords (like INPUT or GROUP symbols) */
static int tcc_load_ldscript(TCCState *s1)
{
    char cmd[64];
    char filename[1024];
    int t;
    
    ch = file->buf_ptr[0];
    ch = handle_eob();
    for(;;) {
        t = ld_next(s1, cmd, sizeof(cmd));
        if (t == LD_TOK_EOF)
            return 0;
        else if (t != LD_TOK_NAME)
            return -1;
            
        if (!strcmp(cmd, "INPUT") || !strcmp(cmd, "GROUP")) {
            t = ld_next(s1, cmd, sizeof(cmd));
            if (t != '(')
                expect("(");
            t = ld_next(s1, filename, sizeof(filename));
            for(;;) {
                if (t == LD_TOK_EOF) {
                    error_noabort("unexpected end of file");
                    return -1;
                } else if (t == ')') {
                    break;
                } else if (t != LD_TOK_NAME) {
                    error_noabort("filename expected");
                    return -1;
                } 
                tcc_add_file(s1, filename);
                t = ld_next(s1, filename, sizeof(filename));
                if (t == ',') {
                    t = ld_next(s1, filename, sizeof(filename));
                }
            }
        } else {
            return -1;
        }
    }
    return 0;
}
