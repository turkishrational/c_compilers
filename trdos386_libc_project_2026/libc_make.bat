nasm -f elf32 read.s -o read.o
nasm -f elf32 write.s -o write.o
nasm -f elf32 lseek.s -o lseek.o
nasm -f elf32 errno.s -o errno.o
# ... diğerleri ...
ar rcs libc.a read.o write.o lseek.o errno.o