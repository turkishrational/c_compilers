.intel_syntax noprefix
.global _start
.extern _main
.text

_start:
    jmp .L_START

    .ascii "CRT0"

.L_START:
    .extern _end

    mov ebx, offset _end            /* ebx = Nihai BSS sonu adresi */
    add ebx, 3
    and ebx, 0xFFFFFFFC             /* 4-Byte Dword Hizalama */
    mov eax, 17                     /* EAX = 17 (sys_break) */
    int 0x40                        /* TRDOS Kernel Resmi Bildirimi */

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

    .byte 0
    .ascii "C Compiler v1.0 for TRDOS 386"
    .byte 0
    .ascii "Erdogan Tan - 2026"
    .byte 0
