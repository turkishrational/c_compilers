;nasm

; int close(int fd)
global _close
_close:
    mov ebx, [esp+4]
    sub ebx, 3
    jb .ignore          ; 0, 1, 2 ise bir þey yapma

    mov eax, 6          ; sys_close
    int 40h
    jc .err
.ok:
.ignore:		; Hata verme, baþarýlý dön
    xor eax, eax
    ret
.err:
    mov eax, -1
    ret
