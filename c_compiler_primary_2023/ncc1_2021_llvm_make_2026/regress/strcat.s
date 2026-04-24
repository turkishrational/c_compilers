.text
_strcat:
L1:
	pushq %rbp
	movq %rsp,%rbp
L12:
	movq %rdi,%rax
L4:
	movzbl (%rdi),%ecx
	addq $1,%rdi
	cmpl $0,%ecx
	jnz L4
L6:
	addq $-1,%rdi
L7:
	movzbl (%rsi),%ecx
	addq $1,%rsi
	movb %cl,(%rdi)
	movzbl %cl,%ecx
	addq $1,%rdi
	cmpl $0,%ecx
	jnz L7
L3:
	popq %rbp
	ret
L14:
.globl _strcat
