;nasm

; void _printn(unsigned int n, int base)
global _printn
extern _putchar

_printn:
    push ebp
    mov ebp, esp
    push ebx

    mov eax, [ebp+8]    ; n
    mov ebx, [ebp+12]   ; base
    
    xor edx, edx
    div ebx             ; EAX = n / base, EDX = n % base
    
    test eax, eax
    jz .print_digit
    
    push ebx
    push eax
    call _printn        ; Recursive çaðrý
    add esp, 8

.print_digit:
    mov eax, edx
    cmp al, 10
    jb .decimal
    add al, 7           ; Hex 'A'-'F' için offset
.decimal:
    add al, '0'
    push eax
    call _putchar
    pop eax

    pop ebx
    pop ebp
    ret
