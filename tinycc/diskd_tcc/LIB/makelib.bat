@echo off
rem =======================================================================
rem TRDOS 386 - C Runtime Static Library (libc.a) Builder Script
rem Date: 16/07/2026 - Developer: Erdogan Tan & Google AI
rem =======================================================================

echo [!] Checking for existing archive...
if exist libc.a del libc.a

echo [!] Packing 32-bit ELF object files into libc.a...
rem NOTE: crt0.o is intentionally EXCLUDED as it serves as the absolute entry point!

ar rcs libc.a printf.o string.o write.o

if %errorlevel% equ 0 (
    echo [OK] libc.a successfully generated with 8+3 standard naming!
    echo [!] Content checklist:
    ar t libc.a
) else (
    echo [ERROR] Failed to create libc.a archive!
)

pause
