:: 1. ADIM: Tüm nesne dosyalarýný PE formatýnda tek bir ara çýktý (TCC.tmp) olarak baðlayýn

c:\tdm-gcc-32\bin\ld.exe -T trdos386.ld -o TCC.tmp tcc.o libc/memset.o libc/memcpy.o libc/strlen.o libc/strcmp.o libc/strcpy.o libc/memcmp.o libc/strchr.o libc/strrchr.o libc/memmove.o libc/strncmp.o libc/chkstk.o libc/gcc_stubs.o libc/strcat.o libc/atoi.o libc/strpbrk.o libc/libc_core.o libc/libc_support.o libc/lseek.o libc/tell.o libc/open.o libc/close.o libc/read.o libc/write.o libc/sbrk.o libc/crt0.o libc/math64.o libc/mingw_hack.o


:: 2. ADIM: Linker Script'i güncelleyeceðimiz için (aþaðýya göz atýn) oluþan PE dosyasýný Flat Binary'e dönüþtürün

c:\tdm-gcc-32\bin\objcopy.exe -O binary TCC.tmp TCC.PRG


:: 3. ADIM: Ara dosyayý temizleyin

del TCC.tmp