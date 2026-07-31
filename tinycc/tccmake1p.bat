@echo off
c:\tdm-gcc-32\bin\ld.exe -T trdos386.ld -static --gc-sections -o TCC.tmp libc/crt0.o tcc.o --start-group libc/memset.o libc/memcpy.o libc/strlen.o libc/strcmp.o libc/strcpy.o libc/memcmp.o libc/strchr.o libc/strrchr.o libc/memmove.o libc/strncmp.o libc/chkstk.o libc/gcc_stubs.o libc/strcat.o libc/atoi.o libc/strpbrk.o libc/libc_core.o libc/libc_support.o libc/lseek.o libc/tell.o libc/open.o libc/close.o libc/read.o libc/write.o libc/math64.o libc/mingw_hack.o libc/libc_printf_bridge.o libc/libc_itoa.o libc/malloc.o libc/fstat.o libc/strstr.o libc/freeopen.o libc/ldexp.o libc/win32_stubs.o --end-group

if %errorlevel% neq 0 (
    echo [HATA] Linker asamasi basarisiz.
    exit /b 1
)

c:\tdm-gcc-32\bin\objcopy.exe -O binary TCC.tmp TCC.PRG
del TCC.tmp
echo TCC.PRG 0.9.23 CEKIRDEKLE %100 FLAT OLARAK DOGDU!