@echo off
echo ===================================================================
echo [TDM-GCC-32] INITIAL (MODIFIED) TCC.C DERLEME ISTASYONU
echo ===================================================================

:: Ana tek gövde TCC kaynak kodunu derliyoruz
gcc -c tcc.c -o tcc.o -DTCC_TARGET_I386 -nostdlib -mabi=sysv -fpack-struct=4 -mno-ms-bitfields -fno-ms-extensions -mpreferred-stack-boundary=2 -fno-stack-check -fno-stack-protector -ffunction-sections -fdata-sections

if %errorlevel% equ 0 (
    echo [BASARILI] Initial tcc.o nesne dosyasi sorunsuz uretildi!
) else (
    echo [HATA] Derleme esnasinda bir uyuþmazlik yasandi.
    exit /b 1
)