// int read(int fd, void *buf, int count) içindeki STDIN kýsmý
if (fd == 0) { // stdin
    char *p = (char *)buf;
    int i;
    for (i = 0; i < count; i++) {
        int c = getc(); // Doðrudan sys_stdio çaðýrýr
        p[i] = (char)c;
        if (c == '\n' || c == '\r') { // Satýr sonu geldiðinde dön
            i++;
            break;
        }
    }
    return i; // Okunan net miktar
}