;nasm

; int getchar(void)
global _getchar
_getchar:
    push ebp
    mov ebp, esp
    
    mov eax, 46         ; sys_stdio
    mov bl, 0           ; BL = 0 (read character - wait)
    int 40h
    
    movzx eax, al       ; Okunan karakter EAX'e (AL'den)
    pop ebp
    ret
