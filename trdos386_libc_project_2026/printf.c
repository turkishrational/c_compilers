/* 
   TRDOS 386 - Minimal printf (v1.0)
   - %s, %c, %d, %x desteði
   - Baðýmsýz yapý (No va_list/stdarg.h)
   - Doðrudan putc (EAX=46/47) kullanýr
*/

extern void putc(int c); // Daha önce yazdýðýmýz ASM sarmalayýcýsý

// String basma
static void print_str(const char *s) {
    if (!s) s = "(null)";
    while (*s) putc(*s++);
}

// Tam sayý ve Hex basma
static void print_int(int n, int base) {
    char buf[32];
    int i = 0;
    unsigned int num = (unsigned int)n;

    if (n == 0) {
        putc('0');
        return;
    }

    // Negatif kontrolü (sadece 10'luk tabanda)
    if (base == 10 && n < 0) {
        putc('-');
        num = (unsigned int)(-n);
    }

    const char *digits = "0123456789abcdef";
    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }

    // Buffer'dan ters sýrada bas
    while (i > 0) {
        putc(buf[--i]);
    }
}

// Ana printf fonksiyonu
void printf(const char *fmt, ...) {
    // Stack üzerindeki argümanlara doðrudan eriþim
    char **arg = (char **)&fmt;
    arg++; // fmt'den sonra gelen ilk argümana geç

    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++; // '%' iþaretini geç
            switch (*fmt) {
                case 's':
                    print_str(*arg++);
                    break;
                case 'c':
                    // char yýðýnda int olarak terfi ettirilir
                    putc((int)(size_t)*arg++);
                    break;
                case 'd':
                    print_int((int)(size_t)*arg++, 10);
                    break;
                case 'x':
                    print_int((int)(size_t)*arg++, 16);
                    break;
                case '%':
                    putc('%');
                    break;
                default:
                    putc('%');
                    putc(*fmt);
                    break;
            }
        } else {
            putc(*fmt);
        }
        fmt++;
    }
}