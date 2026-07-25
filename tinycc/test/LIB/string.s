/* =========================================================================
   TRDOS 386 CORE STRING & MATH LIBRARY (GAS / AT&T Pure ELF32)
   Developers: Erdogan Tan & Google AI Architect (06/07/2026)
   ========================================================================= */

.global _itoa
.global itoa
.global _itoab
.global itoab
.global _strlen
.global strlen

.text

/* =========================================================================
   void _itoa(int value, char *str)
   Converts a signed 32-bit integer into a base-10 null-terminated string.
   ========================================================================= */
_itoa:
itoa:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx              /* Preserve EBX per cdecl rules */
    pushl %esi              /* Preserve ESI for reverse tracking */
    pushl %edi              /* Preserve EDI for buffer navigation */

    movl 8(%ebp), %eax      /* eax = signed integer value */
    movl 12(%ebp), %edi     /* edi = destination character buffer pointer */
    movl %edi, %esi         /* esi = backup buffer start to define reverse limit */

    testl %eax, %eax
    jns .L_itoa_positive    /* Skip sign insertion if value is non-negative */
    
    /* Input is negative: insert minus sign and negate the value */
    movb $45, (%edi)        /* 45 = '-' */
    incl %edi
    incl %esi               /* Shift the reverse boundary past the minus sign */
    negl %eax               /* Make the value positive */

.L_itoa_positive:
    movl $10, %ebx          /* Divisor base = 10 */

.L_itoa_loop:
    xorl %edx, %edx         /* Clear EDX before 32-bit unsigned division */
    divl %ebx               /* eax = quotient, edx = remainder */
    addb $48, %dl           /* 48 = '0' (Convert remainder digit to ASCII) */
    movb %dl, (%edi)        /* Store character into the buffer */
    incl %edi               /* Advance buffer pointer */
    testl %eax, %eax        /* Check if quotient is zero */
    jnz .L_itoa_loop        /* Continue loop if characters remain */

    movb $0, (%edi)         /* Append null terminator to string end */
    decl %edi               /* edi now points to the last valid digit character */

    /* Reverse the extracted digits in-place since they were processed backwards */
.L_itoa_reverse:
    cmpl %edi, %esi         /* Check if pointers met or crossed */
    jge .L_itoa_done        /* Reversal complete */
    movb (%esi), %al        /* Fetch left-side character */
    movb (%edi), %bl        /* Fetch right-side character */
    movb %bl, (%esi)        /* Swap right character to left side */
    movb %al, (%edi)        /* Swap left character to right side */
    incl %esi               /* Move left pointer forward */
    decl %edi               /* Move right pointer backward */
    jmp .L_itoa_reverse

.L_itoa_done:
    popl %edi               /* Restore register state */
    popl %esi
    popl %ebx
    movl %ebp, %esp
    popl %ebp
    ret

/* =========================================================================
   void _itoab(unsigned int value, char *str, int base)
   Converts an unsigned integer into a null-terminated string using bases 2-16.
   ========================================================================= */
_itoab:
itoab:
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx              
    pushl %esi              
    pushl %edi              

    movl 8(%ebp), %eax      /* eax = unsigned integer value */
    movl 12(%ebp), %edi     /* edi = destination character buffer pointer */
    movl 16(%ebp), %ecx     /* ecx = radix base (e.g., 2, 8, 10, 16) */
    movl %edi, %esi         /* esi = backup buffer start */

    /* Radix sanity check safety boundary constraints */
    cmpl $2, %ecx
    jl .L_itoab_invalid     /* Fallback if base is less than 2 */
    cmpl $16, %ecx
    jg .L_itoab_invalid     /* Fallback if base is greater than 16 */
    jmp .L_itoab_loop

.L_itoab_invalid:
    movl $10, %ecx          /* Enforce safe fallback base 10 on error */

.L_itoab_loop:
    xorl %edx, %edx         /* Clear upper dividend register */
    divl %ecx               /* eax = quotient, edx = remainder */
    
    /* Map remainder to alphanumeric hex representation range symbol */
    cmpb $9, %dl
    ja .L_itoab_alpha       /* Branch if value requires alphabet digit (a-f) */
    addb $48, %dl           /* 48 = '0' */
    jmp .L_itoab_store

.L_itoab_alpha:
    addb $87, %dl           /* 87 = 'a' - 10 (Map alpha value 10-15 to lowercase 'a'-'f') */

.L_itoab_store:
    movb %dl, (%edi)        /* Store generated symbol into the buffer */
    incl %edi               /* Advance pointer */
    testl %eax, %eax        /* Loop until quotient reaches absolute zero */
    jnz .L_itoab_loop

    movb $0, (%edi)         /* Append string null terminator */
    decl %edi               /* edi now points to the final valid alphanumeric digit */

    /* Reverse the buffer string layout in-place to correct directional layout */
.L_itoab_reverse:
    cmpl %edi, %esi         /* Verify collision intersection */
    jge .L_itoab_done
    movb (%esi), %al
    movb (%edi), %bl
    movb %bl, (%esi)
    movb %al, (%edi)
    incl %esi
    decl %edi
    jmp .L_itoab_reverse

.L_itoab_done:
    popl %edi               /* Restore registers */
    popl %esi
    popl %ebx
    movl %ebp, %esp
    popl %ebp
    ret

/* =========================================================================
   int _strlen(const char *str)
   Computes the length of a null-terminated string.
   ========================================================================= */
_strlen:
strlen:
    pushl %ebp
    movl %esp, %ebp
    pushl %edi

    movl 8(%ebp), %edi
    xorl %eax, %eax         /* Clear EAX (counter = 0) */

.L_strlen_loop:
    cmpb $0, (%edi)
    je .L_strlen_done
    incl %edi
    incl %eax
    jmp .L_strlen_loop

.L_strlen_done:
    popl %edi
    movl %ebp, %esp
    popl %ebp
    ret
