# TRDOS 386 Makefile
CC = gcc
AS = nasm
AR = ar
LD = ld

CFLAGS = -m32 -ffreestanding -nostdlib -fno-stack-protector -O2 -ffunction-sections -fdata-sections 
LDFLAGS = -m elf_i386 -Ttext 0 -n --oformat binary --gc-sections

all: libc.a crt0.o

crt0.o: crt0.asm
	$(AS) -f elf32 crt0.asm -o crt0.o

sys.o: sys.asm
	$(AS) -f elf32 sys.asm -o sys.o

stdio.o: stdio.c
	$(CC) $(CFLAGS) -c stdio.c -o stdio.o

libc.a: sys.o stdio.o
	$(AR) rcs libc.a sys.o stdio.o

clean:
	del *.o *.a *.prg