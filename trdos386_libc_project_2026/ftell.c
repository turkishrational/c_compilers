// ftell(fp) simülasyonu
long ftell(FILE *fp) {
    int fd = (int)(uintptr_t)fp - 3; // TRDOS FD'sine geri dön
    // sys_lseek: EAX=19, EBX=fd, ECX=0, EDX=1 (SEEK_CUR)
    return syscall(19, fd, 0, 1, 0, 0); 
}