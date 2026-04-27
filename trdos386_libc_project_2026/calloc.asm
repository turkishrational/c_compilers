;nasm

; TRDOS 386 LIBC - calloc
; void *calloc(size_t nmemb, size_t size)

[BITS 32]
section .text
global _calloc
extern _end
extern current_brk

_calloc:		; calloc yeni ayrýlan bellek kýsmýný sýfýrlar/temizler
    mov eax, [current_brk]
    test eax, eax
    jnz .increase
    mov dword [current_brk], _end ; Ýlk çaðrýda program sonunu baz al
.increase:
    ; Burada kernel'e "bana bellek ayýr" (sys_mem_alloc) çaðrýsý yapýlabilir
    ; veya demand paging sayesinde sadece adresi dönmek yetebilir.
    mov  ecx, [esp+8]   ; size
    mov  edx, [esp+4]   ; nmemb
    imul ecx, edx       ; ECX = Toplam byte (nmemb * size)
.sbrk:			; sysbreak stack ile örtüþmeyen bellek kýsmýný temizler
    push ebx
    mov eax, 17         ; sys_break (TRDOS 386)
    mov ebx, ecx  	; Yeni break/snýr addresi
    add ebx, [current_brk]
    int 40h             ; Yeni sýnýr EAX'te döner
    pop ebx
    jnc .done
    ret
.done:
    xchg [current_brk], eax ; önceki break adresi eax'e yenisi þimdiki alanýna
    ret