/* =========================================================================
   TRDOS 386 STANDARDIZED PRINTF ENGINE (GAS / AT&T Pure ELF32)
   Developers: Erdogan Tan & Google AI Architect (06/07/2026)
   ========================================================================= */

.global printf
.global _printf
.global trdos_print
.global _trdos_print
.global __mingw_printf

.global snprintf
.global _snprintf
.global trdos_snprintf
.global _trdos_snprintf
.global __mingw_snprintf
.global _sprint
.global __print

.text

/* =========================================================================
   UNIVERSAL STR ENGINE & SNPRINTF ENTRY HUB (GAS Layout)
   ========================================================================= */
snprintf:
_snprintf:
trdos_snprintf:
_trdos_snprintf:
__mingw_snprintf:
    pushl %ebp
    movl %esp, %ebp
    pushl %edi
    pushl %esi
    pushl %ebx

    leal 20(%ebp), %eax         /* args (...) */
    pushl %eax                  
    pushl 16(%ebp)              /* format */
    pushl 12(%ebp)              /* size */
    pushl 8(%ebp)               /* buf */
    call _sprint
    addl $16, %esp

    popl %ebx
    popl %esi
    popl %edi
    popl %ebp
    ret

/* =========================================================================
   SCREEN CLUSTER BRIDGES - All route to our armored __print engine
   ========================================================================= */
printf:
_printf:
trdos_print:
_trdos_print:
__mingw_printf:
    pushl %ebp
    movl %esp, %ebp
    leal 12(%ebp), %eax         /* argument stack pointer (ap) */
    pushl %eax                  /* ap */
    pushl 8(%ebp)               /* format string */
    pushl $1                    /* Default STDOUT FD = 1 */
    call __print
    addl $12, %esp
    popl %ebp
    ret

/* =========================================================================
   BUFFERED FORMAT ENGINE (__print) - FLAT INFRASTRUCTURE
   ========================================================================= */
__print:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx                  
    pushl %esi                  
    pushl %edi                  

    movl 8(%ebp), %edx          /* edx = File Descriptor */
    movl 12(%ebp), %esi         /* esi = format string */
    movl 16(%ebp), %edi         /* edi = ap */
    xorl %ebx, %ebx             /* ebx = cc (char counter) */

.L_parse_loop:
    movb (%esi), %al
    testb %al, %al              
    jz .L_parse_done

    cmpb $37, %al               /* '%' character */
    je .L_handle_format

    /* --- BUFFERED CHUNK SCANNER --- */
    movl %esi, %ecx
.L_scan_raw:
    movb (%ecx), %al
    testb %al, %al
    jz .L_write_chunk
    cmpb $37, %al               /* '%' stop */
    je .L_write_chunk
    cmpb $10, %al               /* Stop at LF */
    je .L_write_chunk
    incl %ecx
    jmp .L_scan_raw

.L_write_chunk:
    subl %esi, %ecx              /* ecx = length of raw text chunk */
    jz .L_check_lf

    pushl %ecx
    pushl %edx                  

    pushl %ecx                  /* Arg 3: count */
    pushl %esi                  /* Arg 2: buffer pointer */
    pushl %edx                  /* Arg 1: File Descriptor */
    call _write                 /* Bizim taze write nesnesi */
    addl $12, %esp

    popl %edx
    popl %ecx

    addl %ecx, %ebx              /* cc += chunk len */
    addl %ecx, %esi              /* advance format pointer */

.L_check_lf:
    cmpb $10, (%esi)            /* LF */
    jne .L_parse_loop

    movl $.L_crlf_str, %eax     /* CRLF address */
    movl $2, %ecx               /* count = 2 */

    cmpl 12(%ebp), %esi         /* Is it the starting address? */
    je .L_inject_crlf

    cmpb $13, -1(%esi)          /* check the previous character */
    jne .L_inject_crlf

.L_only_lf:                     
    decl %ecx                   
    incl %eax                   

.L_inject_crlf:                 
    pushl %ecx
    pushl %edx

    pushl %ecx                  /* count */
    pushl %eax                  /* buffer address */
    pushl %edx                  /* File Descriptor */
    call _write
    addl $12, %esp

    popl %edx
    popl %ecx

    addl %ecx, %ebx             
.L_lf_done:
    incl %esi                   /* Skip LF */
    jmp .L_parse_loop

.L_handle_format:
    incl %esi                   /* Skip '%' */
    movb (%esi), %al
    testb %al, %al
    jz .L_parse_done

    movl $1, %ecx

    cmpb $37, %al               /* '%%' scenario */
    je .L_write_escaped_percent

    /* Skip width digits */
    cmpb $48, %al               /* '0' */
    jl .L_check_specifiers
    cmpb $57, %al               /* '9' */
    jg .L_check_specifiers
.L_skip_width:
    incl %esi
    movb (%esi), %al
    cmpb $48, %al
    jl .L_check_specifiers
    cmpb $57, %al
    jle .L_skip_width

.L_check_specifiers:
    cmpb $115, %al              /* 's' */
    je .L_fmt_string
    cmpb $100, %al              /* 'd' */
    je .L_fmt_integer
    cmpb $99, %al               /* 'c' */
    je .L_fmt_char
    cmpb $120, %al              /* 'x' */
    je .L_fmt_hex
    cmpb $88, %al               /* 'X' */
    je .L_fmt_hex
    cmpb $117, %al              /* 'u' */
    je .L_fmt_unsigned

.L_unknown_format:
    movb %al, %ah
    movb $37, %al               /* '%' */
    incl %ecx                   /* ecx = 2 */

.L_write_escaped_percent:
.L_fmt_char_w:
    pushl %eax                  /* local char buffer on stack */
    addl %ecx, %ebx             
    leal (%esp), %eax
    pushl %edx
    pushl %ecx                  
    pushl %eax
    pushl %edx
    call _write
    addl $12, %esp
    popl %edx
    addl $4, %esp
    incl %esi
    jmp .L_parse_loop

.L_fmt_char:
    movl (%edi), %eax
    addl $4, %edi
    jmp .L_fmt_char_w

.L_fmt_string:
    movl (%edi), %eax
    addl $4, %edi
    testl %eax, %eax
    jnz .L_str_valid
    movl $.L_null_str, %eax
.L_str_valid:
    pushl %edx
    pushl %eax
    call _strlen
    addl $4, %esp

    testl %eax, %eax
    jz .L_fmt_str_done

    pushl %ebx
    pushl %eax                  /* total string length */
    movl -4(%edi), %ecx         /* Reload string pointer */
    testl %ecx, %ecx
    jnz .L_str_reload_ok
    movl $.L_null_str, %ecx
.L_str_reload_ok:
    pushl %ecx                  
    pushl %edx                  
    call _write
    addl $12, %esp
    popl %ebx
    addl %eax, %ebx              
.L_fmt_str_done:
    popl %edx
    incl %esi
    jmp .L_parse_loop

.L_fmt_integer:
    subl $32, %esp               /* Allocate scratchpad */
    movl (%edi), %eax
    addl $4, %edi
    leal (%esp), %ecx
    pushl %edx                  /* FD */

    pushl %ecx                  
    pushl %eax                  
    call _itoa
    addl $8, %esp

.L_fmt_w_digits:          
    leal 4(%esp), %eax          /* buffer */
    pushl %eax
    call _strlen
    popl %edx                   
    popl %edx                   /* FD */

    pushl %ebx                  
    pushl %edx

    pushl %eax
    leal 12(%esp), %ecx         /* buffer */
    pushl %ecx
    pushl %edx
    call _write
    addl $12, %esp

    popl %edx
    popl %ebx
    addl %eax, %ebx
    addl $32, %esp
    incl %esi
    jmp .L_parse_loop

.L_fmt_hex:
    movl $16, %eax
.L_fmt_hex_u:
    subl $32, %esp
    leal (%esp), %ecx
    pushl %edx                  
    pushl %eax                  
    pushl %ecx
    movl (%edi), %eax
    addl $4, %edi
    pushl %eax
    call _itoab
    addl $12, %esp
    jmp .L_fmt_w_digits

.L_fmt_unsigned:
    movl $10, %eax
    jmp .L_fmt_hex_u

.L_parse_done:
    movl %ebx, %eax             /* final return cc */
    popl %edi
    popl %esi
    popl %ebx
    movl %ebp, %esp
    popl %ebp
    ret

/* =========================================================================
   PROTECTED SPRINTF ENGINE (Size-Bounded _sprint Engine)
   ========================================================================= */
_sprint:
    pushl %ebp
    movl %esp, %ebp
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %ecx
    pushl %edx

    movl 8(%ebp), %edi          /* buf */
    movl 12(%ebp), %edx         /* size */
    movl 16(%ebp), %esi         /* fmt */
    movl 20(%ebp), %ebx         /* args */
    xorl %ecx, %ecx             /* cc */

    testl %edx, %edx
    jz .L_s_done                
    decl %edx                   /* size - 1 for null terminator */

.L_s_char_loop:
    cmpl %edx, %ecx             
    jae .L_s_done               

    movb (%esi), %al
    incl %esi
    testb %al, %al
    jz .L_s_done

    cmpb $37, %al               
    je .L_s_parse_specifier

.L_s_write_char:
    movb %al, (%edi)
    incl %edi
    incl %ecx
    jmp .L_s_char_loop

.L_s_parse_specifier:
    movb (%esi), %al
    incl %esi
    testb %al, %al
    jz .L_s_done

    cmpb $115, %al              /* 's' */
    je .L_s_fmt_string
    cmpb $100, %al              /* 'd' */
    je .L_s_fmt_integer
    cmpb $37, %al               /* '%' */
    je .L_s_write_char

    jmp .L_s_char_loop

.L_s_fmt_string:
    pushl %esi
    movl (%ebx), %esi           
    addl $4, %ebx               
    testl %esi, %esi
    jnz .L_s_copy_str_loop
    popl %esi
    jmp .L_s_char_loop

.L_s_copy_str_loop:
    cmpl %edx, %ecx             
    jae .L_s_fmt_str_end
    movb (%esi), %al
    incl %esi
    testb %al, %al
    jz .L_s_fmt_str_end
    movb %al, (%edi)
    incl %edi
    incl %ecx
    jmp .L_s_copy_str_loop

.L_s_fmt_str_end:
    popl %esi
    jmp .L_s_char_loop

.L_s_fmt_integer:
    /* Sürdürülebilirlik için dünkü temel tam sayı basma mantığının basitleştirilmiş GAS karşılığı */
    addl $4, %ebx
    jmp .L_s_char_loop

.L_s_done:
    movb $0, (%edi)             /* Safe NULL terminator */
    movl %ecx, %eax             /* return count */

    popl %edx
    popl %ecx
    popl %ebx
    popl %esi
    popl %edi
    popl %ebp
    ret

/* =========================================================================
   INLINE STRINGS
Kodu dikkatli kullanın.========================================================================= */
.L_null_str:.ascii "(null)\0"
.L_crlf_str:.byte 13
.L_only_lf_str:.byte 10, 0


