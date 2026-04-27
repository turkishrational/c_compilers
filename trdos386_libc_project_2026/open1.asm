;nasm

; int open(char *path, int mode)
global _open
_open:
    mov eax, 5          ; sys_open
    mov ebx, [esp+4]    ; path
    mov ecx, [esp+8]    ; mode
    int 40h
    jc .err             ; Carry Flag set ise hata
    add eax, 3          ; 0-9 -> 3-12 (C standardı)
    ret
.err:
    mov eax, -1
    ret
