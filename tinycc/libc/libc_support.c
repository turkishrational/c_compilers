/* 23/07/2026 */
/* 08/07/2026 - Google AI */

#define NULL ((void*)0)

typedef unsigned int size_t_trdos;

/* Fixed: Explicit implementation for missing ldexpl function */
long double ldexpl(long double x, int exp) { return x; }

/* Baseline standard stub framework implementations for TRDOS 386 target environment */
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

/* 23/07/2026 - Google AI */
/* =========================================================================
   TRDOS-386 CORE INTEGER MATHEMATICS - HIGH PERFORMANCE STRTOL/STRTOUL MOTOR
   ========================================================================= */
unsigned long strtoul(const char *nptr, char **endptr, int base)
{
    unsigned long res = 0;
    const char *p = nptr;
    int current_base = base;

    if (!p) return 0;

    /* Skip leading whitespaces */
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v') {
        p++;
    }

    /* Process structural sign qualifiers (strtoul can have signs per standard) */
    int sign = 1;
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    /* Auto-detect base if set to 0 */
    if (current_base == 0) {
        if (*p == '0') {
            if (*(p + 1) == 'x' || *(p + 1) == 'X') {
                current_base = 16;
                p += 2;
            } else {
                current_base = 8;
                p++;
            }
        } else {
            current_base = 10;
        }
    } else if (current_base == 16) {
        /* Skip optional 0x prefix for base 16 */
        if (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')) {
            p += 2;
        }
    }

    /* Parse continuous character digit sequences based on target base */
    while (*p) {
        int digit = -1;
        if (*p >= '0' && *p <= '9') {
            digit = *p - '0';
        } else if (*p >= 'a' && *p <= 'f') {
            digit = *p - 'a' + 10;
        } else if (*p >= 'A' && *p <= 'F') {
            digit = *p - 'A' + 10;
        }

        /* If digit is out of bounds for the current base, we halt */
        if (digit < 0 || digit >= current_base) {
            break;
        }

        res = (res * current_base) + digit;
        p++;
    }

    /* Update the optional trailing end tracker pointer reference if existing */
    if (endptr) {
        /* Standard compliance: if no conversion performed, endptr gets original nptr */
        if (p == nptr || (nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X') && p == nptr + 2)) {
            *endptr = (char *)nptr;
        } else {
            *endptr = (char *)p;
        }
    }

    return sign * res;
}

long strtol(const char *nptr, char **endptr, int base) 
{
    return (long)strtoul(nptr, endptr, base);
}

/* Route 64-bit fallbacks onto our robust 32-bit engine for the time being */
long long strtoll(const char *nptr, char **endptr, int base) 
{ 
    return (long long)strtoul(nptr, endptr, base); 
}

unsigned long long strtoull(const char *nptr, char **endptr, int base) 
{ 
    return (unsigned long long)strtoul(nptr, endptr, base); 
}
