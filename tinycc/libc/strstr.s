.intel_syntax noprefix
.global _strstr
.global strstr
.text

_strstr:
strstr:
    push ebp
    mov ebp, esp
    push edi
    push esi
    push ebx

    mov edi, [ebp + 8]    /* edi = haystack (ana metin) */
    mov esi, [ebp + 12]   /* esi = needle (aranan kelime) */

    /* Aranan kelime boþ mu? (\0) */
    mov al, [esi]
    cmp al, 0
    jnz .L_main_loop
    mov eax, edi          /* Boþ dize ise doðrudan haystack adresini dön */
    jmp .L_done

.L_main_loop:
    mov al, [edi]
    cmp al, 0
    jz .L_null     /* Ana metin bittiyse ve bulunamadýysa NULL (0) dön */

    /* Ýç karþýlaþtýrma döngüsü baþlangýcý */
    mov ebx, edi          /* ebx = geçici haystack tarayýcý */
    mov edx, esi          /* edx = geçici needle tarayýcý */

.L_compare:
    mov cl, [edx]
    cmp cl, 0
    jz .L_match    /* Aranan kelimenin sonuna (\0) ulaþtýysak BAÞARILI! */
    
    mov ch, [ebx]
    cmp cl, ch
    jne .L_next    /* Karakterler eþleþmiyorsa ana döngüde bir saða kaydýr */
    
    inc ebx
    inc edx
    jmp .L_compare

.L_next:
    inc edi               /* Haystack iþaretçisini bir karakter ilerlet */
    jmp .L_main_loop

.L_match:
    mov eax, edi          /* Eþleþmenin baþladýðý ana adresi eax'e yükle */
    jmp .L_done

.L_null:
    xor eax, eax          /* Bulunamadý, NULL (0) döndür */

.L_done:
    pop ebx
    pop esi
    pop edi
    pop ebp
    ret
