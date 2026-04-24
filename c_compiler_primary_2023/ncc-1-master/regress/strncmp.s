.text
_strncmp:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	cmpq $0,%rdx
	jz L6
L7:
	movzbl (%rdi),%eax
	movzbl (%rsi),%ecx
	addq $1,%rsi
	cmpl %ecx,%eax
	jnz L8
L12:
	movzbl (%rdi),%eax
	addq $1,%rdi
	cmpl $0,%eax
	jnz L9
L14:
	xorl %eax,%eax
	jmp L3
L9:
	addq $-1,%rdx
	jnz L7
L8:
	cmpq $0,%rdx
	jz L6
L18:
	movzbl (%rdi),%eax
	cmpl $0,%eax
	jnz L23
L21:
	movl $-1,%eax
	jmp L3
L23:
	addq $-1,%rsi
	movzbl (%rsi),%eax
	cmpl $0,%eax
	jnz L27
L25:
	movl $1,%eax
	jmp L3
L27:
	movzbl (%rdi),%eax
	movzbl (%rsi),%esi
	subl %esi,%eax
	jmp L3
L6:
	xorl %eax,%eax
L3:
	popq %rbp
	ret
L34:
.globl _strncmp
