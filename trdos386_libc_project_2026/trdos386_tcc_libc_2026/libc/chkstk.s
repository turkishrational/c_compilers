.intel_syntax noprefix
.global ___chkstk_ms
.text

___chkstk_ms:
    push ecx
    push eax
    cmp eax, 4096
    lea ecx, [esp + 12]
    jb .L_chkstk_less

.L_chkstk_loop:
    sub ecx, 4096
    test [ecx], eax
    sub eax, 4096
    cmp eax, 4096
    jae .L_chkstk_loop

.L_chkstk_less:
    sub ecx, eax
    test [ecx], eax
    pop eax
    pop ecx
    ret
