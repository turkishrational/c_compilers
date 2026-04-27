;nasm

; int puts(const char *s)
global _puts
_puts:
    push ebx
    push ecx
    mov ebx, [esp+12]    ; s (string adresi)
.loop:
    mov cl, [ebx]        ; Karakteri al
    test cl, cl          ; Null terminator mü?
    jz .done
    
    mov eax, 46          ; sys_stdio
    mov dl, 2            ; BL=2 (STDOUT)
    ; Not: TRDOS 386'da CL karakteri tutuyor
    push ebx             ; Register koruma (Kernel bozabilirse)
    mov bl, dl
    int 40h
    pop ebx
    
    inc ebx
    jmp .loop
.done:
    ; Yeni satýr ekle (puts standardý)
    mov eax, 46
    mov bl, 2
    mov cl, 10           ; '\n'
    int 40h
    
    pop ecx
    pop ebx
    ret
