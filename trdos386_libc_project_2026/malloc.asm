; void *calloc(size_t nmemb, size_t size)
global _calloc
; void *malloc(intptr_t increment)
global _malloc
extern _end             ; Baðlayýcý (linker) tarafýndan tanýmlanan program sonu

section .data
    current_brk: dd 0   ; Mevcut bellek sýnýrý

section .text
_calloc:
    mov	ecx, [esp+4]	; calloc yeni ayrýlan bellek kýsmýný sýfýrlar/temizler
    mov	edx, [ecx+8]	; size
    imul edx, ecx       ; EDX = Toplam byte (nmemb * size) -artýþ miktarý-
    jmp .alloc
_malloc:
    xor ecx, ecx	; malloc bellek içeriðini sýfýrlamaz
    mov edx, [esp+4]    ; artýþ miktarý
.alloc:
    mov eax, [current_brk]
    test eax, eax
    jnz .increase
    mov eax, _end       ; Ýlk çaðrýda program sonunu baz al
    mov [current_brk], eax

.increase:
    add edx, eax
    add	edx, 3		; round up (yukarý yuvarla) for dword alignment
    and	dl, ~3		
    cmp edx, esp	; eax ... edx ... esp
    jb .ok
    cmp eax, esp	; stack ile çakýþma kontrolü
    ja .ok		; esp ... eax ... edx
.fail:    		; eax ... esp ... edx
    xor eax, eax ; 0	; memory allocation error 
    ret    	 
.ok:
    ; Burada kernel'e "bana bellek ayýr" (sys_mem_alloc) çaðrýsý yapýlabilir
    ; veya demand paging sayesinde sadece adresi dönmek yetebilir.

    jecxz .done		; malloc
    ; calloc
.clear:
    mov	byte [eax], 0
    inc	eax
    cmp eax, edx
    jb .clear
    mov eax, [current_brk] 	
.done:
    mov [current_brk], edx ; yeni break adresi = eski+artýþ (dword aligned)
    ret                 ; EAX = eski break adresi (malloc için baþlangýç)