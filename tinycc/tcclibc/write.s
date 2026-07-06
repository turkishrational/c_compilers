/* ===================================================================
; TRDOS 386 SYSTEM CALL: write / _write (TCC Native AT&T/GAS Format)
; Developer: Erdogan Tan & Google AI Architect (06/07/2026)
; =================================================================== */

.global write
.global _write

.text

write:
_write:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    /* Argument 1: fd    -> [ebp + 8]  */
    /* Argument 2: buf   -> [ebp + 12] */
    /* Argument 3: count -> [ebp + 16] */
    movl 8(%ebp), %ebx    
    movl 12(%ebp), %esi   
    movl 16(%ebp), %edi   

    cmpl $3, %ebx
    jb .L_write_stdio     /* fd < 3 (0, 1, 2) ise konsola yönlendir */

    /* -----------------------------------------------------------------------
       Standard File Write (sys_write) - Kaynaktan Hedefe (AT&T)
       ----------------------------------------------------------------------- */
    subl $3, %ebx          /* C-FD'yi TRDOS-FD'ye dönüþtür */
    movl %esi, %ecx        /* ECX = buffer adresi */
    movl %edi, %edx        /* EDX = bayt sayýsý */
    movl $4, %eax          /* EAX = 4 (TRDOS 386 sys_write) */
    int $0x40
    jnc .L_write_done
    jmp .L_write_fail

    /* -----------------------------------------------------------------------
       Console Output Write Loop (sys_stdio - CRLF & ASCIIZ Guarded)
       ----------------------------------------------------------------------- */
.L_write_stdio:
    cmpl $0, %ebx
    je .L_write_fail       /* stdin'e (fd=0) yazmak geçersizdir */

    cmpl $2, %ebx
    je .L_set_stderr
    movb $2, %bl           /* BL = 2 (stdout) */
    jmp .L_init_loop
.L_set_stderr:
    movb $3, %bl           /* BL = 3 (stderr) */

.L_init_loop:
    xorl %edx, %edx        /* EDX = Character counter */

.L_stdio_loop_next:
    cmpl %edi, %edx        /* Sayaç (%edx) == Ýstenen byte (%edi)? */
    jnb .L_stdio_loop_ok  

    movb (%esi), %cl       /* CL = ASCII karakter kodu */
    testb %cl, %cl         /* ASCIIZ null terminator kontrolü */
    jz .skip_stdio_w  
    
    cmpb $10, %cl          /* LF ('\n') kontrolü */
    jne .skip_crlf
    
    /* TCC için AT&T formatýnda yerel deðiþken byte eriþimi */
    movb .pchar, %al
    cmpb $13, %al          /* Önceki karakter CR mý? */ 
    je .skip_crlf
    
    /* CR Enjeksiyonu */
    movl $13, %ecx
    movl $46, %eax         /* EAX = 46 (TRDOS 386 sys_stdio) */
    int $0x40              
    jc .L_write_fail
    movb $10, %cl          /* CL'ye tekrar LF yükle */

.skip_crlf:
    movb %cl, .pchar       /* Güncel karakteri hafýzaya yaz */
    movb $0, %ch           /* CH = 0 (Renk özniteliði yok) */
    movl $46, %eax         /* EAX = 46 (TRDOS 386 sys_stdio) */
    int $0x40              
    jc .L_write_fail

.skip_stdio_w:
    incl %edx               
    incl %esi               
    jmp .L_stdio_loop_next

.L_stdio_loop_ok:
    movl %edx, %eax        /* Gerçek iþlenen byte sayýsýný dön */
    jmp .L_write_done

.L_write_fail:
    movl $-1, %eax           

.L_write_done:
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp
    ret

.pchar:
    .byte 0                /* Önceki karakter hafýzasý */
