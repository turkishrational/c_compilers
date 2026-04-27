;nasm

; TRDOS 386 LIBC - lseek (Revize edilmiþ)
global _lseek
_lseek:
    push ebx
    mov ebx, [esp+8]    ; fd
    mov ecx, [esp+12]   ; offset
    mov edx, [esp+16]   ; whence

    sub ebx, 3          ; C-FD (3,4..) -> TRDOS-FD (0,1..)
    jb .err             ; Eðer fd < 3 ise (negatif olduysa) hata ver (sysstdio)

    mov eax, 19         ; sys_lseek
    int 40h
    jc .err
    
    pop ebx
    ret

.err:
    mov eax, -1
    pop ebx
    ret