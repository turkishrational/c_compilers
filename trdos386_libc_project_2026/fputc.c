// stdio.h içindeki tanım
typedef void FILE; // FILE artık bir yapı değil, belirsiz bir tip

FILE* fopen(const char* path, const char* mode) {
    int fd = open(path, mode); // Bu bizim yazdığımız EAX=5 + 3 dönen fonksiyon
    if (fd == -1) return NULL;
    return (FILE*)(uintptr_t)fd; // Sayıyı adrese dönüştür (Örn: 3 -> 0x00000003)
}

int fputc(int c, FILE* stream) {
    int fd = (int)(uintptr_t)stream; // Adresi tekrar sayıya çevir
    // write(fd, &c, 1) çağrısı yapılır...
}