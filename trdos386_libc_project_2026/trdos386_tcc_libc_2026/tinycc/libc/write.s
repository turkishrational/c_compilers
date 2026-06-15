.intel_syntax noprefix
.global _write
.text

_write:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]    /* Argument 1: fd */
    mov ecx, [ebp + 12]   /* Argument 2: buf */
    mov edx, [ebp + 16]   /* Argument 3: count */

    cmp ebx, 3
    jb .L_write_stdio     /* If fd < 3, route to console write */

    sub ebx, 3            /* Convert to Kernel FD */
    mov eax, 4            /* TRDOS 386 sys_write */
    int 0x40
    jnc .L_write_done
    jmp .L_write_fail

.L_write_fail:
    mov eax, -1
    pop ebx
    pop ebp
    ret

.L_write_stdio:
    /* For writing, fd=0 (stdin) is invalid, fd=1 or fd=2 are valid */
    cmp ebx, 0
    je .L_write_fail

    /* TRDOS 386 sys_stdio call for console output */
    /* BL = 1 (stdout) or 2 (stderr) which matches ebx value exactly */
    mov eax, 46           /* TRDOS 386 sys_stdio print string function */
    int 0x40
    jnc .L_write_done
    jmp .L_write_fail

.L_write_done:
    pop ebx
    pop ebp
    ret
