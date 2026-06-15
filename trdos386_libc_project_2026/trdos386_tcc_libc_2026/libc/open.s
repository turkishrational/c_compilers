.intel_syntax noprefix
.global _open
.text

_open:
    push ebp
    mov ebp, esp
    push ebx              /* Protect EBX per cdecl rule */

    mov ebx, [ebp + 8]    /* Argument 1: pathname */
    mov ecx, [ebp + 12]   /* Argument 2: flags */
    mov edx, [ebp + 16]   /* Argument 3: mode */

    mov eax, 5            /* TRDOS 386 sys_open system call number */
    int 0x40              /* Call TRDOS 386 Kernel */
    jc .L_open_fail

    add eax, 3            /* Convert TRDOS FD (0-9) to LIBC FD (3-12) */
    jmp .L_open_done

.L_open_fail:
    mov eax, -1           /* Return standard C error */

.L_open_done:
    pop ebx
    pop ebp
    ret

