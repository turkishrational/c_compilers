.intel_syntax noprefix
.global _lseek
.text

_lseek:
    push ebp
    mov ebp, esp
    push ebx              /* Protect ebx per cdecl rule */

    mov ebx, [ebp + 8]    /* Parameter 1: fd (Fixed position via ebp) */
    mov ecx, [ebp + 12]   /* Parameter 2: offset */
    mov edx, [ebp + 16]   /* Parameter 3: whence */

    sub ebx, 3            /* C-FD (3,4..) -> TRDOS-FD (0,1..) */
    jb .L_lseek_err       /* If fd < 3, it's an invalid regular file descriptor */

    mov eax, 19           /* sys_lseek system call number */
    int 0x40              /* TRDOS 386 Kernel Interrupt */
    jc .L_lseek_err       /* If carry flag set, kernel returned an error */
    
    pop ebx
    pop ebp
    ret

.L_lseek_err:
    mov eax, -1           /* Return -1 on failure */
    pop ebx
    pop ebp
    ret
