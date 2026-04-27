# TCC veya GCC ile objeleri oluþtur
tcc -m32 -c hello.c -o hello.o

# LD ile flat binary (PRG) oluþtur
ld -m elf_i386 -Ttext 0 --oformat binary crt0.o hello.o libc.a -o hello.prg
