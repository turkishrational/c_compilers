.intel_syntax noprefix
.global _vfprintf
.global __mingw_vfprintf
.global _fprintf
.global __mingw_fprintf
.global _printf
.global __print

.text

/* 18/6/2026 - Google AI */
/* =========================================================================
   1. LIBC ENTRY POINTS (BRIDGES)
   ========================================================================= */

_vfprintf:
__mingw_vfprintf:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]    /* Stream pointer or FD */
    push [ebp + 16]       /* ap */
    push [ebp + 12]       /* format */
    push eax              /* Resolved FD */
    call __print
    add esp, 12
    pop ebp
    ret

_fprintf:
__mingw_fprintf:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]
    lea ecx, [ebp + 16]   /* Variable arguments head */
    push ecx
    push [ebp + 12]
    push eax
    call __print
    add esp, 12
    pop ebp
    ret

_printf:
    push ebp
    mov ebp, esp
    lea eax, [ebp + 12]
    push eax
    push [ebp + 8]
    push 1                /* Default STDOUT FD = 1 */
    call __print
    add esp, 12
    pop ebp
    ret

/* =========================================================================
   2. BUFFERED FORMAT ENGINE (__print) - FLAT INFRASTRUCTURE
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
    movzx eax, byte ptr [esi]
    test al, al
    jz .L_parse_done

    cmp al, '%'
    je .L_handle_format

    /* --- BUFFERED CHUNK SCANNER --- */
    mov ecx, esi          
.L_scan_raw:
    movzx eax, byte ptr [ecx]
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
    test ecx, ecx
    jz .L_check_lf

    push edx              /* Protect context registers */
    push ecx              /* Arg 3: count */
    push esi              /* Arg 2: buffer pointer */
    push edx              /* Arg 1: File Descriptor */
    call _write
    add esp, 12
    pop edx
    
    add ebx, ecx          /* cc += chunk len */
    add esi, ecx          /* advance format pointer */
    jmp .L_parse_loop

.L_check_lf:
    movzx eax, byte ptr [esi]
    cmp al, 10
    jne .L_parse_loop     
    
    /* Inject "\r\n" cleanly via stack frame buffer */
    sub esp, 4
    mov word ptr [esp], 0x0A0D 
    
    push edx
    push 2                /* 2 bytes count */
    lea eax, [esp + 4]    /* stack pointer to "\r\n" */
    push eax
    push edx              /* FD */
    call _write
    add esp, 12
    pop edx
    add esp, 4
    
    add ebx, 2            
    inc esi               /* Skip LF */
    jmp .L_parse_loop

.L_handle_format:
    inc esi               /* Skip '%' */
    movzx eax, byte ptr [esi]
    test al, al
    jz .L_parse_done

    cmp al, '%'
    je .L_write_escaped_percent

    /* Skip width digits */
    cmp al, '0'
    jl .L_check_specifiers
    cmp al, '9'
    jg .L_check_specifiers
.L_skip_width:
    inc esi
    movzx eax, byte ptr [esi]
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
    cmp al, 'u'
    je .L_fmt_unsigned

.L_write_escaped_percent:
    sub esp, 4
    mov byte ptr [esp], al
    push edx
    push 1
    lea eax, [esp + 4]
    push eax
    push edx
    call _write
    add esp, 12
    pop edx
    add esp, 4
    inc ebx
    inc esi
    jmp .L_parse_loop

.L_fmt_char:
    mov eax, [edi]        
    add edi, 4
    sub esp, 4
    mov byte ptr [esp], al
    
    push edx
    push 1
    lea eax, [esp + 4]
    push eax
    push edx
    call _write
    add esp, 12
    pop edx
    add esp, 4
    
    inc ebx
    inc esi 
    jmp .L_parse_loop

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
    pop edx
    
    test eax, eax
    jz .L_fmt_str_done    
    
    push ebx              
    push edx
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
    pop edx
    pop ebx
    add ebx, eax          /* Accumulate cc */
.L_fmt_str_done:
    inc esi
    jmp .L_parse_loop

.L_fmt_integer:
    sub esp, 32           /* Allocate temporary local scratchpad frame */
    mov eax, [edi]
    add edi, 4
    lea ecx, [esp]        
    push ecx              
    push eax              
    call _itoa            
    add esp, 8

    lea eax, [esp]
    push edx
    push eax
    call _strlen
    add esp, 4
    pop edx

    push ebx
    push edx
    push eax              
    lea ecx, [esp + 8]    
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
    sub esp, 32
    mov eax, [edi]
    add edi, 4
    lea ecx, [esp]
    push 16               
    push ecx              
    push eax              
    call _itoab           
    add esp, 12
    
    lea eax, [esp]
    push edx
    push eax
    call _strlen
    add esp, 4
    pop edx

    push ebx
    push edx
    push eax              
    lea ecx, [esp + 8]    
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

.L_fmt_unsigned:
    sub esp, 32
    mov eax, [edi]
    add edi, 4
    lea ecx, [esp]
    push 10               
    push ecx
    push eax
    call _itoab
    add esp, 12
    
    lea eax, [esp]
    push edx
    push eax
    call _strlen
    add esp, 4
    pop edx

    push ebx
    push edx
    push eax              
    lea ecx, [esp + 8]    
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

.L_parse_done:
    mov eax, ebx          /* EAX = final written characters count */
    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret

/* =========================================================================
   3. INLINE STRINGS (EMBEDDED INSIDE CODE SEGMENT FOR FLAT RUNTIME)
   ========================================================================= */
.L_null_str:
    .ascii "(null)\0"
