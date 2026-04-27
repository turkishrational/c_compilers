;nasm

;TRDOS 386 demand paging yaptýðý için, sbrk aslýnda kernel'den
;fiziksel sayfa istemek yerine sadece prosesin "adres limitini"
;yukarý çekmekten ibarettir. Kernel, proses bu yeni adrese
;dokunduðunda (page fault) sayfayý atayacaktýr.

; void *sbrk(int increment)
global _sbrk
extern _end             ; Linker (LD) tarafýndan atanan veri segmenti sonu

section .data
    current_brk: dd 0

section .text
_sbrk:
    mov eax, [current_brk]
    test eax, eax
    jnz .do_inc
    mov eax, _end       ; Ýlk kullanýmda baþlangýç deðerini ata
    mov [current_brk], eax

.do_inc:
    mov edx, [esp+4]    ; increment
    add [current_brk], edx ; Limiti yükselt (demand paging halleder)
    ; EAX eski 'break' adresini tutar, malloc bunu kullanýr.
    ret