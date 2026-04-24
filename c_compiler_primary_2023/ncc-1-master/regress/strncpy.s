.text
_strncpy:
L1:
	pushq %rbp
	movq %rsp,%rbp
L26:
	movq %rdi,%rax
	cmpq $0,%rdx
	jz L3
L7:
	movzbl (%rsi),%ecx
	addq $1,%rsi
	movb %cl,(%rdi)
	movzbl %cl,%ecx
	addq $1,%rdi
	cmpl $0,%ecx
	jz L9
L10:
	addq $-1,%rdx
	jnz L7
L9:
	addq $-1,%rsi
	movzbl (%rsi),%esi
	cmpl $0,%esi
	jnz L3
L17:
	addq $-1,%rdx
	jz L3
L21:
	movb $0,(%rdi)
	addq $1,%rdi
	addq $-1,%rdx
	jnz L21
L3:
	popq %rbp
	ret
L28:
.globl _strncpy
