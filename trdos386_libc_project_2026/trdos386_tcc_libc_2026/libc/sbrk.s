/* ===========================================================================
   TRDOS 386 - TEKÝL VE GÜVENLÝ SBRK MOTORU (sbrk.s)
   =========================================================================== */
.intel_syntax noprefix
.global _sbrk
.extern _end       /* Linker'dan gelen BSS bitiþ koordinatý */

.text
_sbrk:
    push ebp
    mov ebp, esp
    push ebx
    push ecx

    mov ecx, [ebp + 8]    /* Ýstenen byte boyutu (size) */
    
    /* DWORD Hizalamasý (Her ihtimale karþý sbrk seviyesinde zýrh) */
    add ecx, 3
    and ecx, 0xFFFFFFFC

    /* Ýlk çaðrý kontrolü */
    mov eax, [current_break]
    test eax, eax
    jnz .L_calc_new
    
    /* Ýlk çaðrýda taban adresi _end olarak mühürleniyor */
    mov eax, offset _end
    add eax, 3
    and eax, 0xFFFFFFFC
    mov [current_break], eax

.L_calc_new:
    mov ebx, [current_break] /* EBX = Eski u.break (Geri dönecek olan geçerli adres) */
    mov edx, ebx
    add edx, ecx           /* EDX = Yeni u.break sýnýrý */

    /* TRDOS Kernel sys_break (17) Çaðrýsý */
    push ebx
    mov ebx, edx
    mov eax, 17
    int 0x40
    pop ebx
    
    /* Hata Kontrolü (Carry Flag veya EAX < 0 kontrolü yapýlabilir) */
    cmp eax, 0
    jl .L_err

    /* Baþarýlý ise global göstergeyi güncelle */
    mov [current_break], edx

    /* Demand Paging & Zero-Fill Zýrhý: Yeni alaný el ile tetikle ve temizle */
    push edi
    mov edi, ebx           /* Baþlangýç adresi */
    shr ecx, 2             /* DWORD sayýsýna böl (ecx / 4) */
    xor eax, eax           /* Yazýlacak deðer: 0 */
    rep stosd              /* Tüm yeni alaný sýfýrla ve sayfalarý Ring 3'e baðla */
    pop edi

    mov eax, ebx           /* Eski break adresini (tahsis edilen yerin baþý) döndür */
    jmp .L_done

.L_err:
    xor eax, eax           /* Bellek bittiyse NULL (0) dön */

.L_done:
    pop ecx
    pop ebx
    pop ebp
    ret

current_break: .long 0  /* Tüm sistemin tekil u.break izleyicisi */
