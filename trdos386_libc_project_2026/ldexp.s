.intel_syntax noprefix
.global _ldexp
.global ldexp
.text

_ldexp:
ldexp:
    push ebp
    mov ebp, esp
    
    fild dword ptr [ebp + 16]    /* Üssü (exp) integer olarak FPU yýðýnýna yükle */
    fld qword ptr [ebp + 8]      /* Mantis'i (x) double olarak FPU yýðýnýna yükle */
    fscale                       /* ST(0) = ST(0) * 2^ST(1) iþlemini çak */
    fstp st(1)                   /* Üs deðerini FPU'dan temizle, sonuç st(0)'da kalýr */
    
    pop ebp
    ret
