// Temel sarmalayýcý makrolar
#define syscall0(eax) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax) : "memory"); \
    res; \
})

#define syscall1(eax, ebx) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax), "b"(ebx) : "memory"); \
    res; \
})

#define syscall2(eax, ebx, ecx) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax), "b"(ebx), "c"(ecx) : "memory"); \
    res; \
})

#define syscall3(eax, ebx, ecx, edx) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx) : "memory"); \
    res; \
})

#define syscall4(eax, ebx, ecx, edx, esi) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx), "S"(esi) : "memory"); \
    res; \
})

// Ýhtiyaca göre 5'e kadar devam edebilir...
#define syscall5(eax, ebx, ecx, edx, esi, edi) ({ \
    int res; \
    __asm__ __volatile__ ("int $0x40" : "=a"(res) : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx), "S"(esi), "D"(edi) : "memory"); \
    res; \
})