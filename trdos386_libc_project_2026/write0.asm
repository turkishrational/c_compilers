;nasm

; int write(int fd, void *buf, int count)
global _write
_write:
    push ebx
    mov ebx, [esp+8]    ; fd
    cmp ebx, 3
    jb .is_std          ; fd < 3 (0,1,2)

    ; Normal Dosya Yazma (EAX=4)
    sub ebx, 3          ; C FD -> TRDOS FD
    mov ecx, [esp+12]   ; buf
    mov edx, [esp+16]   ; count
    mov eax, 4          ; sys_write
    int 40h
    pop ebx
    ret

.is_std:
    ; Standart Çýkýþ Yönlendirme (EAX=7)
    ; TRDOS'ta BL=2 stdout, BL=3 stderr
    add ebx, 2          ; 1 (stdout) -> BL=3 mü? (Mimarinize göre ayarlayýn)
    ; ... (burada tampon döngüsüyle sys_stdio çaðrýlýr) ...
    pop ebx
    ret