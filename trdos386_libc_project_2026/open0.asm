;nasm

; int open(const char *path, int flags, int mode)
global _open
_open:
    push ebp
    mov ebp, esp
    push ebx
    push ecx

    mov ebx, [ebp+8]    ; path
    mov ecx, [ebp+12]   ; mode/flags
    
    mov eax, 5          ; sys_open (TRDOS 386)
    int 40h
    
    jnc .success        ; CF=0 ise hata yok
    mov eax, -1         ; Hata varsa -1 döndür
    jmp .done

.success:
    add eax, 3          ; 0-9 arasý dönen FD'yi C standardý için 3-12 yapýyoruz
.done:
    pop ecx
    pop ebx
    pop ebp
    ret
