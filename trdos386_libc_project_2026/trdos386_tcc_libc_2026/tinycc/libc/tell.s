.intel_syntax noprefix
.global _tell
.text

_tell:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]    /* Parameter 1: LIBC File Descriptor (fd) */
    sub ebx, 3            /* Convert LIBC FD (3-12) to Kernel FD (0-9) */
    jb .L_tell_err        /* If fd < 3 (stdin/out/err), it's invalid for tell */

    mov edx, 1            /* EDX = 1 -> TRDOS kernel: return current file offset */
    mov ecx, 0            /* ECX = 0 -> Ignored per TRDOS kernel design */
    mov eax, 20           /* TRDOS 386 sys_tell system call number */
    int 0x40              /* Call TRDOS 386 Kernel Interrupt */
    jc .L_tell_err        /* If carry flag set, kernel returned an error */
    
    pop ebx
    pop ebp
    ret

.L_tell_err:
    mov eax, -1           /* Return -1 on failure */
    pop ebx
    pop ebp
    ret
