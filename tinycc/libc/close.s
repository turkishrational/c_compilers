.intel_syntax noprefix
.global _close
.text

_close:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]    /* Argument 1: LIBC File Descriptor (fd) */
    sub ebx, 3            /* Convert LIBC FD (3-12) to Kernel FD (0-9) */
    jb .L_close_fail      /* If fd < 3, it's a stdio handle, ignore or fail */

    mov eax, 6            /* TRDOS 386 sys_close system call number */
    int 0x40
    jc .L_close_fail
    
    xor eax, eax          /* Return 0 on success */
    jmp .L_close_done

.L_close_fail:
    mov eax, -1

.L_close_done:
    pop ebx
    pop ebp
    ret
