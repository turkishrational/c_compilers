gcc -c tcc.c -o tcc.o -DTCC_TARGET_I386 -nostdlib -mabi=sysv -fpack-struct=4 -fno-ms-extensions -mpreferred-stack-boundary=2 -fno-stack-check -fno-stack-protector -ffunction-sections -fdata-sections
