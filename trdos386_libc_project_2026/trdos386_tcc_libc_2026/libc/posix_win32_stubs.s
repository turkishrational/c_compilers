.intel_syntax noprefix
.global _system
.global system
.global ___p__environ
.global _GetProcAddress@8
.global _LoadLibraryA@4
.global _FreeLibrary@4
.global _VirtualProtect@16
.global _SetUnhandledExceptionFilter@4
.global _SearchPathA@24

/* MSYS2 / POSIX Sembol Giriþleri */
.global ___getreent
.global __getreent
.global _strcasecmp
.global strcasecmp
.global _strtod
.global strtod
.global _puts
.global puts
.global _chmod
.global chmod
.global _realpath
.global realpath
.global _execvp
.global execvp
.global _gettimeofday
.global gettimeofday
.global _sem_init
.global _sem_wait
.global _sem_post

.global ___errno
/* .global __errno */

.text

/* =========================================================================
   1. NÝHAÝ POSIX REENTRANT VE GÖLGE ERRNO ZIRHI
   ========================================================================= */
/* Hem __getreent hem de __errno çaðrýlarýný tek bir güvenli bellek */
/* hüvviyetine (fake_reent_struct) baðlýyoruz. Linker adlandýrma çakýþmasýný */
/* ezmek için sembolü doðrudan montaj seviyesinde kilitliyoruz. */

___getreent:
__getreent:
    /* mov eax, offset fake_reent_struct */
    /* ret */ 

/* =========================================================================
   2. ÜÇ ALT ÇÝZGÝLÝ GÖLGE __errno ENJEKSÝYONU
   ========================================================================= */
/* Windows ld.exe baðlayýcýsýnýn isimlendirme hilesini aþmak için buraya */
/* tam 3 alt çizgili etiket çakýyoruz. ld.exe bunu tcc.o'nun beklediði */
/* iki alt çizgili '__errno' olarak haritalayacaktýr. */
/* Kendi içimizdeki baðýmsýz bellek havuzunu döndüðü için çakýþma ÝMKANSIZDIR. */
___errno:
__errno:
    mov eax, offset fake_reent_struct /* Hata numarasý bu bloðun ilk dword'üne yazýlsýn */
    ret

/* 2. strcasecmp - Büyük küçük harf duyarsýz string karþýlaþtýrma */
_strcasecmp:
strcasecmp:
    push ebp
    mov ebp, esp
    push edi
    push esi
    push ebx
    mov esi, [ebp + 8]   /* string 1 */
    mov edi, [ebp + 12]  /* string 2 */
.L_scc_loop:
    mov al, [esi]
    mov bl, [edi]
    cmp al, 65
    jl .L_skip_al
    cmp al, 90
    jg .L_skip_al
    add al, 32
.L_skip_al:
    cmp bl, 65
    jl .L_skip_bl
    cmp bl, 90
    jg .L_skip_bl
    add bl, 32
.L_skip_bl:
    cmp al, bl
    jne .L_scc_diff
    cmp al, 0
    je .L_scc_same
    inc esi
    inc edi
    jmp .L_scc_loop
.L_scc_diff:
    sub al, bl
    movsx eax, al
    jmp .L_scc_done
.L_scc_same:
    xor eax, eax
.L_scc_done:
    pop ebx
    pop esi
    pop edi
    pop ebp
    ret

/* 3. strtod - String to Double Çeviri Bypass'ý */
_strtod:
strtod:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]   /* str nptr */
    mov ecx, [ebp + 12]  /* endptr */
    cmp ecx, 0
    jz .L_strtod_skip
    mov [ecx], eax
.L_strtod_skip:
    fldz
    pop ebp
    ret

/* 4. puts - Yerleþik kütüphane _strlen ve _write üzerinden Konsola Ýtme */
_puts:
puts:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp + 8]
    test ebx, ebx
    jz .L_puts_done

    push ebx
    .extern _strlen
    call _strlen
    add esp, 4
    test eax, eax
    jz .L_puts_lf

    push eax
    push ebx
    push 1
    .extern _write
    call _write
    add esp, 12

.L_puts_lf:
    mov eax, offset local_lf_char
    push 1
    push eax
    push 1
    call _write
    add esp, 12

.L_puts_done:
    xor eax, eax
    pop ebx
    pop ebp
    ret

/* 5. Boþ / Pasif Dönüþ Hücreleri */
_system:
system:
    mov eax, -1
    ret

___p__environ:
_GetProcAddress@8:
_LoadLibraryA@4:
_FreeLibrary@4:
_VirtualProtect@16:
_SetUnhandledExceptionFilter@4:
_SearchPathA@24:
_chmod:
chmod:
_realpath:
realpath:
_execvp:
execvp:
_gettimeofday:
gettimeofday:
_sem_init:
_sem_wait:
_sem_post:
    xor eax, eax
    ret

.data
.align 4
fake_reent_struct:    .space 256, 0  /* Geniþletilmiþ reentrant ve hata kayýt havuzu */
local_lf_char:        .byte 10

