#define NULL ((void*)0)

/* Ensure linker can find symbols with underscore prefix mechanism */
extern int open(const char *pathname, int flags, int mode);
extern int close(int fd);
extern int read(int fd, void *buf, unsigned int count);
extern int write(int fd, const void *buf, unsigned int count);
extern long lseek(int fd, long offset, int whence);
extern long tell(int fd);

/* 24/6/2026 */
/* =========================================================================
   PHASE 3: MINGW UYUMLU DOÐAL HÝZALAMALI 32-BYTE STRUCT YAPISI
   ========================================================================= */

/* pragma pack(1) kaldýrýldý! TCC ile tam offset uyumu için 4-byte doðal hizalama zorunlu */
struct mingw_internal_iob {
    char *_ptr;       /* +0  */
    int   _cnt;       /* +4  */
    char *_base;      /* +8  */
    int   _flag;      /* +12 */
    int   _file;      /* +16 - TCC tam bu offseti okur */
    char  _charbuf;   /* +20 */
    char  _pad[3];    /* +21 - _bufsiz'ý 4-byte sýnýrýna hizalamak için dolgu */
    int   _bufsiz;    /* +24 */
    int   _reserved;  /* +28 - Tam 32 byte'a tamamlayan son 4 byte */
};

/* 
   Ýlklendirme sorununu kökten çözmek için fonksiyona güvenmiyoruz!
   Diziyi derleme anýnda (Compile-time) direkt statik olarak kilitliyoruz.
*/
struct mingw_internal_iob _iob[3] = {
    { NULL, 0, NULL, 0, 0, 0, {0}, 0, 0 }, /* idx = 0: stdin  -> _file = 0 */
    { NULL, 0, NULL, 0, 1, 0, {0}, 0, 0 }, /* idx = 1: stdout -> _file = 1 */
    { NULL, 0, NULL, 0, 2, 0, {0}, 0, 0 }  /* idx = 2: stderr -> _file = 2 */
};

void * _imp___iob = _iob;

/* High-Level IO Functions mapping directly to TRDOS 386 System Operations */

// void *fopen(const char *filename, const char *mode) {
//    int flags = 0;
//    int trdos_fd;
//    if (mode[0] == 'r') flags = 0;
//    else if (mode[0] == 'w') flags = 1;
//    else if (mode[0] == 'a') flags = 2;
//
//    trdos_fd = open(filename, flags, 0);
//    if (trdos_fd < 0) return NULL;
//    return (void*)trdos_fd;
// }

// int fclose(void *stream) {
//    int fd = (int)stream;
//    if (fd >= 3 && fd <= 12) return close(fd);
//    return -1;
// }

/* =========================================================================
   AKILLI FREAD & FWRITE DÖNÜÞTÜRÜCÜSÜ (PHASE 3)
   ========================================================================= */

unsigned int fread(void *ptr, unsigned int size, unsigned int count, void *stream) {
    int fd;
    int bytes_read;

    if (stream == NULL) return 0;

    /* Eðer TCC, stdin/stdout/stderr adresi gönderdiyse */
    if ((char*)stream >= (char*)_iob && (char*)stream < ((char*)_iob + 96)) {
        int idx = ((char*)stream - (char*)_iob) / 32; /* Tam 32-byte hizalý indeks bulucu */
        fd = _iob[idx]._file;
    } else {
        /* Düz handle veya fopen'dan gelen sayýsal deðer */
        fd = (int)stream;
    }

    if (fd < 0) return 0;

    bytes_read = read(fd, ptr, size * count);
    if (bytes_read <= 0) return 0;
    return (bytes_read / size);
}

unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, void *stream) {
    int fd;
    int bytes_written;

    if (stream == NULL) return 0;

    /* Eðer TCC, stdin/stdout/stderr adresi gönderdiyse */
    if ((char*)stream >= (char*)_iob && (char*)stream < ((char*)_iob + 96)) {
        int idx = ((char*)stream - (char*)_iob) / 32; /* Tam 32-byte hizalý indeks bulucu */
        fd = _iob[idx]._file;

    } else {
        /* Düz handle veya fopen'dan gelen sayýsal deðer */
        fd = (int)stream;
    }

    if (fd < 0) return 0;

    bytes_written = write(fd, ptr, size * count);
    if (bytes_written <= 0) return 0;
    return (bytes_written / size);
}

/* Connect fseek and ftell directly to our new TRDOS _lseek call */
int fseek(void *stream, long int offset, int whence) {
    int fd = (int)stream;
    long res = lseek(fd, offset, whence);
    if (res < 0) return -1;
    return 0;
}

long int ftell(void *stream) {

    int fd = (int)stream;

    /* Direct TRDOS 386 systell kernel interrupt call instead of seek simulation */

    return tell(fd); 
}

int fgetc(void *stream) {
    char ch;
    if (fread(&ch, 1, 1, stream) == 1) return (int)ch;
    return -1;
}

int fputc(int character, void *stream) {
    char ch = (char)character;
    if (fwrite(&ch, 1, 1, stream) == 1) return character;
    return -1;
}

int fputs(const char *str, void *stream) {
    unsigned int len = 0;
    while(str[len]) len++;
    return fwrite(str, 1, len, stream);
}

int fflush(void *stream) { return 0; }
void exit(int status) { while(1); }
char *getenv(const char *name) { return NULL; }
int remove(const char *filename) { return 0; }
unsigned long __stdcall GetTickCount(void) { return 0; }
int _spawnvp(int mode, const char *cmdname, const char *const *argv) { return -1; }
int _cwait(int *termstat, int procHandle, int action) { return 0; }
