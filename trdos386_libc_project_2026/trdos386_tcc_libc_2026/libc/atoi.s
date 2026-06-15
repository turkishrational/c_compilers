.intel_syntax noprefix
.global _atoi
.text

_atoi:
    push ebp
    mov ebp, esp
    push esi

    mov esi, [ebp + 8]    /* Parameter 1: str pointer address */
    xor eax, eax          /* Initialize result = 0 */
    xor ecx, ecx          /* Sign tracker: 0 = positive, 1 = negative */

.L_skip_ws:
    mov dl, [esi]
    cmp dl, 32            /* Space character */
    je .L_next_ws
    cmp dl, 9             /* Tab character */
    jl .L_check_sign
    cmp dl, 13            /* CR / Newline boundaries */
    jg .L_check_sign
.L_next_ws:
    inc esi
    jmp .L_skip_ws

.L_check_sign:
    cmp dl, 45            /* Minus sign '-' */
    jne .L_check_plus
    inc ecx               /* Toggle negative sign indicator */
    inc esi
    jmp .L_convert_loop
.L_check_plus:
    cmp dl, 43            /* Plus sign '+' */
    jne .L_convert_loop
    inc esi

.L_convert_loop:
    mov dl, [esi]
    cmp dl, 48            /* Less than ASCII '0' */
    jl .L_atoi_done
    cmp dl, 57            /* Greater than ASCII '9' */
    jg .L_atoi_done

    sub dl, 48            /* Convert character to numeric digit value */
    imul eax, 10
    movzx edx, dl
    add eax, edx
    inc esi
    jmp .L_convert_loop

.L_atoi_done:
    test ecx, ecx
    jz .L_atoi_exit
    neg eax               /* Negate result if sign tracker ecx is active */

.L_atoi_exit:
    pop esi
    pop ebp
    ret
