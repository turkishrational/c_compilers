;nasm ; TRDOS 386 version 2.0.9 ve sonrasý için

; int read(int fd, void *buf, int count)

global _read
_read:
    ;push ebp
    ;mov ebp, esp
    ;push ebx
    ;push ecx
    ;push edx

    ;mov ebx, [ebp+8]   ; C-FD
    ;mov ecx, [ebp+12]  ; buffer
    mov ebx, [esp+4]	; C-FD
    mov ecx, [esp+8]	; buffer

    ; Stdio kontrolü
    cmp ebx, 3
    jb .read_stdio      ; FD < 3 ise sys_stdio (EAX=46) yönlendir

    ; Normal Dosya Okuma
    sub ebx, 3          ; TRDOS FD'sine çevir
    ;mov edx, [ebp+16]  ; count
    mov edx, [esp+12]   ; count
    mov eax, 3          ; sys_read
    int 40h
    
    jnc .done           ; CF=0 ise EAX (okunan byte veya 0) döndür
.fail:
    mov eax, -1         ; CF=1 ise sistem hatasýdýr, -1 döndür
    ;jmp .done
    ret

.read_stdio:
    ; Buraya EAX=46 üzerinden STDIN okuma döngüsü gelebilir
    ; (Karakter karakter okuyup buffer'a doldurur)

    cmp	bl, 1		; read iþlevi için 1 & 2 beklenmez ! 
    cmc	
    jc  short .fail
    xor edx, edx

.read_stdio_next:
    mov eax, 46
    ;mov bl, 0		; STDIN'dan bir karakter oku/al
    int 40h
    jc short .ok
    mov	[ecx], al
    and al, al
    jz .ok
    inc	edx
    ;cmp edx, [ebp+16]
    cmp edx, [esp+12]
    jnb short .ok
    inc	ecx
    cmp	al, 27		; ESC
    je .ok
    cmp al, 13		; CR/ENTER
    je .ok
    jmp	.read_stdio_next
.ok:
    mov	eax, edx	; okunan/alýnan karakter sayýsý
.done:
    ;pop edx
    ;pop ecx
    ;pop ebx
    ;pop ebp
    ret
