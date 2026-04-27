;nasm

; TRDOS 386 - LIBC ASM WRAPPERS

[BITS 32]
global _putc, _getc, _open, _write, _sbrk, _exit

SECTION .text

; int putc(int c) -> EAX=46, BL=2
_putc:
    mov eax, 46
    mov bl, 2
    mov cl, [esp+4]
    int 40h
    movzx eax, cl
    ret

; int getc(void) -> EAX=46, BL=0
_getc:
    mov eax, 46
    mov bl, 0
    int 40h
    movzx eax, al
    ret

; int open(char *path, int mode) -> EAX=5, FD+3 Mapping
_open:
    mov eax, 5
    mov ebx, [esp+4]
    mov ecx, [esp+8]
    int 40h
    jc .err
    add eax, 3          ; C uyumluluðu için +3
    ret
.err:
    mov eax, -1
    ret

; void* sbrk(int inc) -> EAX=17 (sys_break)
_sbrk:
    mov eax, 17
    mov ebx, [esp+4]
    int 40h
    ret

; void exit(int code) -> EAX=1
_exit:
    mov eax, 1
    mov ebx, [esp+4]
    int 40h
    hlt                 ; Emniyet
