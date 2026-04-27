;nasm

; int putchar(int c)
global _putchar
_putchar:
    push ebp
    mov ebp, esp
    
    mov eax, 46         ; sys_stdio
    mov bl, 2           ; BL = 2 (write character to stdout)
    mov cl, [ebp+8]     ; CL = character
    int 40h
    
    movzx eax, cl       ; Yazýlan karakteri döndür
    pop ebp
    ret
