.text
_strcpy:
L1:
	pushq %rbp
	movq %rsp,%rbp
L9:
	movq %rdi,%rax
L4:
	movzbl (%rsi),%ecx
	addq $1,%rsi
	movb %cl,(%rdi)
	movzbl %cl,%ecx
	addq $1,%rdi
	cmpl $0,%ecx
	jnz L4
L3:
	popq %rbp
	ret
L11:
.globl _strcpy
