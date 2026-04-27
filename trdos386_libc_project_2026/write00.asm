;nasm

; int write(int fd, const void *buf, int count)
global _write
_write:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx

    mov ebx, [ebp+8]    ; fd
    mov ecx, [ebp+12]   ; buf
    mov edx, [ebp+16]   ; count

    cmp ebx, 3
    jb .use_stdio       ; fd < 3 (0, 1, 2) ise stdio simülasyonuna git

    ; Normal dosya yazma
    sub ebx, 3          ; C'deki 3 -> TRDOS'taki 0
    mov eax, 4          ; sys_write
    int 40h
    jmp .finish

.use_stdio:
    ; ebx zaten 0, 1 veya 2 (stdin, stdout, stderr)
    ; TRDOS 386 sys_stdio (eax=46) protokolüne göre:
    ; ebx=fonksiyon(read/write), ecx=buffer, edx=count
    ; Ancak sizin sys_stdio yapýnýza göre ebx'i ayarlamanýz gerekebilir
    mov eax, 46          ; sys_stdio
    ; ... buraya sys_stdio parametre yerleþimi gelecek ...
    int 40h

.finish:
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret
