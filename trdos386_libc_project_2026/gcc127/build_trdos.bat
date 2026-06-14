:: -Ttext 0x0 ile ORG 0 yapýlýr ve linker otomatik olarak _end deðerini belirler

ld -o GCC127.PRG crt0.o compiler.o open.o close.o read.o write.o sbrk.o --oformat binary -Ttext 0x0