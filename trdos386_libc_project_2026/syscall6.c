int syscall(int eax, int ebx, int ecx, int edx, int esi, int edi) {
    int res;
    __asm__ __volatile__ (
        "int $0x40"
        : "=a"(res)                // Çýktý: EAX register'ýndan res deðiþkenine
        : "a"(eax), "b"(ebx), "c"(ecx), "d"(edx), "S"(esi), "D"(edi)
        : "memory"                 // Bellek yan etkileri için güvenlik önlemi
    );
    return res;
}
