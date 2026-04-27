;nasm

; void redirect_stdout(int fd)
global _redirect_stdout
_redirect_stdout:
    push ebp
    mov ebp, esp
    push ebx
    
    mov eax, 8          ; sys_stdio (TRDOS 386)
    mov bl, 5           ; BL = 5 (redirect stdout)
    mov ecx, [ebp+8]    ; FD
    sub ecx, 3		; 3-12 (C standardý) -> 0-9 (kernel)
    int 40h
    
    pop ebx
    pop ebp
    ret

