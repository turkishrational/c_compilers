How I compiled tcc v0.9.27 (2017) source code on Windows 7:

(as a win32 console program)

* Initially I used mysys-mingw32.

(After installing gnu autotools on mysys2 via pacman)

to use the "configure" script code.

(Because using make required the "config.h" and "config.mak" files.)

* Compilation succeeded with TDM-GCC-32 (gcc) via make.

* Compilation succeeded with LLVM-MINGW32 (clang-gcc) via make.

Note: I will not use the newly compiled tcc.exe on Windows...
My goal was/is not to recompile TCC for Windows.

I am simply preparing myself to adapt/port the tcc compiler 
to the TRDOS386/386DOS operating system.

This effort is a preliminary study.

Erdogan Tan - 14/04/2026