;nasm ; TRDOS 386 version 2.0.9 ve sonrasý için

; int write(int fd, void *buf, int count)
global _write
_write:
    ;push ebx
    ;mov ebx, [esp+8]   ; fd
    mov ebx, [esp+4]
    cmp ebx, 3
    jb .std_out         ; fd < 3 ise sys_stdio kullan

    ; Dosya Yazma
    sub ebx, 3          ; C-FD -> TRDOS-FD
    ;mov ecx, [esp+12]  ; buffer
    ;mov edx, [esp+16]  ; count
    mov ecx, [esp+8]
    mov edx, [esp+12]
    mov eax, 4          ; sys_write
    int 40h
    ;pop ebx
    ret

.fail:
    mov eax, -1
    ret

.std_out:
    ; Burada basit bir döngü ile buf içindeki karakterler 
    ; EAX=46, BL=2 (veya 3) üzerinden tek tek basýlabilir.
    ; (Hýzlý çözüm için kernelde string-stdio desteði yoksa bu yol izlenir)

    cmp	bl, 1		; write iþlevi için 1 (STDIN) & 2 (STDOUT) beklenir !
    jb  short .fail
    xor edx, edx
    inc	ebx ; 		; (write) sysstdio: bl=2 -> STDOUT, bl=3 -> STDERR

    push esi
    mov esi, [esp+8]
.std_out_next:
    lodsb
    mov	cl, al
    mov eax, 46
    ;mov bl, 2		; STDOUT'a (veya STDERR'a) bir karakter yaz/gönder
    int 40h
    jc short .ok
    inc edx
    cmp edx, [esp+12]
    jb .std_out_next
.ok:
    pop esi
    mov	eax, edx	; yazýlan karakter sayýsý
.done:
    ;pop ebx
    ret
