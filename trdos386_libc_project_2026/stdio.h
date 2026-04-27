/* trdos_stdio.h */
#define stdin  0
#define stdout 1
#define stderr 2

int putc(int c);
int getc(void);
int open(const char *path, int flags);
int read(int fd, void *buf, int n);
int write(int fd, const void *buf, int n);