@echo off
echo ===================================================================
echo [TRDOS-386 MASTER OBJECT LINKING & FLAT GENERATION]
echo ===================================================================

:: 1. Adým: crt0.o'yu listenin en baþýna koyarak geçici tam PE dosyasýný baðla
c:\tdm-gcc-32\bin\ld.exe -T trdos386.ld -o TCC.tmp libc/crt0.o tcc.o --start-group libc/memset.o libc/memcpy.o libc/strlen.o libc/strcmp.o libc/strcpy.o libc/memcmp.o libc/strchr.o libc/strrchr.o libc/memmove.o libc/strncmp.o libc/chkstk.o libc/gcc_stubs.o libc/strcat.o libc/atoi.o libc/strpbrk.o libc/libc_core.o libc/libc_support.o libc/lseek.o libc/tell.o libc/open.o libc/close.o libc/read.o libc/write.o libc/sbrk.o libc/math64.o libc/mingw_hack.o libc/libc_printf_bridge.o libc/libc_itoa.o --end-group

:: 2. Adým: Oluþan geçici PE imajýný flat ikiliye (TCC.PRG) dönüþtür
c:\tdm-gcc-32\bin\objcopy.exe -O binary TCC.tmp TCC.PRG

:: 3. Adým: Temizlik
del TCC.tmp

echo TCC.PRG milimetrik olarak en bastan hizalandi!