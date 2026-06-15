How I compiled tcc v0.9.28rc (2026) win32 source code on Windows 7:

(as a win32 console program)

AT FIRST:

* I ran the "build-tcc.bat" batch file in the win32 sub directory.
("tcc.exe" executable/PE files were created). (+"libtcc.dll")

* This process was successful in both "LLVM-MINGW" (clang)
  and "TDM-GCC-32" (gcc) compiler suites.

THEN:

* I ran the "make.bat" (mingw32-make.exe) in tcc source code directory.
("tcc.exe" and "c2str.exe" executable/PE files were created).
(+"libtcc.dll")

*** These two processes were successful in both "LLVM-MINGW" (clang)
    and "TDM-GCC-32" (gcc) compiler suites.

NOTE:
* Compilations with LLVM-MINGW (clang) have smaller file sizes.

Erdogan Tan - 15/04/2026