/* 24/6/2026 - Google AI */

#define NULL ((void*)0)
#define REALLOC_AREA 0x02800000

/* LIBC temel hafýza kopyalama fonksiyonu prototipi */
extern void *memcpy(void *dest, const void *src, unsigned int n);

/* Sanal doðrusal heap göstergeleri (Dünkü akýllý zýrh korundu) */
static unsigned long current_heap_ptr  = REALLOC_AREA;
static unsigned long allocated_limit   = REALLOC_AREA;
static unsigned long last_allocated_ptr  = 0;
static unsigned long last_allocated_size = 0;

/* =========================================================================
   TRDOS-386 GÜVENLÝ DOÐRUSAL HEAP VE DÝNAMÝK ARDIÞIK BÜYÜME MOTORU
   ========================================================================= */
void *_mingw_realloc(void *ptr, unsigned int size) {
    void *new_ptr;

    /* Sayfa bitmap korumasý: Çekirdeði yormamak için size=0 ise pas geçiyoruz */
    if (size == 0) return NULL;

    /* x86 mimarisi için DWORD (4-byte) hizalamasý */
    unsigned long aligned_size = (size + 3) & ~3;

    /* 1. DURUM: [YERÝNDE GENÝÞLEME ZIRHI] - Peþ peþe gelen tablo büyütme talepleri */
    if (ptr != NULL && (unsigned long)ptr == last_allocated_ptr) {
        unsigned long next_heap_ptr = last_allocated_ptr + aligned_size;

        if (next_heap_ptr > allocated_limit) {
            unsigned long needed_bytes = next_heap_ptr - allocated_limit;
            /* 4096 sayfa sýnýrýna göre yukarý yuvarlama (Round-Up) */
            unsigned long page_chunks = (needed_bytes + 4095) / 4096;
            unsigned long new_pages_start = allocated_limit;
            unsigned long new_pages_bytes = page_chunks * 4096;

            /* Çekirdeðin Page Fault Handler mekanizmasýný tetiklemek için 
               Ring 3 seviyesinde yeni sayfa sýnýrlarýna güvenle dokunuyoruz */
            unsigned long check_addr = new_pages_start;
            unsigned int idx;
            for (idx = 0; idx < page_chunks; idx++) {
                volatile char *touch = (volatile char *)check_addr;
                *touch = 0; /* Kernel burada boþ fiziksel sayfayý Ring 3'e atar */
                check_addr += 4096;
            }
            
            allocated_limit += new_pages_bytes;
        }

        current_heap_ptr = next_heap_ptr;
        last_allocated_size = aligned_size;
        return ptr; 
    }

    /* 2. DURUM: [YENÝ BELLEK ALANI TAHSÝSÝ] */
    new_ptr = (void*)current_heap_ptr;
    unsigned long next_heap_ptr = current_heap_ptr + aligned_size;

    if (next_heap_ptr > allocated_limit) {
        unsigned long needed_bytes = next_heap_ptr - allocated_limit;
        unsigned long page_chunks = (needed_bytes + 4095) / 4096;
        unsigned long new_pages_start = allocated_limit;
        unsigned long new_pages_bytes = page_chunks * 4096;

        /* Güvenli Yazma (Write) Page Fault Tetikleyici Döngü */
        unsigned long check_addr = new_pages_start;
        unsigned int idx;
        for (idx = 0; idx < page_chunks; idx++) {
            volatile char *touch = (volatile char *)check_addr;
            *touch = 0;
            check_addr += 4096;
        }

        allocated_limit += new_pages_bytes;
    }

    /* Eðer bu bir geniþletme (realloc) ise, eski verileri güvenle taþý */
    if (ptr != NULL) {
        unsigned long copy_size = (last_allocated_size < size) ? last_allocated_size : size;
        memcpy(new_ptr, ptr, copy_size);
    }

    /* Durum haritasýný güncelle */
    last_allocated_ptr  = (unsigned long)new_ptr;
    last_allocated_size = aligned_size;
    current_heap_ptr    = next_heap_ptr;

    return new_ptr;
}

/* Orijinal TCC nesne dosyalarýnýn arayacaðý global standart semboller */
void *realloc(void *ptr, unsigned int size) {
    return _mingw_realloc(ptr, size);
}

// void *malloc(unsigned int size) {
//     return _mingw_realloc(NULL, size);
// }

/* 26/6/2026 - Google AI */
/* TRDOS Kernel Sistem Çaðrýsý Yardýmcýlarý */
// #define SYS_BREAK 17

void *malloc(unsigned int size) {
    unsigned int current_break = 0;
    unsigned int new_break = 0;
    unsigned int aligned_size = 0;
    char *ptr;
    unsigned int i;

    if (size == 0) return NULL;

    /* 1. ADIM: 4-Byte i386 Dword Hizalamasý (Small-C and al, not 3 mantýðý) */
    aligned_size = (size + 3) & ~3;

    /* 2. ADIM: sys_break, -1 ile mevcut u.break (BSS sonu) adresini kernel'dan alýyoruz */
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "mov ebx, -1\n"
        "mov eax, 17\n"     /* sys_break çaðýrma numarasý */
        "int 0x40\n"
        "mov %0, eax\n"     /* Dönen mevcut u.break adresini yakala */
        ".att_syntax\n"
        : "=r" (current_break)
        :: "eax", "ebx"
    );

    /* 4-Byte Hizalamayý mevcut adrese de uyguluyoruz */
    current_break = (current_break + 3) & ~3;

    /* Yeni hedef break noktasý hesaplanýyor */
    new_break = current_break + aligned_size;

    /* 3. ADIM: sys_break, new_break ile kernel'daki u.break sýnýrýný geniþletiyoruz */
    int status = 0;
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n"
        "mov ebx, %1\n"     /* ebx = yeni u.break adresi */
        "mov eax, 17\n"
        "int 0x40\n"
        "mov %0, eax\n"     /* Baþarý durumunu al */
        ".att_syntax\n"
        : "=r" (status)
        : "r" (new_break)
        : "eax", "ebx"
    );

    /* Carry flag set ise veya kernel hata döndüyse (bellek yetersiz) NULL dön */
    if (status < 0) {
        return NULL;
    }

    /* 4. ADIM: DEMAND PAGING VE ZERO-FILL ZIRHI! */
    /* Yeni ayrýlan bellek alanýnýn baþlangýç adresini ptr yapýyoruz */
    ptr = (char *)current_break;

    /* Kernel'ýn gizlice fiziksel sayfa atamasýný (Page Fault telafisini) tetiklemek ve */
    /* çöpm verileri temizlemek için alana her byte için el ile Sýfýr (0) yazýyoruz (Write aný) */
    for (i = 0; i < aligned_size; i++) {
        ptr[i] = 0; /* Bu yazma iþlemi kernel'a güvenli sayfa (page) ayartýrýr! */
    }

    /* Baþlangýç adresini döndür */
    return (void *)ptr;
}


void *calloc(unsigned int nelem, unsigned int elsize) {
    return _mingw_realloc(NULL, nelem * elsize);
}

/* Açýklamalarýnýz doðrultusunda free çaðrýlarý tamamen sönümleniyor */
void free(void *ptr) {
    /* TRDOS-386 üzerinde sayfa delinmesini ve bitmap þiþmesini önlemek için 
       Ring 3 düzeyinde serbest býrakma talepleri güvenle yutulur. */
}

void _mingw_free(void *ptr) {
    /* Baðýmlýlýk Stub */
}