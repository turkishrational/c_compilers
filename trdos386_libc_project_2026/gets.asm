;nasm

; char* gets(char *s)
global _gets
_gets:
    push ebx
    mov ebx, [esp+8]     ; s (buffer adresi)
    mov edx, ebx         ; Baþlangýç adresini sakla
.loop:
    mov eax, 46          ; sys_stdio
    mov bl, 0            ; BL=0 (read wait)
    int 40h
    ; Karakter AL'de döndü
    
    cmp al, 13           ; CR (Enter)?
    je .done
    cmp al, 10           ; LF?
    je .done
    
    mov [edx], al        ; Buffer'a yaz
    inc edx
    
    ; Ekrana bas (Echo) - Ýsteðe baðlý, genelde gets echo yapar
    push eax
    mov eax, 46
    mov bl, 2
    mov cl, [esp]
    int 40h
    pop eax
    
    jmp .loop
.done:
    mov byte [edx], 0    ; Null terminate
    mov eax, ebx         ; Orijinal buffer adresini döndür
    pop ebx
    ret