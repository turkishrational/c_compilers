;nasm

; void* sbrk(int increment)

[BITS 32]
global _sbrk

section .data
    curr_brk: dd 0  ; Mevcut break adresi burada tutulur

section .text
_sbrk:
    push ebp
    mov ebp, esp
    push ebx

    ; Eðer ilk çaðrýysa mevcut break'i al (increment=0 ile çaðýrarak)
    mov eax, [curr_brk]
    test eax, eax
    jnz .do_inc
    
    ; Ýlk kez çaðrýlýyor, mevcut sýnýrý kernelden öðren
    mov eax, 17         ; sys_break
    mov ebx, 0          ; Mevcut sýnýrý döndür (TRDOS kuralý)
    int 40h
    mov [curr_brk], eax

.do_inc:
    mov ecx, [ebp+8]    ; artýþ miktarý (increment)
    test ecx, ecx
    jz .return_current  ; increment 0 ise sadece mevcut adresi dön

    mov ebx, [curr_brk]
    add ebx, ecx        ; Yeni hedef break adresi
    
    mov eax, 17         ; sys_break
    int 40h             ; Kernel break noktasýný ebx'e çeker
    
    cmp eax, -1         ; Hata kontrolü (örn. Stack'e çarptý mý?)
    je .error

    mov eax, [curr_brk] ; Eski break'i (yeni boþ alanýn baþý) döndür
    add [curr_brk], ecx ; Güncel sýnýrý kaydet
    jmp .done

.error:
    mov eax, -1
    jmp .done

.return_current:
    mov eax, [curr_brk]

.done:
    pop ebx
    pop ebp
    ret,