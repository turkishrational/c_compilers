#define NULL ((void*)0)

typedef unsigned int size_t_trdos;

/* GCC compiler maps C symbols to underscore prefix. */
// void _mingw_free(void *ptr) {}
// void *_mingw_realloc(void *ptr, size_t_trdos size) { return ptr; }

/* Fixed: Explicit implementation for missing ldexpl function */
double ldexpl(double x, int exp) { return x; }

/* Kalan standart stub yapýsý */
long strtol(const char *nptr, char **endptr, int base) { return 0; }
unsigned long strtoul(const char *nptr, char **endptr, int base) { return 0; }
long long strtoll(const char *nptr, char **endptr, int base) { return 0; }
unsigned long long strtoull(const char *nptr, char **endptr, int base) { return 0; }
char *getcwd(char *buf, size_t_trdos size) { if (buf && size > 0) buf = '\0'; return buf; }
char *_fullpath(char *absPath, const char *relPath, size_t_trdos maxLength) { return absPath; }
int unlink(const char *filename) { return 0; }
void qsort(void *base, unsigned int nitems, unsigned int size, int (*compar)(const void *, const void *)) {}
void *fdopen(int handle, const char *mode) { return (void*)handle; }

int _errno = 0;
char *strerror(int errnum) { return "TRDOS LIBC Error"; }
long long time(long long *tloc) { return 0; }
void *localtime(const long long *timer) { return NULL; }
int _setjmp(void *env) { return 0; }
void longjmp(void *env, int val) {}
void __stdcall InitializeCriticalSection(void *lpCriticalSection) {}
void __stdcall EnterCriticalSection(void *lpCriticalSection) {}
void __stdcall LeaveCriticalSection(void *lpCriticalSection) {}

#if 0 /* <- KOÞULSUZ OLARAK KATI MODDA DEVRE DIÞI BIRAKIYORUZ */
float strtof(const char *nptr, char **endptr) { return 0.0f; }
long double strtold(const char *nptr, char **endptr) { return 0.0L; }
#endif
double __strtod(const char *nptr, char **endptr) { return 0.0; }
unsigned long __stdcall GetModuleFileNameA(void* hModule, char* lpFilename, unsigned long nSize) { return 0; }
