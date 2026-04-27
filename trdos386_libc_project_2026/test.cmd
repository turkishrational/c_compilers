batch

@echo off
:: test.c -> test.o
gcc -m32 -ffreestanding -nostdlib -c test.c -o test.o

:: Linkleme (crt0.o her zaman ilk sýrada olmalý!)
ld -m elf_i386 -Ttext 0 --oformat binary crt0.o test.o libc.a -o test.prg

echo test.prg hazir!
