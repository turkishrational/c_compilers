;nasm

;int getc(void)
global _getc
_getc:
    mov eax, 46         ; sys_stdio
    mov bl, 0           ; BL=0 -> read char (wait)
    int 40h
    ; Karakter doðrudan AL'de döndü, 
    ; u.getc tamponu kernel içinde zaten yönetiliyor.
    movzx eax, al
    ret