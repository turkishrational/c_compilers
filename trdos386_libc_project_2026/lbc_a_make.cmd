# 1. ASM dosyalarýný ELF32 formatýnda derle
nasm -f elf32 crt0.asm -o crt0.o
nasm -f elf32 sys_wrappers.asm -o sys_wrappers.o

# 2. Varsa C tabanlý yardýmcýlarý derle (printf vb.)
# i686-elf-gcc -m32 -ffreestanding -c stdio.c -o stdio.o

# 3. libc.a arþivini oluþtur (crt0.o hariç tutulur, o ayrý linklenir)
ar rcs libc.a sys_wrappers.o # + varsa diðer .o dosyalarý