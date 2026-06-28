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
       Standard File Write (sys_write) - DEÐÝÞMEDÝ
       ----------------------------------------------------------------------- */
    sub ebx, 3            /* Convert C-FD to TRDOS-FD */
    mov ecx, esi          /* ECX = buffer address */
    mov edx, edi          /* EDX = byte count */
    mov eax, 4            /* EAX = 4 (TRDOS 386 sys_write) */
    int 0x40
    jnc .L_write_done
    jmp .L_write_fail

    /* -----------------------------------------------------------------------
       Console Output Write Loop (sys_stdio - FIX: Sadece EDI Count Kadar Basar)
       ----------------------------------------------------------------------- */

    /* 24/6/2026 - Erdogan Tan */
.pchar:
    .byte 0		  /* Previous Character - CRLF check */

.L_write_stdio:
    cmp ebx, 0
    je .L_write_fail      /* Writing to stdin (fd=0) is invalid */

    /* Map C-FD to TRDOS sys_stdio BL codes */
    cmp ebx, 2
    je .L_set_stderr
    mov bl, 2             /* BL = 2 (stdout) */
    jmp .L_init_loop
.L_set_stderr:
    mov bl, 3             /* BL = 3 (stderr) */

.L_init_loop:
    xor edx, edx          /* EDX = Character counter (bytes written) */

.L_stdio_loop_next:
    cmp edx, edi          /* Sýnýr Kontrolü: Sayaç (EDX) == Ýstenen byte (EDI) oldu mu? */
    jnb .L_stdio_loop_ok  /* Sayaca ulaþýldýysa null karakter uyarýsý olmadan baþarýyla çýk */

    mov cl, [esi]         /* CL = ASCII character code (Null dahil her þey basýlýr) */
    /* 24/6/2026 */
    test cl, cl           /* Check for ASCIIZ null terminator */
    // jz .L_stdio_loop_ok
    jz .skip_stdio_w  
    
    cmp	cl, 10            /* LF */
    jne .skip_crlf
    cmp byte ptr [.pchar], 13 /* CR */ 
    je  .skip_crlf
    mov ecx, 13
    mov eax, 46           /* EAX = 46 (TRDOS 386 sys_stdio) */
    int 0x40              /* Call kernel to print a single character */
    jc .L_write_fail
    mov cl, 10
.skip_crlf:
    mov byte ptr [.pchar], cl

    mov ch, 0             /* CH = 0 (No CGA color attribute) */
    mov eax, 46           /* EAX = 46 (TRDOS 386 sys_stdio) */
    int 0x40              /* Call kernel to print a single character */
    jc .L_write_fail

.skip_stdio_w:
    inc edx               /* Increment processed characters counter */
    inc esi               /* Advance buffer pointer to the next character */
    jmp .L_stdio_loop_next

.L_stdio_loop_ok:
    mov eax, edx          /* TCC'ye tam olarak yazýlan gerçek byte sayýsýný dön */
    jmp .L_write_done

.L_write_fail:
    mov eax, -1           /* Return -1 on error */

.L_write_done:
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
