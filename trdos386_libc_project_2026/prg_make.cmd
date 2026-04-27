# hello.c'yi derle ve libc ile baðla
i686-elf-ld -m elf_i386 -Ttext 0 --oformat binary \
    crt0.o hello.o libc.a -o hello.prg