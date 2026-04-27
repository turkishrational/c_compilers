; nasm

; int wait(int *status)
global _wait
_wait:
    push ebx
    mov eax, 7		; syswait (TRDOS 386)
    int 40h
    pop ebx
    jc .fail
    mov	[esp+4], eax	; child's process ID
    ret 
.fail:
    mov eax, -1         ; TRDOS'ta child process bekleme mekanizmasý yoksa
    ret
