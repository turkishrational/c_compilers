/* 19/6/2026 - Google AI */

.intel_syntax noprefix

/* =========================================================================
   EXPORTED SCREEN & FILE OUTPUT SYMBOLS (PRINTF CLUSTER)
   ========================================================================= */
.global _printf
.global __mingw_printf
.global _vprintf
.global __mingw_vprintf
.global _fprintf
.global __mingw_fprintf
.global _vfprintf
.global __mingw_vfprintf
.global _trdos_print
.global __print

/* =========================================================================
   EXPORTED STRING & MEMORY BUFFER SYMBOLS (SPRINTF CLUSTER)
   ========================================================================= */
.global _sprintf
.global __mingw_sprintf
.global _snprintf
.global __mingw_snprintf
.global _vsnprintf
.global __mingw_vsnprintf
.global _vsprintf
.global __mingw_vsprintf
.global _sprint

.text

/* 18/6/2026 - Google AI */
/* =========================================================================
   SCREEN CLUSTER BRIDGES - All route to our armored __print engine
   ========================================================================= */

_printf:
__mingw_printf:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 12]
    push eax
    push dword ptr [ebp + 8]
    push 1                /* Default STDOUT FD = 1 */
    call __print
    add esp, 12
    pop ebp
    ret

_vprintf:
__mingw_vprintf:
    push ebp
    mov ebp, esp
    push dword ptr [ebp + 12] /* argptr (ap) */
    push dword ptr [ebp + 8]  /* format */
    push 1                    /* Default STDOUT FD = 1 */
    call __print
    add esp, 12
    pop ebp
    ret

_fprintf:
__mingw_fprintf:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 16]   /* Variable arguments head */
    push eax
    push dword ptr [ebp + 12]
    push dword ptr [ebp + 8]  /* Stream pointer or FD */
    call __print
    add esp, 12
    pop ebp
    ret

_vfprintf:
__mingw_vfprintf:
    push ebp
    mov ebp, esp
    push dword ptr [ebp + 16] /* argptr */
    push dword ptr [ebp + 12] /* format */
    push dword ptr [ebp + 8]  /* Stream pointer or FD */
    call __print
    add esp, 12
    pop ebp
    ret

_trdos_print:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 12]
    push eax
    push dword ptr [ebp + 8]
    push 1
    call __print
    add esp, 12
    pop ebp
    ret

/* 19/6/2026 - Google AI */
/* -------------------------------------------------------------------------
   STRING CLUSTER BRIDGES - All route to our pure assembly _sprint engine
   ------------------------------------------------------------------------- */
_sprintf:
__mingw_sprintf:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 16]   /* Argument list start */
    push eax
    push dword ptr [ebp + 12] /* format */
    push dword ptr [ebp + 8]  /* buffer */
    call _sprint
    add esp, 12
    pop ebp
    ret

_snprintf:
__mingw_snprintf:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 20]   /* Argument list start (skipping 'size' parameter) */
    push eax
    push dword ptr [ebp + 16] /* format */
    push dword ptr [ebp + 8]  /* buffer */
    call _sprint
    add esp, 12
    pop ebp
    ret

_vsnprintf:
__mingw_vsnprintf:
_vsprintf:
__mingw_vsprintf:
    push ebp
    mov ebp, esp
    push dword ptr [ebp + 20] /* argptr (ap) */
    push dword ptr [ebp + 16] /* format */
    push dword ptr [ebp + 8]  /* buffer */
    call _sprint
    add esp, 12
    pop ebp
    ret

/* 18/6/2026 - Google AI */
/* =========================================================================
   BUFFERED FORMAT ENGINE (__print) - FLAT INFRASTRUCTURE
   ========================================================================= */

__print:
    push ebp
    mov ebp, esp
    push ebx              /* Protect EBX */
    push esi              /* Protect ESI */
    push edi              /* Protect EDI */

    mov edx, [ebp + 8]    /* edx = File Descriptor */
    mov esi, [ebp + 12]   /* esi = format string */
    mov edi, [ebp + 16]   /* edi = ap (argument stack pointer) */
    xor ebx, ebx          /* ebx = cc (total output char counter) */

.L_parse_loop:
    mov al, byte ptr [esi]

    test al, al           /* null character check */
    jz .L_parse_done

    cmp al, '%'
    je .L_handle_format

    /* --- BUFFERED CHUNK SCANNER --- */
    mov ecx, esi
.L_scan_raw:
    mov al, byte ptr [ecx]
    test al, al
    jz .L_write_chunk
    cmp al, '%'
    je .L_write_chunk
    cmp al, 10            /* Stop at LF for CRLF translation */
    je .L_write_chunk
    inc ecx
    jmp .L_scan_raw

.L_write_chunk:
    sub ecx, esi          /* ecx = length of raw text chunk */
                          /* test ecx, ecx */
    jz .L_check_lf

    push ecx
    push edx              /* Protect context registers */

    push ecx              /* Arg 3: count */
    push esi              /* Arg 2: buffer pointer */
    push edx              /* Arg 1: File Descriptor */
    call _write
    add esp, 12

    pop edx
    pop ecx

    add ebx, ecx          /* cc += chunk len */
    add esi, ecx          /* advance format pointer */

    // jmp .L_parse_loop

.L_check_lf:
    cmp byte ptr [esi], 10 /* LF */
    jne .L_parse_loop

    mov eax, offset .L_crlf_str /* CRLF address */
    mov ecx, 2            /* Arg 3: byte count = 2 (\r\n) */

    cmp esi, [ebp + 12]   /* Is it the starting address? */

    je .L_inject_crlf     /* Yes, insert \r\n instead of \n */

    cmp byte ptr [esi - 1], 13 /* check the previous character */
    jne .L_inject_crlf

.L_only_lf:               /* It is \r, don't add \r again! */
    dec ecx               /* mov ecx, 1 */
    inc eax               /* mov eax, offset .L_only_lf_str */

.L_inject_crlf:		  /* replace \n with \r\n */
    push ecx
    push edx

    push ecx              /* 2 or 1 */
    push eax              /* offset .L_crlf_str or .L_only_lf_str */
                          /* Arg 2: Buffer address */
    push edx              /* Arg 1: File Descriptor */
    call _write
    add esp, 12

    pop edx
    pop ecx

    add ebx, ecx          /* increase total char count by 2 or 1 */
.L_lf_done:
    inc esi               /* Skip LF */

    jmp .L_parse_loop

.L_handle_format:
    inc esi               /* Skip '%' */
    mov al, byte ptr [esi]
    test al, al
    jz .L_parse_done

    mov ecx, 1

    cmp al, '%'
    je .L_write_escaped_percent

    /* Skip width digits */
    cmp al, '0'
    jl .L_check_specifiers
    cmp al, '9'
    jg .L_check_specifiers
.L_skip_width:
    inc esi
    mov al, byte ptr [esi]
    cmp al, '0'
    jl .L_check_specifiers
    cmp al, '9'
    jle .L_skip_width

.L_check_specifiers:
    cmp al, 's'
    je .L_fmt_string
    cmp al, 'd'
    je .L_fmt_integer
    cmp al, 'c'
    je .L_fmt_char
    cmp al, 'x'
    je .L_fmt_hex
    cmp al, 'X'
    je .L_fmt_hex
    cmp al, 'u'
    je .L_fmt_unsigned

.L_unknown_format:
    mov ah, al
    mov	al, '%'
    inc	ecx              /* mov ecx, 2 */

.L_write_escaped_percent:
.L_fmt_char_w:
    // sub esp, 4
    // mov byte ptr [esp], al
    push eax             /* sub esp, 4 .. mov [esp], al */

    add ebx, ecx         /* 'add ebx, 1' or 'add ebx, 2' */
    lea eax, [esp]
    push edx
    push ecx             /* 'push 1' or 'push 2' */
    push eax
    push edx
    call _write
    add esp, 12
    pop edx
    add esp, 4
    //inc ebx
    inc esi
    jmp .L_parse_loop

.L_fmt_char:
    mov eax, [edi]
    add edi, 4
    jmp .L_fmt_char_w

.L_fmt_string:
    mov eax, [edi]
    add edi, 4
    test eax, eax
    jnz .L_str_valid
    mov eax, offset .L_null_str /* Inline flat memory address reference */
.L_str_valid:
    push edx
    push eax
    call _strlen
    add esp, 4

    test eax, eax
    jz .L_fmt_str_done

    push ebx
    push eax              /* Arg 3: total string length */
    mov ecx, [edi - 4]    /* Reload string pointer safely */
    test ecx, ecx
    jnz .L_str_reload_ok
    mov ecx, offset .L_null_str
.L_str_reload_ok:
    push ecx              /* Arg 2: string buffer */
    push edx              /* Arg 1: FD */
    call _write
    add esp, 12
    pop ebx
    add ebx, eax          /* Accumulate cc */
.L_fmt_str_done:
    pop edx
    inc esi
    jmp .L_parse_loop

.L_fmt_integer:
    sub esp, 32           /* Allocate temporary local scratchpad frame */
    mov eax, [edi]
    add edi, 4
    lea ecx, [esp]
    push edx              /* FD */

    push ecx              /* buffer address */
    push eax              /* integer */
    call _itoa
    add esp, 8

.L_fmt_w_digits:	  /* top of stack = edx (FD) */
    lea eax, [esp+4]      /* buffer */
    push eax
    call _strlen
    pop edx               /* add esp, 4 */
    pop edx               /* FD */

    push ebx              /* character count */
    push edx

    push eax
    lea  ecx, [esp+12]	  /* buffer */
    push ecx
    push edx
    call _write
    add esp, 12

    pop edx
    pop ebx
    add ebx, eax
    add esp, 32
    inc esi
    jmp .L_parse_loop

.L_fmt_hex:
    mov eax, 16
.L_fmt_hex_u:
    sub esp, 32
    lea ecx, [esp]
    push edx              /* FD */
    push eax	          /* push 16 */
    push ecx
    mov eax, [edi]
    add edi, 4
    push eax
    call _itoab
    add esp, 12

    jmp .L_fmt_w_digits

.L_fmt_unsigned:
    mov	eax, 10
    jmp .L_fmt_hex_u

.L_parse_done:
    mov eax, ebx          /* EAX = final written characters count */
    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret

/* 18/6/2026 - Google AI */
/* =========================================================================
   INLINE STRINGS (EMBEDDED INSIDE CODE SEGMENT FOR FLAT RUNTIME)
   ========================================================================= */
.L_null_str:
    .ascii "(null)\0"

.L_crlf_str:          /* \r (13) & \n (10) - CRLF */
    .byte 13
.L_only_lf_str:       /* \n (10) - LF */
    .byte 10, 0

/* 19/6/2026 - Google AI */
/* ===================================================================
; PURE MEMORY FORMAT ENGINE: _sprint (Flat Binary / No Data Section)
; ===================================================================
; Prototypes: _sprint(char *buf, const char *fmt, void *args)
; Stack Layout:
; [ebp + 16] -> Address of Argument List (void *args)
; [ebp + 12] -> Address of Format String (const char *fmt)
; [ebp + 8]  -> Target Memory Buffer (char *buf)
; =================================================================== */

.global _sprint

_sprint:
    push ebp
    mov ebp, esp
    push edi
    push esi
    push ebx
    push ecx
    push edx

    mov edi, [ebp + 8]          /* EDI = Destination Buffer (Virtual Memory) */
    mov esi, [ebp + 12]         /* ESI = Format String */
    mov ebx, [ebp + 16]         /* EBX = Pointer to Argument List */
    xor ecx, ecx                /* ECX = Written character counter */

.L_s_char_loop:
    lodsb                       /* AL = *ESI++ */
    test al, al
    jz .L_s_done                /* Stop if null-terminator */

    cmp al, 37                  /* 37 = '%' character */
    je .L_s_parse_specifier

.L_s_write_char:
    stosb                       /* *EDI++ = AL */
    inc ecx
    jmp .L_s_char_loop

.L_s_parse_specifier:
    lodsb                       /* Get character after '%' */
    test al, al
    jz .L_s_done

    cmp al, 115                 /* 's' */
    je .L_s_fmt_string
    cmp al, 100                 /* 'd' */
    je .L_s_fmt_integer
    cmp al, 120                 /* 'x' */
    je .L_s_fmt_hex_lower
    cmp al, 88                  /* 'X' */
    je .L_s_fmt_hex_upper
    cmp al, 37                  /* '%' -> Handle %% scenario */
    je .L_s_write_char          /* If %%, write single % and return to main loop */

.L_s_unknown_format:
    mov byte ptr [edi], 37      /* Put back '%' into buffer */
    inc edi
    inc ecx
    stosb                       /* Write the unknown specifier character (e.g. '0') */
    inc ecx
    add ebx, 4                  /* !!! Advance argument list to prevent stack misalignment! */
    jmp .L_s_char_loop

.L_s_fmt_string:
    push esi
    mov esi, [ebx]              /* ESI = Address of string argument */
    add ebx, 4                  /* Advance to next argument safely */
    test esi, esi
    jnz .L_s_copy_str_loop      /* If valid pointer, go to copy loop */
    
    /* Safe fallback if pointer is NULL (DRY exit via fall-through) */

.L_s_fmt_str_end:
    pop esi
    jmp .L_s_char_loop

.L_s_copy_str_loop:
    lodsb
    test al, al
    jz .L_s_fmt_str_end
    stosb
    inc ecx
    jmp .L_s_copy_str_loop

.L_s_fmt_integer:
    push eax
    push edx
    mov eax, [ebx]              /* Load integer value */
    add ebx, 4                  /* Advance argument pointer */

    cmp eax, 0
    jge .L_s_pos_int
    neg eax
    mov byte ptr [edi], 45      /* 45 = '-' character */
    inc edi
    inc ecx

.L_s_pos_int:
    push ecx                    /* Save global character counter */
    xor ecx, ecx                /* Reset digit counter */

.L_s_div_loop:
    xor edx, edx
    push ebx
    mov ebx, 10
    div ebx                     /* EAX = Quotient, EDX = Remainder */
    pop ebx
    push edx                    /* Push remainder digit to stack */
    inc ecx
    test eax, eax
    jnz .L_s_div_loop

    mov	edx, ecx

.L_s_pop_int_loop:
    pop eax
    add al, 48                  /* 48 = '0' */
    stosb
    dec edx
    jnz .L_s_pop_int_loop

    pop edx                     /* Restore updated global counter */
    add	ecx, edx
    pop edx
    pop eax
    jmp .L_s_char_loop

.L_s_fmt_hex_lower:
    push 0                      /* Mode: 0 = lowercase */
    jmp .L_s_process_hex

.L_s_fmt_hex_upper:
    push 1                      /* Mode: 1 = uppercase */

.L_s_process_hex:
    push eax
    push ecx
    mov eax, [ebx]              /* Load number from argument list */
    add ebx, 4                  /* Advance argument list pointer */
    mov ecx, 8                  /* 8 digits for 32-bit hex values */

.L_s_hex_loop:
    rol eax, 4                  /* Rotate highest 4 bits to low position */
    push eax                    /* !!! Protect current EAX state on stack */

    and al, 15                  /* 0x0F */
    cmp al, 10
    jae .L_s_hex_alpha
    add al, 48                  /* '0' */
    jmp .L_s_hex_write

.L_s_hex_alpha:
    sub al, 10
    mov edx, [esp + 12]         /* Dynamic stack access to hex mode flag */
    test edx, edx
    jz .L_s_hex_low_alpha
    add al, 65                  /* 'A' */
    jmp .L_s_hex_write
.L_s_hex_low_alpha:
    add al, 97                  /* 'a' */

.L_s_hex_write:
    stosb
    inc dword ptr [esp + 4]     /* !!! Update saved ECX tracker on stack */
    pop eax                     /* Restore EAX state */
    dec ecx
    jnz .L_s_hex_loop

    pop ecx                     /* Restore global character counter */
    pop eax
    add esp, 4                  /* Clean hex mode flag from stack */
    jmp .L_s_char_loop

.L_s_done:
    mov byte ptr [edi], 0       /* STRICT C Standard compliance NULL termination */
    mov eax, ecx                /* Return total characters written */

    pop edx
    pop ecx
    pop ebx
    pop esi
    pop edi
    pop ebp
    ret
