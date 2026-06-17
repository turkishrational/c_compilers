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

    /* -----------------------------------------------------------------------
       TRDOS 386 Native I/O & Flat Stream Array Resolver for FREAD (Google AI)
       ----------------------------------------------------------------------- */
    /* 1. Check if the stream pointer belongs to our flat 4-byte _iob array */
    if ((char*)stream >= (char*)_iob && (char*)stream < ((char*)_iob + 12)) {
        
        int stream_index = ((char*)stream - (char*)_iob) / 4;
        
        /* Map flat array elements directly to TRDOS sysstdio handles (0, 1, 2) */
        if (stream_index == 0)      fd = 0; /* stdin  -> sysstdio */
        else if (stream_index == 1) fd = 1; /* stdout -> sysstdio */
        else if (stream_index == 2) fd = 2; /* stderr -> sysstdio */
        else                        fd = -1;
        
    } else {
        /* 2. Process file streams or explicit file descriptors */
        /* If it's a legacy or raw small integer descriptor within the disk file range (3-12) */
        if ((unsigned int)stream >= 3 && (unsigned int)stream <= 12) {
            fd = (int)stream;
        } else {
            /* If it's a dynamic structure address returned by a real fopen/open */
            fd = ((TRDOS_FILE*)stream)->fd;
        }
    }
    /* ----------------------------------------------------------------------- */

    if (fd < 0) return 0;

    bytes_read = read(fd, ptr, size * count);
    if (bytes_read <= 0) return 0;
    return (bytes_read / size);
}

unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, void *stream) {
    int fd;
    int bytes_written;

    /* -----------------------------------------------------------------------
       HEX DEBUGGER INJECTION: Print input parameters directly via sysmsg (EAX=35)
       ----------------------------------------------------------------------- */
    {
        unsigned int debug_stream = (unsigned int)stream;
        char dbg_msg[] = "FWRITE DEBUG: stream=0x00000000\r\n";
        char hex_chars[] = "0123456789ABCDEF";
        
        /* Convert stream pointer address to hexadecimal text format */
        dbg_msg[21] = hex_chars[(debug_stream >> 28) & 0x0F];
        dbg_msg[22] = hex_chars[(debug_stream >> 24) & 0x0F];
        dbg_msg[23] = hex_chars[(debug_stream >> 20) & 0x0F];
        dbg_msg[24] = hex_chars[(debug_stream >> 16) & 0x0F];
        dbg_msg[25] = hex_chars[(debug_stream >> 12) & 0x0F];
        dbg_msg[26] = hex_chars[(debug_stream >> 8) & 0x0F];
        dbg_msg[27] = hex_chars[(debug_stream >> 4) & 0x0F];
        dbg_msg[28] = hex_chars[debug_stream & 0x0F];

        /* Direct TRDOS 386 sysmsg call (EAX=35) - Bypasses all libc logic */
        __asm__ __volatile__ (
            "movl %0, %%ebx\n\t"        /* EBX = Message pointer address */
            "movl $33, %%ecx\n\t"       /* ECX = Message length (33 bytes) */
            "movb $0x0E, %%dl\n\t"      /* DL  = Color attribute (0x0E = Yellow) */
            "movl $35, %%eax\n\t"       /* EAX = 35 (TRDOS 386 sysmsg) */
            "int $0x40\n\t"
            :
            : "g" (dbg_msg)
            : "eax", "ebx", "ecx", "edx"
        );
    }

    /* -----------------------------------------------------------------------
       TRDOS 386 Native I/O & Flat Stream Array Resolver for FWRITE (Google AI)
       ----------------------------------------------------------------------- */
    if (stream == NULL) return 0;

    /* 1. Check if the stream pointer belongs to our flat 4-byte _iob array */
    if ((char*)stream >= (char*)_iob && (char*)stream < ((char*)_iob + 12)) {
        
        int stream_index = ((char*)stream - (char*)_iob) / 4;
        
        /* Map flat array elements directly to TRDOS sysstdio handles (0, 1, 2) */
        if (stream_index == 0)      fd = 0; /* stdin  -> sysstdio */
        else if (stream_index == 1) fd = 1; /* stdout -> sysstdio */
        else if (stream_index == 2) fd = 2; /* stderr -> sysstdio */
        else                        fd = -1;
        
    } else {
        /* 2. Process file streams or explicit file descriptors */
        /* If it's a legacy or raw small integer descriptor within the disk file range (3-12) */
        if ((unsigned int)stream >= 3 && (unsigned int)stream <= 12) {
            fd = (int)stream;
        } else {
            /* If it's a dynamic structure address returned by a real fopen/open */
            fd = ((TRDOS_FILE*)stream)->fd;
        }
    }
    /* ----------------------------------------------------------------------- */

    /* -----------------------------------------------------------------------
       HEX DEBUGGER INJECTION 2: Print the finalized FD variable value
       ----------------------------------------------------------------------- */
    {
        unsigned int debug_fd = (unsigned int)fd;
        char dbg_msg2[] = "FWRITE DEBUG: final fd=0x00000000\r\n";
        char hex_chars[] = "0123456789ABCDEF";

        dbg_msg2[25] = hex_chars[(debug_fd >> 28) & 0x0F];
        dbg_msg2[26] = hex_chars[(debug_fd >> 24) & 0x0F];
        dbg_msg2[27] = hex_chars[(debug_fd >> 20) & 0x0F];
        dbg_msg2[28] = hex_chars[(debug_fd >> 16) & 0x0F];
        dbg_msg2[29] = hex_chars[(debug_fd >> 12) & 0x0F];
        dbg_msg2[30] = hex_chars[(debug_fd >> 8) & 0x0F];
        dbg_msg2[31] = hex_chars[(debug_fd >> 4) & 0x0F];
        dbg_msg2[32] = hex_chars[debug_fd & 0x0F];

        __asm__ __volatile__ (
            "movl %0, %%ebx\n\t"
            "movl $37, %%ecx\n\t"       /* ECX = Message length (37 bytes) */
            "movb $0x0B, %%dl\n\t"      /* DL  = Color attribute (0x0B = Light Cyan) */
            "movl $35, %%eax\n\t"
            "int $0x40\n\t"
            :
            : "g" (dbg_msg2)
            : "eax", "ebx", "ecx", "edx"
        );
    }
    /* ----------------------------------------------------------------------- */

    if (fd < 0) return 0;

    /* 17/6/2026 - Google AI */
    /* ===========================================================================
       TRDOS 386 Native I/O & Small-C Inspired CRLF Engine (Google AI & User 2026)
       =========================================================================== */
    if (fd >= 0 && fd <= 2) {
        /* Screen / Console Output Stream Filter */
        const char *buf = (const char *)ptr;
        unsigned int total_bytes = size * count;
        unsigned int i;
        int bytes_written = 0;
        static char prev_char = 0; /* Persistent state tracker across calls */
        char cr = '\r';

        for (i = 0; i < total_bytes; i++) {
            char current_char = buf[i];

            /* If current is LF ('\n'), check if previous char was CR ('\r') */
            if (current_char == '\n') {
                if (prev_char != '\r') {
                    /* Inject missing Carriage Return directly via TRDOS write */
                    write(fd, &cr, 1);
                }
            }

            /* Write the actual character */
            if (write(fd, &current_char, 1) == 1) {
                bytes_written++;
            }

            /* Save the state for the next cycle or next fwrite call */
            prev_char = current_char;
        }

        return (bytes_written / size);
    } else {
        /* -----------------------------------------------------------------------
           Standard Disk File Access (FD >= 3) - Completely Raw & Binary Safe
           ----------------------------------------------------------------------- */
        bytes_written = write(fd, ptr, size * count);
        if (bytes_written <= 0) return 0;
        return (bytes_written / size);
    }
    /* =========================================================================== */
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
