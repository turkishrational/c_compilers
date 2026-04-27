// Temel syscall fonksiyonu (inline)
static inline int trdos_syscall(int a, int b, int c, int d, int s, int D) {
    int res;
    __asm__ __volatile__ (
        "int $0x40"
        : "=a"(res)
        : "a"(a), "b"(b), "c"(c), "d"(d), "S"(s), "D"(D)
        : "memory"
    );
    return res;
}

// NASM'daki 'sys' makrosunun C karþýlýðý (Variadic Macro)
#define get_macro(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define sys(...) get_macro(__VA_ARGS__, sys6, sys5, sys4, sys3, sys2, sys1)(__VA_ARGS__)

// Farklý argüman sayýlarý için alt makrolar
#define sys1(eax)                         trdos_syscall(eax, 0, 0, 0, 0, 0)
#define sys2(eax, ebx)                    trdos_syscall(eax, ebx, 0, 0, 0, 0)
#define sys3(eax, ebx, ecx)               trdos_syscall(eax, ebx, ecx, 0, 0, 0)
#define sys4(eax, ebx, ecx, edx)          trdos_syscall(eax, ebx, ecx, edx, 0, 0)
#define sys5(eax, ebx, ecx, edx, esi)     trdos_syscall(eax, ebx, ecx, edx, esi, 0)
#define sys6(eax, ebx, ecx, edx, esi, edi) trdos_syscall(eax, ebx, ecx, edx, esi, edi)