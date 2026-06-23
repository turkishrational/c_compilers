@echo off
echo ===================================================================
echo [TRDOS-386 NATIVE TCC NATIVE FLAT LINKING]
echo ===================================================================

:: Tüm nesne dosyalarýný doðrudan saf FLAT BINARY (TCC.PRG) olarak baðlayýn
c:\tdm-gcc-32\bin\ld.exe -T trdos386.ld --oformat binary -o TCC.PRG tcc.o --start-group libc/memset.o libc/memcpy.o libc/strlen.o libc/strcmp.o libc/strcpy.o libc/memcmp.o libc/strchr.o libc/strrchr.o libc/memmove.o libc/strncmp.o libc/chkstk.o libc/gcc_stubs.o libc/strat.o libc/atoi.o libc/strpbrk.o libc/libc_core.o libc/libc_support.o libc/lseek.o libc/tell.o libc/open.o libc/close.o libc/read.o libc/write.o libc/sbrk.o libc/crt0.o libc/math64.o libc/mingw_hack.o libc/libc_printf_bridge.o libc/libc_itoa.o --start-group

echo TCC.PRG basariyla ve dogrudan saf binary olarak uretildi!