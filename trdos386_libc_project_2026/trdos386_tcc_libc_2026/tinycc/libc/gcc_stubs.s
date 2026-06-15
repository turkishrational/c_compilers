.intel_syntax noprefix
.global ___main
.global _stricmp
.global _strlwr
.text

___main:
    ret

_stricmp:
    xor eax, eax
    ret

_strlwr:
    mov eax, [esp + 4]
    ret
