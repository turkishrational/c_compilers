;nasm

; TRDOS 386 LIBC - Smart Open/Creat Logic
; int open(const char *path, int flags, int mode)

global _open
_open:
    push ebx
    push esi
    
    mov ebx, [esp+12]   ; path
    mov esi, [esp+16]   ; C flags (0=R, 1=W, 2=RW, + 0x40=CREAT, + 0x200=TRUNC)
    
    ; 1. Niyet: Sadece Okuma mý? (O_RDONLY = 0)
    test esi, esi       ; Flags 0 mý?
    jz .do_sys_open     ; Sadece okuma ise direkt sysopen(read)
    
    ; 2. Niyet: Yazma var ama Truncate (Sýfýrlama) isteniyor mu?
    ; C'de "w" modu O_WRONLY | O_CREAT | O_TRUNC (0x241) gönderir.
    test esi, 0x200     ; O_TRUNC (512) seti mi?
    jnz .do_sys_creat   ; Varsa direkt syscreat (Truncate özelliði için)
    
    ; 3. Niyet: Yazma var ama Truncate istenmiyor (O_APPEND veya mevcut datayý deðiþtirme)
    ; Bu durumda dosya yoksa hata almamak için O_CREAT kontrolü yapýlýr.
    test esi, 0x40      ; O_CREAT (64) seti mi?
    jz .do_sys_open_write ; CREAT yoksa sadece mevcut dosyayý yazma modunda açmayý dene

    ; 4. Karma Niyet: Hem oluþtur hem mevcutsa bozma (O_CREAT | O_EXCL gibi durumlar için)
    ; Önce sysopen dene, dosya bulunamazsa syscreat'e geç.
    mov eax, 5          ; sys_open
    mov ecx, 1          ; write mode
    int 40h
    jnc .success_fd     ; Dosya varsa ve açýldýysa devam et
    
    cmp eax, 2          ; Hata: Dosya bulunamadý mý? (TRDOS error 2)
    je .do_sys_creat    ; Bulunamadýysa oluþtur
    jmp .err            ; Baþka bir hataysa (örn: Access Error) dur ve dön.

.do_sys_creat:
    mov eax, 8          ; sys_creat (Truncate/Create)
    mov ecx, 0          ; attribute: normal
    int 40h
    jmp .handle_res

.do_sys_open_write:
    mov eax, 5          ; sys_open
    mov ecx, 1          ; write mode
    int 40h
    jmp .handle_res

.do_sys_open:
    mov eax, 5          ; sys_open
    mov ecx, 0          ; read mode
    int 40h

.handle_res:
    jc .err
.success_fd:
    add eax, 3          ; C-FD (0->3)
    jmp .done

.err:
    mov eax, -1
.done:
    pop esi
    pop ebx
    ret
