@echo off
set CC=gcc
set AS=nasm
set AR=ar

echo [1/4] Derleniyor: crt0.asm...
%AS% -f elf32 crt0.asm -o crt0.o

echo [2/4] Derleniyor: sys.asm...
%AS% -f elf32 sys.asm -o sys.o

echo [3/4] Derleniyor: stdio.c...
%CC% -m32 -ffreestanding -nostdlib -c stdio.c -o stdio.o

echo [4/4] Kutuphane olusturuluyor: libc.a...
if exist libc.a del libc.a
%AR% rcs libc.a sys.o stdio.o

echo Islem Tamamlandi!
pause