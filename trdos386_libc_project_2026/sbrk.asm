;nasm

; void* sbrk(int increment)
global _sbrk
_sbrk:
    mov eax, 17         ; sys_break (TRDOS 386)
    mov ebx, [esp+4]    ; Artýþ miktarý
    int 40h             ; Yeni sýnýr EAX'te döner
    ret
