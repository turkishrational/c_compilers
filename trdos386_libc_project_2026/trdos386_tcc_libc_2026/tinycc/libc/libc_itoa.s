.intel_syntax noprefix
.global _itoa
.global _itoab

.text

/* 18/6/2026 - Google AI */
/* =========================================================================
   void _itoa(int value, char *str)
   Converts a signed 32-bit integer into a base-10 null-terminated string.
   ========================================================================= */
_itoa:
    push ebp
    mov ebp, esp
    push ebx              /* Preserve EBX per cdecl rules */
    push esi              /* Preserve ESI for reverse tracking */
    push edi              /* Preserve EDI for buffer navigation */

    mov eax, [ebp + 8]    /* eax = signed integer value */
    mov edi, [ebp + 12]   /* edi = destination character buffer pointer */
    mov esi, edi          /* esi = backup buffer start to define reverse limit */

    test eax, eax
    jns .L_itoa_positive  /* Skip sign insertion if value is non-negative */
    
    /* Input is negative: insert minus sign and negate the value */
    mov byte ptr [edi], '-'
    inc edi
    inc esi               /* Shift the reverse boundary past the minus sign */
    neg eax               /* Make the value positive */

.L_itoa_positive:
    mov ebx, 10           /* Divisor base = 10 */

.L_itoa_loop:
    xor edx, edx          /* Clear EDX before 32-bit unsigned division */
    div ebx               /* eax = quotient, edx = remainder */
    add dl, '0'           /* Convert remainder digit to ASCII character */
    mov [edi], dl         /* Store character into the buffer */
    inc edi               /* Advance buffer pointer */
    test eax, eax         /* Check if quotient is zero */
    jnz .L_itoa_loop      /* Continue loop if characters remain */

    mov byte ptr [edi], 0 /* Append null terminator to string end */
    dec edi               /* edi now points to the last valid digit character */

    /* Reverse the extracted digits in-place since they were processed backwards */
.L_itoa_reverse:
    cmp esi, edi          /* Check if pointers met or crossed */
    jge .L_itoa_done      /* Reversal complete */
    mov al, [esi]         /* Fetch left-side character */
    mov bl, [edi]         /* Fetch right-side character */
    mov [esi], bl         /* Swap right character to left side */
    mov [edi], al         /* Swap left character to right side */
    inc esi               /* Move left pointer forward */
    dec edi               /* Move right pointer backward */
    jmp .L_itoa_reverse

.L_itoa_done:
    pop edi               /* Restore register state */
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret

/* =========================================================================
   void _itoab(unsigned int value, char *str, int base)
   Converts an unsigned integer into a null-terminated string using bases 2-16.
   ========================================================================= */
_itoab:
    push ebp
    mov ebp, esp
    push ebx              /* Preserve EBX per cdecl rules */
    push esi              /* Preserve ESI for reverse tracking */
    push edi              /* Preserve EDI for buffer navigation */

    mov eax, [ebp + 8]    /* eax = unsigned integer value */
    mov edi, [ebp + 12]   /* edi = destination character buffer pointer */
    mov ecx, [ebp + 16]   /* ecx = radix base (e.g., 2, 8, 10, 16) */
    mov esi, edi          /* esi = backup buffer start */

    /* Radix sanity check safety boundary constraints */
    cmp ecx, 2
    jl .L_itoab_invalid   /* Fallback if base is less than 2 */
    cmp ecx, 16
    jg .L_itoab_invalid   /* Fallback if base is greater than 16 */
    jmp .L_itoab_loop

.L_itoab_invalid:
    mov ecx, 10           /* Enforce safe fallback base 10 on error */

.L_itoab_loop:
    xor edx, edx          /* Clear upper dividend register */
    div ecx               /* eax = quotient, edx = remainder */
    
    /* Map remainder to alphanumeric hex representation range symbol */
    cmp dl, 9
    ja .L_itoab_alpha     /* Branch if value requires alphabet digit (a-f) */
    add dl, '0'           /* Map numeric index 0-9 to ASCII '0'-'9' */
    jmp .L_itoab_store

.L_itoab_alpha:
    add dl, 'a' - 10      /* Map alpha value 10-15 to lowercase ASCII 'a'-'f' */

.L_itoab_store:
    mov [edi], dl         /* Store generated symbol into the buffer */
    inc edi               /* Advance pointer */
    test eax, eax         /* Loop until quotient reaches absolute zero */
    jnz .L_itoab_loop

    mov byte ptr [edi], 0 /* Append string null terminator */
    dec edi               /* edi now points to the final valid alphanumeric digit */

    /* Reverse the buffer string layout in-place to correct directional layout */
.L_itoab_reverse:
    cmp esi, edi          /* Verify collision intersection */
    jge .L_itoab_done
    mov al, [esi]
    mov bl, [edi]
    mov [esi], bl
    mov [edi], al
    inc esi
    dec edi
    jmp .L_itoab_reverse

.L_itoab_done:
    pop edi               /* Restore registers */
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret
