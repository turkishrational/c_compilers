tcc -m32 -c stdio.c -o stdio.o
ar rcs libc.a stdio.o sys_wrappers.o