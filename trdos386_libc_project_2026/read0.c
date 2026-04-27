// syscall_internals.c veya asm içinde
int read(int fd, void* buf, int count) {
    if (fd < 3) {
        // sysstdio üzerinden okuma (TRDOS'a özgü parametreler)
        return trdos_sysstdio_read(fd, buf, count);
    }
    // Normal dosya okuma (FD-3 mapping)
    return syscall(3, fd - 3, (int)buf, count, 0, 0); 
}