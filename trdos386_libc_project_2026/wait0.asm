; nasm

; int wait(int *status)
global _wait
_wait:
    mov eax, -1         ; TRDOS'ta child process bekleme mekanizmasý yoksa
    ret
