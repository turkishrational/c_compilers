;int putc(int c)
global _putc
_putc:
    mov eax, 46         ; sys_stdio (TRDOS 386)
    mov bl, 2           ; BL=2 -> STDOUT (Yönlendirme destekli)
    mov cl, [esp+4]     ; Karakter
    int 40h
    movzx eax, cl       ; Yazýlan karakteri döndür
    ret