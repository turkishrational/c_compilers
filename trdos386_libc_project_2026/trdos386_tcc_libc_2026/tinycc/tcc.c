/*
 *  TCC - Tiny C Compiler
 * 
 *  Copyright (c) 2001-2004 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef ONE_SOURCE
# define ONE_SOURCE 1
#endif

#include "tcc.h"
#if ONE_SOURCE
# include "libtcc.c"
#endif
#include "tcctools.c"

/* 18/6/2026 - Google AI */
// extern int trdos_print(const char *format, ...);
// #define printf trdos_print

static const char help[] = "TCC TRDOS 386 HELP TEST\n";

static const char help2[] =
    "Tiny C Compiler "TCC_VERSION" - More Options\n"
    "Special options:\n"
    "  -P -P1                        with -E: no/alternative #line output\n"
    "  -dD -dM                       with -E: output #define directives\n"
    "  -pthread                      same as -D_REENTRANT and -lpthread\n"
    "  -On                           same as -D__OPTIMIZE__ for n > 0\n"
    "  -Wp,-opt                      same as -opt\n"
    "  -include file                 include 'file' above each input file\n"
    "  -nostdlib                     do not link with standard crt/libs\n"
    "  -isystem dir                  add 'dir' to system include path\n"
    "  -static                       link to static libraries (not recommended)\n"
    "  -dumpversion                  print version\n"
    "  -print-search-dirs            print search paths\n"
    "  -dt                           with -run/-E: auto-define 'test_...' macros\n"
    "Ignored options:\n"
    "  -arch -C --param -pedantic -pipe -s -traditional\n"
    "-W[no-]... warnings:\n"
    "  all                           turn on some (*) warnings\n"
    "  error[=warning]               stop after warning (any or specified)\n"
    "  write-strings                 strings are const\n"
    "  unsupported                   warn about ignored options, pragmas, etc.\n"
    "  implicit-function-declaration warn for missing prototype (*)\n"
    "  discarded-qualifiers          warn when const is dropped (*)\n"
    "-f[no-]... flags:\n"
    "  unsigned-char                 default char is unsigned\n"
    "  signed-char                   default char is signed\n"
    "  common                        use common section instead of bss\n"
    "  leading-underscore            decorate extern symbols\n"
    "  ms-extensions                 allow anonymous struct in struct\n"
    "  dollars-in-identifiers        allow '$' in C symbols\n"
    "  reverse-funcargs              evaluate function arguments right to left\n"
    "  gnu89-inline                  'extern inline' is like 'static inline'\n"
    "  asynchronous-unwind-tables    create eh_frame section [on]\n"
    "  test-coverage                 create code coverage code\n"
    "-m... target specific options:\n"
    "  ms-bitfields                  use MSVC bitfield layout\n"
#ifdef TCC_TARGET_ARM
    "  float-abi                     hard/softfp on arm\n"
#endif
#ifdef TCC_TARGET_X86_64
    "  no-sse                        disable floats on x86_64\n"
#endif
    "-Wl,... linker options:\n"
    "  -nostdlib                     do not search standard library paths\n"
    "  -[no-]whole-archive           load lib(s) fully/only as needed\n"
    "  -export-all-symbols           same as -rdynamic\n"
    "  -export-dynamic               same as -rdynamic\n"
    "  -image-base= -Ttext=          set base address of executable\n"
    "  -section-alignment=           set section alignment in executable\n"
#ifdef TCC_TARGET_PE
    "  -file-alignment=              set PE file alignment\n"
    "  -stack=                       set PE stack reserve\n"
    "  -large-address-aware          set related PE option\n"
    "  -subsystem=[console/windows]  set PE subsystem\n"
    "  -oformat=[pe-* binary]        set executable output format\n"
    "Predefined macros:\n"
    "  tcc -E -dM - < nul\n"
#else
    "  -rpath=                       set dynamic library search path\n"
    "  -enable-new-dtags             set DT_RUNPATH instead of DT_RPATH\n"
    "  -soname=                      set DT_SONAME elf tag\n"
#if defined(TCC_TARGET_MACHO)
    "  -install_name=                set DT_SONAME elf tag (soname macOS alias)\n"
#else
    "  -Ipath, -dynamic-linker=path  set ELF interpreter to path\n"
#endif
    "  -Bsymbolic                    set DT_SYMBOLIC elf tag\n"
    "  -oformat=[elf32/64-* binary]  set executable output format\n"
    "  -init= -fini= -Map= -as-needed -O -z= (ignored)\n"
    "Predefined macros:\n"
    "  tcc -E -dM - < /dev/null\n"
#endif
    "See also the manual for more details.\n"
    ;

static const char version[] =
    "tcc version "TCC_VERSION
#ifdef TCC_GITHASH
    " "TCC_GITHASH
#endif
    " ("
#ifdef TCC_TARGET_I386
        "i386"
#elif defined TCC_TARGET_X86_64
        "x86_64"
#elif defined TCC_TARGET_C67
        "C67"
#elif defined TCC_TARGET_ARM
        "ARM"
# ifdef TCC_ARM_EABI
        " eabi"
#  ifdef TCC_ARM_HARDFLOAT
        "hf"
#  endif
# endif
#elif defined TCC_TARGET_ARM64
        "AArch64"
#elif defined TCC_TARGET_RISCV64
        "riscv64"
#endif
#ifdef TCC_TARGET_PE
        " Windows"
#elif defined(TCC_TARGET_MACHO)
        " Darwin"
#elif TARGETOS_FreeBSD || TARGETOS_FreeBSD_kernel
        " FreeBSD"
#elif TARGETOS_OpenBSD
        " OpenBSD"
#elif TARGETOS_NetBSD
        " NetBSD"
#else
        " Linux"
#endif
    ")\n"
    ;

static void print_dirs(const char *msg, char **paths, int nb_paths)
{
    int i;
    printf("%s:\n%s", msg, nb_paths ? "" : "  -\n");
    for(i = 0; i < nb_paths; i++)
        printf("  %s\n", paths[i]);
}

static void print_search_dirs(TCCState *s)
{
    printf("install: %s\n", s->tcc_lib_path);
    /* print_dirs("programs", NULL, 0); */
    print_dirs("include", s->sysinclude_paths, s->nb_sysinclude_paths);
    print_dirs("libraries", s->library_paths, s->nb_library_paths);
    printf("libtcc1:\n  %s/%s\n", s->library_paths[0], CONFIG_TCC_CROSSPREFIX TCC_LIBTCC1);
#ifdef TCC_TARGET_UNIX
    print_dirs("crt", s->crt_paths, s->nb_crt_paths);
    printf("elfinterp:\n  %s\n",  DEFAULT_ELFINTERP(s));
#endif
}

static void set_environment(TCCState *s)
{
    /* 20/6/2026 - TRDOS Ring 3 Pointer Koruma Zýrhý */
    
    // Dinamik dizi motorunu (tcc_split_path) bypass ediyoruz.
    // Doðrudan derleyicinin .rodata alanýndaki sabit adresleri baðlýyoruz:
    
    static const char* trdos_sys_includes[1] = { "D:/tcc/include/sys" };
    static const char* trdos_includes[1]     = { "D:/tcc/include" };
    static const char* trdos_libs[1]         = { "D:/tcc/lib" };

    s->sysinclude_paths = (char **)trdos_sys_includes;
    s->nb_sysinclude_paths = 1;

    s->include_paths = (char **)trdos_includes;
    s->nb_include_paths = 1;

    s->library_paths = (char **)trdos_libs;
    s->nb_library_paths = 1;
}

static char *default_outputfile(TCCState *s, const char *first_file)
{
    char buf[1024];
    char *ext;
    const char *name = "a";

    if (first_file && strcmp(first_file, "-"))
        name = tcc_basename(first_file);
    if (strlen(name) + 4 >= sizeof buf)
        name = "a";
    strcpy(buf, name);
    ext = tcc_fileextension(buf);
#ifdef TCC_TARGET_PE
    if (s->output_type == TCC_OUTPUT_DLL)
        strcpy(ext, ".dll");
    else
    if (s->output_type == TCC_OUTPUT_EXE)
        strcpy(ext, ".exe");
    else
#endif
    if ((s->just_deps || s->output_type == TCC_OUTPUT_OBJ) && !s->option_r && *ext)
        strcpy(ext, ".o");
    else
        strcpy(buf, "a.out");
    return tcc_strdup(buf);
}

static unsigned getclock_ms(void)
{
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000 + (tv.tv_usec+500)/1000;
#endif
}

int main(int argc, char **argv)
{
    TCCState *s, *s1;
    int ret, opt, n = 0, t = 0, done;
    unsigned start_time = 0, end_time = 0;
    const char *first_file;
    int argc0 = argc;
    char **argv0 = argv;
    FILE *ppfp = stdout;

/* =================================================================== */
/* TRDOS 386 - DIRECT _SPRINT ISOLATION DEBUG BLOCK (FIXED LINK)       */
/* =================================================================== */
{
    /* Flat binary virtual memory üzerinde izole bir test tamponu açýyoruz */
    static char debug_buffer[256]; /* Flat binary için static veya array tabaný */
    int test_version_major = 0;
    int test_version_minor = 9;
    int test_version_patch = 28;
    unsigned int test_hex_val = 0xABCDE123;
    
    /* GCC'nin otomatik alt çizgi dekorasyonunu bypass eden __asm__ zýrhý */
    extern int safe_sprint_engine(char *buf, const char *fmt, void *args) __asm__("_sprint");
    
    /* C calling convention gereði argümanlarý yýðýn düzenine sokuyoruz */
    struct {
        int maj;
        int min;
        int pat;
        unsigned int hx;
    } test_args;
    
    test_args.maj = test_version_major;
    test_args.min = test_version_minor;
    test_args.pat = test_version_patch;
    test_args.hx  = test_hex_val;

    /* Tamponu çöp verilerden korumak için temizliyoruz */
    for(int b = 0; b < 256; b++) debug_buffer[b] = 0;

    /* _sprint motorunu doðrudan gerçek sembol adýyla tetikliyoruz */
    safe_sprint_engine(debug_buffer, "Tiny C Compiler Version %d.%d.%d for TRDOS 386 (HEX: %X)\n", &test_args);

    /* Sonucu formatsýz, saf ve düz metin olarak ekrana basýyoruz */
    trdos_print("%s", debug_buffer);
}
/* =================================================================== */
/* END OF DEBUG BLOCK                                                  */
/* =================================================================== */

redo:
    argc = argc0, argv = argv0;

    s = s1 = tcc_new();

    opt = tcc_parse_args(s, &argc, &argv);

    if (n == 0) {
        ret = 0;
        if (opt == OPT_HELP) {
            fputs(help, stdout);
            if (s->verbose)
                goto help2;
        } else if (opt == OPT_HELP2) {
            help2: fputs(help2, stdout);
        } else if (opt == OPT_M32 || opt == OPT_M64) {
            ret = tcc_tool_cross(argv, opt);
        } else if (s->verbose)
            printf("%s", version);

        if (opt == OPT_AR)
            ret = tcc_tool_ar(argc, argv);
#ifdef TCC_TARGET_PE
        if (opt == OPT_IMPDEF)
            ret = tcc_tool_impdef(argc, argv);
#endif
        if (opt == OPT_PRINT_DIRS) {
            /* initialize search dirs */
            set_environment(s);
            tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
            print_search_dirs(s);
        }
        if (opt) {

            #ifdef TCC_TARGET_I386
            {
                extern int write(int fd, const void *buf, unsigned int count);
                write(1, "TRDOS_TRACE: Forced exit due to (opt != 0) condition!\r\n", 55);
            }
            #endif

            if (opt < 0) err:
                ret = 1;
            tcc_delete(s);
            return ret;
        }
        if (s->nb_files == 0) {

            #ifdef TCC_TARGET_I386
            {
                extern int write(int fd, const void *buf, unsigned int count);
                write(1, "TRDOS_TRACE: Forced exit due to (nb_files == 0)!\r\n", 50);
            }
            #endif

            tcc_error_noabort("no input files");
        } else if (s->output_type == TCC_OUTPUT_PREPROCESS) {
            if (s->outfile && 0!=strcmp("-",s->outfile)) {
                ppfp = fopen(s->outfile, "wb");
                if (!ppfp)
                    tcc_error_noabort("could not write '%s'", s->outfile);
            }
        } else if (s->output_type == TCC_OUTPUT_OBJ && !s->option_r) {
            if (s->nb_libraries)
                tcc_error_noabort("cannot specify libraries with -c");
            else if (s->nb_files > 1 && s->outfile)
                tcc_error_noabort("cannot specify output file with -c many files");
        }
        if (s->nb_errors) {
            #ifdef TCC_TARGET_I386
            {
                extern int write(int fd, const void *buf, unsigned int count);
                write(1, "TRDOS_TRACE: Forced exit due to (nb_errors != 0)!\r\n", 51);
            }
            #endif
            goto err;
        }  
        if (s->do_bench)
           start_time = getclock_ms();
    }

    set_environment(s);

    if (s->output_type == 0)
        s->output_type = TCC_OUTPUT_EXE;
    tcc_set_output_type(s, s->output_type);
    s->ppfp = ppfp;

    if ((s->output_type == TCC_OUTPUT_MEMORY
      || s->output_type == TCC_OUTPUT_PREPROCESS)
        && (s->dflag & 16)) { /* -dt option */
        if (t)
            s->dflag |= 32;
        s->run_test = ++t;
        if (n)
            --n;
    }

    /* compile or add each files or library */
    first_file = NULL;
    do {
        struct filespec *f = s->files[n];
        s->filetype = f->type;
        /* 18/6/2026 - TRDOS 386 Dosya Giris Ýzleme Noktasý */
        trdos_print("\n[LOOP] n=%d, nb_files=%d, Dosya Adi: '%s', Tip: 0x%X\n", n, s->nb_files, f->name, f->type);

        if (f->type & AFF_TYPE_LIB) {
            ret = tcc_add_library(s, f->name);
            trdos_print("-> tcc_add_library donus kodu (ret): %d\n", ret);
        } else {
            if (1 == s->verbose)
                trdos_print("-> %s\n", f->name); /* printf yerine trdos_print garantisi */
            if (!first_file)
                first_file = f->name;
            
            ret = tcc_add_file(s, f->name);
            trdos_print("-> tcc_add_file donus kodu (ret): %d\n", ret);
        }

        /* Eger dosya ekleme basarýsýz olduysa nedenini gorelim */
        if (ret != 0) {
            trdos_print("[HATA] Dosya ekleme dongusu kesildi! s->nb_errors: %d\n", s->nb_errors);
        }

    } while (++n < s->nb_files
            && 0 == ret
            && (s->output_type != TCC_OUTPUT_OBJ || s->option_r));

    if (s->do_bench)
        end_time = getclock_ms();

    if (s->run_test) {
        t = 0;
    } else if (s->output_type == TCC_OUTPUT_PREPROCESS) {
        ;
    } else if (0 == ret) {
        if (s->output_type == TCC_OUTPUT_MEMORY) {
#ifdef TCC_IS_NATIVE
            ret = tcc_run(s, argc, argv);
#endif
        } else {
            if (!s->outfile)
                s->outfile = default_outputfile(s, first_file);
            if (!s->just_deps)
                ret = tcc_output_file(s, s->outfile);
            if (!ret && s->gen_deps)
                gen_makedeps(s, s->outfile, s->deps_outfile);
        }
    }

    done = 1;
    if (t)
        done = 0; /* run more tests with -dt -run */
    else if (ret) {
        if (s->nb_errors)
            ret = 1;
        /* else keep the original exit code from tcc_run() */
    } else if (n < s->nb_files)
        done = 0; /* compile more files with -c */
    else if (s->do_bench)
        tcc_print_stats(s, end_time - start_time);

    tcc_delete(s);
    if (!done)
        goto redo;
    if (ppfp && ppfp != stdout)
        fclose(ppfp);
    return ret;
}