;nasm

section .data
global _errno
_errno: dd 0

section .text
; Yardýmcý fonksiyon: Sistem hatasýný errno'ya yazar ve -1 döner
__syscall_error:
    mov [_errno], eax
    mov eax, -1
    ret
