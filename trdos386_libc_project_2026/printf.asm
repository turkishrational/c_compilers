;nasm

; TRDOS 386 LIBC - printf (Small-C Style)
global _printf
extern _putchar, _strlen

section .text
_printf:
    push ebp
    mov ebp, esp
    push esi
    push ebx

    mov esi, [ebp+8]    ; format string
    lea ebx, [ebp+12]   ; ilk argüman adresi (va_list)

.loop:
    lodsb               ; AL = *format++
    test al, al
    jz .done
    cmp al, '%'
    jne .putc
    
    lodsb               ; Format karakterini al
    test al, al
    jz .done

    ; Format kontrolü
    cmp al, 'd'
    je .print_dec
    cmp al, 'x'
    je .print_hex
    cmp al, 'o'
    je .print_oct
    cmp al, 'b'
    je .print_bin
    cmp al, 's'
    je .print_str
    cmp al, 'c'
    je .print_char
    jmp .putc           ; Bilinmeyen formatý olduðu gibi bas

.print_dec:
    mov eax, [ebx]
    push 10
    push eax
    call _printn        ; Sayý basan yardýmcý fonksiyon
    add esp, 8
    jmp .next_arg

.print_hex:
    mov eax, [ebx]
    push 16
    push eax
    call _printn
    add esp, 8
    jmp .next_arg

.print_oct:
    mov eax, [ebx]
    push 8
    push eax
    call _printn
    add esp, 8
    jmp .next_arg

.print_bin:
    mov eax, [ebx]
    push 2
    push eax
    call _printn
    add esp, 8
    jmp .next_arg

.print_str:
    mov eax, [ebx]
    push eax
    call _putstr_simple
    pop eax
    jmp .next_arg

.print_char:
    mov eax, [ebx]
    push eax
    call _putchar
    pop eax
    jmp .next_arg

.putc:
    push eax
    call _putchar
    pop eax
    jmp .loop

.next_arg:
    add ebx, 4
    jmp .loop

.done:
    pop ebx
    pop esi
    pop ebp
    ret

_putstr_simple:
    mov esi, [esp+4]
.sl: lodsb
    test al, al
    jz .sd
    push eax
    call _putchar
    pop eax
    jmp .sl
.sd: ret
