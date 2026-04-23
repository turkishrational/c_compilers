How I compiled tcc v0.9.27 (2017) win32 source code on Windows 7:

(as a win32 console program)

* I ran the "build-tcc.bat" batch file in the win32 sub directory. 
("i386-win32-tcc.exe" and "tcc.exe" executable/PE files were created).

* This process was successful in both "LLVM-MINGW" (clang) 
  and "TDM-GCC-32" (gcc) compiler suites.

* Compilations with LLVM-MINGW (clang) have smaller file sizes.

Erdogan Tan - 15/04/2026