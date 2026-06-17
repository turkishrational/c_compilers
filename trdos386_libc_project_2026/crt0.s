.intel_syntax noprefix
.global _start
.extern _main
.text

_start:
    jmp .L_START                    /* TRDOS 386 PRG execution entry jump */

    .ascii "CRT0"

.L_START:
    .extern _end                    /* from 'trdos386.ld' linker script ... end of bss */
    mov ebx, offset _end            /* set new u.break address to end of bss */
    mov eax, 17                     /* sysbreak ... kernel clears memory space */
    int 0x40                        /*  between the new (_end) and old u.break address */

    /* esp points to argc directly */
    pop eax                         /* eax = argc (value) */
    mov ebx, esp                    /* ebx = argv[0] pointer address (not value) */

    push ebx                        /* Pass argv pointer to main (Argument 2) */
    push eax                        /* Pass argc value to main (Argument 1) */

    call _main                      /* Execute main function of TCC/program */

    /* At this point, main's return/exit code is inside EAX register */

    add esp, 8                      /* Clean up the 2 arguments from stack */
    
    /* System exit routine via TRDOS 386 Kernel */
    mov ebx, eax                    /* Forward main's exit code to EBX for TRDOS */
    mov eax, 1                      /* TRDOS sys_exit system call number */
    int 0x40                        /* Call TRDOS 386 Kernel */

    .byte 0
    .ascii "C Compiler v1.0 for TRDOS 386"
    .byte 0
    .ascii "Erdogan Tan - 2026"
    .byte 0
