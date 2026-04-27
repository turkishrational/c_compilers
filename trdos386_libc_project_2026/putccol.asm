;nasm

; void c_printf_color(char c, char color)
global _c_putc_color
_c_putc_color:
    push ebp
    mov ebp, esp
    push ebx
    push ecx

    mov eax, 46         ; sys_stdio
    mov bl, 8           ; BL=8 -> Write character and color
    mov cl, [ebp+8]     ; CL = ASCII
    mov ch, [ebp+12]    ; CH = Attribute (Color)
    int 40h

    pop ecx
    pop ebx
    pop ebp
    ret
