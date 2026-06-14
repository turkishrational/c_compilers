; ****************************************************************************
; trdos386_libc_io.s - TRDOS 386 LIBC 2026 Minimalist I/O Layer
; ****************************************************************************

global _sys_open
global _sys_read
global _sys_write
global _sys_close

[BITS 32]

; ----------------------------------------------------------------------------
; int sys_open(const char *path, int flags, int mode);
; C'ye dönüþ: Baþarýlý ise 3-12 arasý handle, Hata ise -1
; ----------------------------------------------------------------------------
_sys_open:
    push    ebp
    mov     ebp, esp
    push    ebx
    push    ecx

    mov     ebx, [ebp+8]    ; path (dosya yolu karakter dizisi)
    mov     ecx, [ebp+12]   ; flags (açma modlarý)
    ; TRDOS 386 sys_open çaðrýsý (EAX = 5)
    mov     eax, 5          
    int     40h
    jc      .open_error     ; Carry flag set ise hata oluþmuþtur

    ; Kernel normalde 0-9 arasý döndürür. Talimat gereði +3 ekliyoruz!
    add     eax, 3          
    jmp     .open_ok

.open_error:
    mov     eax, -1         ; Hata durumunda C standardý -1 döner
.open_ok:
    pop     ecx
    pop     ebx
    pop     ebp
    retn

; ----------------------------------------------------------------------------
; int sys_write(int fd, const void *buf, unsigned int count);
; ----------------------------------------------------------------------------
_sys_write:
    push    ebp
    mov     ebp, esp
    push    ebx
    push    ecx
    push    edx

    mov     ebx, [ebp+8]    ; fd (File Descriptor)
    mov     ecx, [ebp+12]   ; buf (Arabellek adresi)
    mov     edx, [ebp+16]   ; count (Yazýlacak byte sayýsý)

    ; KURAL KONTROLÜ: Standart çýktý veya hata akýþý mý?
    cmp     ebx, 0
    je      .use_sysstdio
    cmp     ebx, 1
    je      .use_sysstdio
    cmp     ebx, 2
    je      .use_sysstdio

    ; Normal Dosya: fd deðerinden 3 çýkarýp kernel handle deðerini (0-9) buluyoruz
    sub     ebx, 3          
    mov     eax, 4          ; sys_write (EAX = 4)
    int     40h
    jc      .write_error
    jmp     .write_ok

.use_sysstdio:
    ; fd = 0, 1 veya 2 ise sysstdio (EAX = 46) çaðrýsý tetiklenir
    ; TRDOS 386 sysstdio mimarisine göre ebx, ecx, edx ayarlanýr
    mov     eax, 46         ; sysstdio çaðrýsý
    int     40h
    jc      .write_error
    jmp     .write_ok

.write_error:
    mov     eax, -1
.write_ok:
    pop     edx
    pop     ecx
    pop     ebx
    pop     ebp
    retn

; ----------------------------------------------------------------------------
; int sys_read(int fd, void *buf, unsigned int count);
; ----------------------------------------------------------------------------
_sys_read:
    push    ebp
    mov     ebp, esp
    push    ebx
    push    ecx
    push    edx

    mov     ebx, [ebp+8]    ; fd
    mov     ecx, [ebp+12]   ; buf
    mov     edx, [ebp+16]   ; count

    cmp     ebx, 2
    jbe     .read_sysstdio  ; 0, 1, 2 ise sysstdio kullan

    sub     ebx, 3          ; Düzenli dosya dönüþümü (3-12 -> 0-9)
    mov     eax, 3          ; sys_read (EAX = 3)
    int     40h
    jc      .read_error
    jmp     .read_ok

.read_sysstdio:
    mov     eax, 46         ; sysstdio (EAX = 46)
    int     40h
    jc      .read_error
    jmp     .read_ok

.read_error:
    mov     eax, -1
.read_ok:
    pop     edx
    pop     ecx
    pop     ebx
    pop     ebp
    retn

; ----------------------------------------------------------------------------
; int sys_close(int fd);
; ----------------------------------------------------------------------------
_sys_close:
    push    ebp
    mov     ebp, esp
    push    ebx

    mov     ebx, [ebp+8]    ; fd

    cmp     ebx, 2
    jbe     .close_skip     ; Standart akýþlar kapatýlamaz, atla

    sub     ebx, 3          ; 3-12 arasý fd'yi 0-9 arasýna indir
    mov     eax, 6          ; sys_close (EAX = 6)
    int     40h
    jc      .close_error
    mov     eax, 0
    jmp     .close_ok

.close_skip:
    mov     eax, 0
    jmp     .close_ok
.close_error:
    mov     eax, -1
.close_ok:
    pop     ebx
    pop     ebp
    retn