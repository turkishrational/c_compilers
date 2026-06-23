@echo off
echo ===================================================================
echo [TRDOS-386 NATIVE TCC - ADVANCED OBJECT COMPILATION]
echo ===================================================================

gcc -c tcc.c -o tcc.o -DTCC_TARGET_I386 -nostdlib -mpreferred-stack-boundary=2 -ffunction-sections -fdata-sections

echo tcc.o basariyla zýrhlandý!
