;nasm

; TRDOS 386 LIBC - _exit
global _exit
_exit:
    mov ebx, [esp+4]    ; Exit code
    mov eax, 1          ; sys_exit (trdosk6.s)
    int 40h
    hlt                 ; Tedbir amaçlý
