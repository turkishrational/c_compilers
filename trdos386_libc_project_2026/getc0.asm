;nasm

; int getc(void)
global _getc
_getc:
    mov eax, 46         ; sys_stdio
    mov bl, 0           ; BL=0 -> STDIN (Bekleyerek oku)
    int 40h
    movzx eax, al       ; Okunan ASCII kodu EAX'te
    ret