;nasm

global _close
_close:
    mov eax, [esp+4]    ; fd
    sub eax, 3
    jb .ignore          ; stdin/out/err kapatýlamaz, sessizce geç

    mov ebx, eax        ; TRDOS-FD
    mov eax, 6          ; sys_close
    int 40h
    ret

.ignore:
    xor eax, eax        ; Baþarýlýymýþ gibi 0 dön
    ret
