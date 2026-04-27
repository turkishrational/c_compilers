int fputs(const char *s, FILE *fp) {
    int fd = (int)(uintptr_t)fp;
    int mode = (fd == 2) ? 3 : 2; // stderr (2) ise BL=3, stdout (1) ise BL=2
    
    while (*s) {
        __asm__ __volatile__ (
            "int $0x40"
            :
            : "a"(46), "b"(mode), "c"(*s++)
        );
    }
    return 0;
}