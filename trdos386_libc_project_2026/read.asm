;nasm

; int read(int fd, void *buf, int count)
global _read
_read:
    push ebx
    
    mov ebx, [esp+8]    ; fd
    mov ecx, [esp+12]   ; buffer
    mov edx, [esp+16]   ; count

    sub ebx, 3
    jnc .file_read      ; fd >= 3 ise dosya oku

    cmp	bl, 1
    cmc
    jc .fail		; STDIN numarasý (0) olmalýydý !

    ; --- STDIN (sys_stdio: bl=0 veya 1) ---
    ; fd=0 -> bl=0 (wait for key/string)
    ;mov ebx, 0

    xor edx, edx ; 0 		

    ; edx = count
.stdin_next: 	
    mov eax, 46         ; sys_stdio
    ; Not: Kernelin stdin string okuma davranýþýna göre 
    ; burada bir döngü veya tek seferlik çaðrý yapýlabilir.
    int 40h
    jc .fail
    mov	[ecx], al
    and al, al
    jz .ok
    inc	edx
    cmp edx, [esp+16]
    jnb .ok
    inc	ecx
    cmp	al, 27		; ESC
    je .ok
    cmp al, 13		; CR/ENTER
    je .ok
    jmp	.stdin_next

.file_read:
    mov eax, 3          ; sys_read
    int 40h
    jnc .done
.fail:
    mov eax, -1         ; Hata
    jmp .done
.ok:
    mov eax, edx	; read count	
.done:
    pop ebx
    ret
