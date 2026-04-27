;nasm

; int fork(void* child_entry_point)
global _fork
_fork:
    push ebp
    mov ebp, esp
    push ebx

    mov eax, 2          ; sys_fork (TRDOS 386)
    mov ebx, [ebp+8]    ; Child sürecin devam edeceði adres (C'den gelen parametre)
    int 40h             ; Hem parent hem child'a PID döner
    
    pop ebx
    pop ebp
    ret
