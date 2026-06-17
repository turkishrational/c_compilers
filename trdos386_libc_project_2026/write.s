.intel_syntax noprefix
.global _write
.text

_write:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi

    mov ebx, [ebp + 8]    /* Argument 1: fd (File Descriptor) */
    mov esi, [ebp + 12]   /* Argument 2: buf (Buffer pointer) */
    mov edi, [ebp + 16]   /* Argument 3: count (Byte count) */

    cmp ebx, 3
    jb .L_write_stdio     /* If fd < 3 (0, 1, 2), route to console sys_stdio */

    /* -----------------------------------------------------------------------
       Standard File Write (sys_write)
       ----------------------------------------------------------------------- */
    sub ebx, 3            /* Convert C-FD to TRDOS-FD */
    mov ecx, esi          /* ECX = buffer address */
    mov edx, edi          /* EDX = byte count */
    mov eax, 4            /* EAX = 4 (TRDOS 386 sys_write) */
    int 0x40
    jnc .L_write_done
    jmp .L_write_fail

    /* -----------------------------------------------------------------------
       Console Output Write Loop (sys_stdio - Character by Character)
       ----------------------------------------------------------------------- */
.L_write_stdio:
    cmp ebx, 0
    je .L_write_fail      /* Writing to stdin (fd=0) is invalid */

    /* Map C-FD to TRDOS sys_stdio BL codes: fd=1 (stdout) -> BL=2, fd=2 (stderr) -> BL=3 */
    cmp ebx, 2
    je .L_set_stderr
    mov bl, 2             /* BL = 2 (stdout onto screen with redirection support) */
    jmp .L_init_loop
.L_set_stderr:
    mov bl, 3             /* BL = 3 (stderr onto screen without redirection) */

.L_init_loop:
    xor edx, edx          /* EDX = Character counter (bytes written) */

.L_stdio_loop_next:
    cmp edx, edi          /* Check if counter (EDX) has reached count (EDI) */
    jnb .L_stdio_loop_ok

    mov cl, [esi]         /* CL = ASCII character code to be printed */
    test cl, cl           /* Check for ASCIIZ null terminator */
    jz .L_stdio_loop_ok

    mov ch, 0             /* CH = 0 (No CGA color attribute used here) */
    mov eax, 46           /* EAX = 46 (TRDOS 386 sys_stdio) */
    int 0x40              /* Call kernel to print a single character in CL */
    jc .L_write_fail      /* If CF=1, kernel returned a hard hardware/I/O error */

    inc edx               /* Increment processed characters counter */
    inc esi               /* Advance buffer pointer to the next character */
    jmp .L_stdio_loop_next

.L_stdio_loop_ok:
    mov eax, edx          /* Return total number of bytes successfully written */
    jmp .L_write_done

.L_write_fail:
    mov eax, -1           /* Return -1 on error */

.L_write_done:
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
