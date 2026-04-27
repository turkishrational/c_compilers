// fgetc: Tek bir karakteri anýnda çeker
int fgetc(FILE *fp) {
    int fd = (int)(uintptr_t)fp;
    if (fd == 0) { // stdin
        int c;
        __asm__ __volatile__ (
            "int $0x40"
            : "=a"(c)
            : "a"(46), "b"(0) // sys_stdio, BL=0 (read wait)
        );
        return c & 0xFF;
    }
    // Dosyadan okuma iþlemi (read(fd, &c, 1) çaðrýsý)
    return _file_read_char(fd);
}