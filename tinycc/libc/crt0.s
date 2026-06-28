.intel_syntax noprefix
.global _start
.extern _main
.extern _itoa
.text

_start:
    jmp .L_START

    .ascii "CRT0"

.L_START:
    .extern _bss_start
    .extern _end

    /* =========================================================================
       ?? [SAF REPMOV/STOSD BSS TEMÝZLEME ZIRHI]
       ========================================================================= */
    mov edi, offset _bss_start      /* EDI = BSS Baþlangýcý (0x44000) */
    mov ecx, offset _end            /* ECX = BSS Sonu (0x5A000) */
    sub ecx, edi                    /* ECX = BSS Toplam Byte Boyutu */
    
    shr ecx, 2                      /* Byte sayýsýný DWORD (4-byte) sayýsýna böl */
    xor eax, eax                    /* EAX = 0 (Sýfýrlama maskesi) */
    rep stosd                       /* Tüm BSS alanýný milimetrik olarak sýfýrla ch çak! */

    /* =========================================================================
       ?? [ANA KODA GEÇÝÞ]
       ========================================================================= */
    pop eax                         /* eax = argc */
    mov ebx, esp                    /* ebx = argv pointer */
    push ebx
    push eax
    call _main                      /* TCC Ana Derleyici Motoru */
    
    add esp, 8
    mov ebx, eax
    mov eax, 1                      /* sys_exit */
    int 0x40

/* ?? SAF .TEXT ÝÇI ADRES VE METÝN ALANI */
msg_bss_start:
    .byte 0x0D, 0x0A
    .ascii "[* TRDOS DIAG *] Linker _bss_start  : \0"
msg_bss_end:
    .byte 0x0D, 0x0A
    .ascii "[* TRDOS DIAG *] Linker _end Address: \0"
msg_newline:
    .byte 0x0D, 0x0A, 0

buf_bss_start:
    .space 32, 0
buf_bss_end:
    .space 32, 0

    .byte 0
    .ascii "C Compiler v1.0 for TRDOS 386"
    .byte 0
    .ascii "Erdogan Tan - 2026"
    .byte 0
