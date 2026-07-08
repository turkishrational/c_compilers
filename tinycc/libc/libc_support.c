/* 8/7/2026 - Google AI */

#define NULL ((void*)0)

typedef unsigned int size_t_trdos;

/* Fixed: Explicit implementation for missing ldexpl function */
long double ldexpl(long double x, int exp) { return x; }

/* Baseline standard stub framework implementations for TRDOS 386 target environment */
long strtol(const char *nptr, char **endptr, int base) { return 0; }
unsigned long strtoul(const char *nptr, char **endptr, int base) { return 0; }
long long strtoll(const char *nptr, char **endptr, int base) { return 0; }
unsigned long long strtoull(const char *nptr, char **endptr, int base) { return 0; }
char *getcwd(char *buf, size_t_trdos size) { if (buf && size > 0) buf[0] = '\0'; return buf; }
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

/* =========================================================================
   TRDOS-386 CORE MATHEMATICS - HIGH PERFORMANCE LIGHTWEIGHT STRTOD MOTOR
   ========================================================================= */
double __strtod(const char *nptr, char **endptr)
{
    double res = 0.0;
    double factor = 1.0;
    double divisor = 10.0;
    int sign = 1;
    int decimal_seen = 0;
    const char *p = nptr;

    if (!p) return 0.0;

    /* Skip leading whitespaces */
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v') {
        p++;
    }

    /* Process structural sign qualifiers */
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    /* Parse continuous character digit sequences */
    while (*p) {
        if (*p >= '0' && *p <= '9') {
            if (!decimal_seen) {
                /* Integer part processing loop */
                res = (res * 10.0) + (*p - '0');
            } else {
                /* Floating point decimal part processing loop */
                res = res + ((*p - '0') / divisor);
                divisor *= 10.0;
            }
            p++;
        } else if (*p == '.' && !decimal_seen) {
            decimal_seen = 1;
            p++;
        } else {
            /* Stop parsing at the first invalid non-numeric token milestone */
            break;
        }
    }

    /* Update the optional trailing end tracker pointer reference if existing */
    if (endptr) {
        *endptr = (char *)p;
    }

    return sign * res;
}

/* =========================================================================
   UNIFIED FLOAT ROUTING BRIDGES MAPPED DIRECTLY UNTO BASIC MOTORS
   ========================================================================= */
float strtof(const char *nptr, char **endptr) 
{ 
    return (float)__strtod(nptr, endptr); 
}

long double strtold(const char *nptr, char **endptr) 
{ 
    return (long double)__strtod(nptr, endptr); 
}

double strtod(const char *nptr, char **endptr) 
{ 
    return __strtod(nptr, endptr); 
}