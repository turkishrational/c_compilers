;nasm

; int isatty(int fd)
global _isatty
_isatty:
    mov eax, [esp+4]    ; fd
    cmp eax, 3
    jb .yes             ; 0, 1, 2 her zaman tty kabul edilir
    xor eax, eax        ; 3 ve sonrasý disk dosyasýdýr, 0 dön (hayýr)
    ret
.yes:
    ;mov eax, 1         ; 1 dön (evet)
    mov al,1
    ret