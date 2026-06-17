.intel_syntax noprefix
.global _read
.text

_read:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]    /* Argument 1: fd */
    mov ecx, [ebp + 12]   /* Argument 2: buf */

    cmp ebx, 3
    jb .L_read_stdio      /* If fd < 3, route to sys_stdio */

    sub ebx, 3            /* Convert to Kernel FD */
    mov edx, [ebp + 16]   /* Argument 3: count */
    mov eax, 3            /* TRDOS 386 sys_read */
    int 0x40
    jnc .L_read_done
    jmp .L_read_fail

.L_read_fail:
    mov eax, -1
    pop ebx
    pop ebp
    ret

.L_read_stdio:
    cmp bl, 1             /* Check if fd is stdout(1) or stderr(2) */
    cmc
    jc .L_read_fail       /* If fd >= 1, abort with error (-1) */
    
    xor edx, edx          /* Clear character counter */

.L_read_stdio_next:
    mov eax, 46           /* TRDOS 386 sys_stdio (BL=0 for STDIN) */
    int 0x40
    jc .L_read_ok

    mov [ecx], al
    and al, al
    jz .L_read_ok
    
    inc edx
    cmp edx, [ebp + 16]   /* Check against requested count limit */
    jnb .L_read_ok
    
    inc ecx
    cmp al, 27            /* ESC check */
    je .L_read_ok
    cmp al, 13            /* Enter check */
    je .L_read_ok
    jmp .L_read_stdio_next

.L_read_ok:
    mov eax, edx          /* Return total read count */

.L_read_done:
    pop ebx
    pop ebp
    ret

