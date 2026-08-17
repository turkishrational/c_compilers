## This is the repository for the Tiny C Compiler ported to the TRDOS 386 operating system.

Project description: Porting the Tiny C Compiler (TCC)—originally developed by Fabrice Bellard—to the TRDOS 386 (and 386 DOS) operating system.

**TCC version ported/being ported: 0.9.18–0.9.27**

Developer: Erdoğan Tan (with the assistance of Google AI)
(The code required for the port was prepared and refined by Google AI based on Erdoğan Tan's TRDOS 386 kernel, methods, and modification specifications.)

```
Host: Windows 7/11 (C:\TDM-GCC-32\tinycc)
Compiler: TDM-GCC-32, mingw32 (gcc)
Target: TRDOS 386 (32-bit) Flat Binary File, TCC.PRG
Test Environment: QEMU (Windows 7/11)
```

**FEATURES:**
   > * TCC.PRG working directory: Any (location where TCC.PRG is situated)
   > * Library directory: ./lib (under the TCC.PRG working directory)
   > * Include directory: ./include (under the TCC.PRG working directory)
   > * Input file formats: TCC 0.9.27-compatible C ('.c'), assembly files with GAS syntax ('.s'), and ELF object files. 
   > * Output file formats: TRDOS 386 (386 DOS) Flat Binary files (with .PRG extension) and ELF32 object files (via the -c option).
   > * Startup code that calls the C code (main function): CRT0 (crt0.o).
   > * Location of crt0.o: ./lib
   > * Default library file: libc.a (in the lib directory).
   > * Library archive format: Archive of ELF32 object files.
   > * Library usage method: Only the necessary object files are linked with the target binary.

The library files used for the port were created in PE/COFF format within the TDM-GCC-32 MinGW environment (collected in the C:\TDM-GCC-32\tinycc\libc directory).

The files prepared as libraries and object files for TCC.PRG (default) are collected in the C:\TDM-GCC-32\tinycc\tcclibc directory; additionally, they are located in the LIB directory under TCC.PRG's working directory for use at runtime.

TCC.PRG 0.9.18 (While the ported version is TCC 0.9.18, it is partially compatible with 0.9.27; for instance, the ELF32 object format is based on 0.9.27).

Note: The process of upgrading TCC.PRG to TCC version 0.9.27 is ongoing (as of July 25, 2026).

_Erdogan Tan - July 25, 2026_

