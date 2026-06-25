#define NULL ((void*)0)

/* Ensure linker can find symbols with underscore prefix mechanism */
extern int open(const char *pathname, int flags, int mode);
extern int close(int fd);
extern int read(int fd, void *buf, unsigned int count);
extern int write(int fd, const void *buf, unsigned int count);
extern long lseek(int fd, long offset, int whence);
extern long tell(int fd);

/* FILE simulation using direct integer file descriptors */
typedef struct {
    int fd;
} TRDOS_FILE;

/* Array mapping to match TCC internal logic for stdin/stdout/stderr */
TRDOS_FILE _iob[3] = { {0}, {1}, {2} };
TRDOS_FILE * _imp___iob = _iob;

/* High-Level IO Functions mapping directly to TRDOS 386 System Operations */

void *fopen(const char *filename, const char *mode) {
    int flags = 0;
    int trdos_fd;
    if (mode[0] == 'r') flags = 0;
    else if (mode[0] == 'w') flags = 1;
    else if (mode[0] == 'a') flags = 2;

    trdos_fd = open(filename, flags, 0);
    if (trdos_fd < 0) return NULL;
    return (void*)trdos_fd;
}

int fclose(void *stream) {
    int fd = (int)stream;
    if (fd >= 0) return close(fd);
    return -1;
}

// unsigned int fread(void *ptr, unsigned int size, unsigned int count, void *stream) {
//  int fd = (int)stream;
//  int bytes_read;
//  if (stream == &_iob) fd = 0;
//  bytes_read = read(fd, ptr, size * count);
//  if (bytes_read <= 0) return 0;
//  return (bytes_read / size);
// }

// unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, void *stream) {
//  int fd = (int)stream;
//  int bytes_written;
//  if (stream == &_iob) fd = 1;
//  else if (stream == &_iob) fd = 2;
//  bytes_written = write(fd, ptr, size * count);
//  if (bytes_written <= 0) return 0;
//  return (bytes_written / size);
// }

/* 16/6/2026 - Google AI */
/* ===========================================================================
   TRDOS 386 Libc - Defensively Patched fread & fwrite for TCC Port
   =========================================================================== */

unsigned int fread(void *ptr, unsigned int size, unsigned int count, void *stream) {
    int fd;
    int bytes_read;

    if (stream == NULL) return 0;

    /* If the stream pointer is within our _iob array memory boundaries */
    if ((char*)stream >= (char*)_iob && (char*)stream <= (char*)&_iob[2]) {
        /* Safely extract the raw file descriptor byte or index directly */
        fd = ((TRDOS_FILE*)stream)->fd;
    } else {
        /* High-level file pointers created via fopen carry the fd as raw pointer value cast */
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

    /* Defensively check if the stream address belongs to our static _iob framework */
    if ((char*)stream >= (char*)_iob && (char*)stream <= (char*)&_iob[2]) {
        /* Directly read the actual 4-byte descriptor value from the structure */
        fd = ((TRDOS_FILE*)stream)->fd;
    } else {
        /* Standard files opened by fopen store the raw integer fd as the stream pointer itself */
        fd = (int)stream;
    }

    if (fd < 0) return 0;

    bytes_written = write(fd, ptr, size * count);
    if (bytes_written <= 0) return 0;
    return (bytes_written / size);
}
/* =========================================================================== */

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
