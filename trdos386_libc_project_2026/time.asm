;nasm

global _time
_time:
    mov eax, 13         ; systime
    mov ebx, 0          ; Unix format
    int 40h
    mov ecx, [esp+4]
    test ecx, ecx
    jz .done
    mov [ecx], eax
.done:
    ret
