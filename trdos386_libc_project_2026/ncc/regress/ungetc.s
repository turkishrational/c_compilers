.text
_ungetc:
L1:
	pushq %rbp
	movq %rsp,%rbp
L21:
	movl %edi,%eax
	cmpl $-1,%eax
	jz L4
L7:
	movl 8(%rsi),%edi
	testl $128,%edi
	jnz L6
L4:
	movl $-1,%eax
	jmp L3
L6:
	movq 16(%rsi),%rcx
	movq 24(%rsi),%rdi
	cmpq %rdi,%rcx
	jnz L14
L12:
	movl (%rsi),%ecx
	cmpl $0,%ecx
	jz L17
L15:
	movl $-1,%eax
	jmp L3
L17:
	addq $1,%rdi
	movq %rdi,24(%rsi)
L14:
	addl $1,(%rsi)
	movq 24(%rsi),%rdi
	addq $-1,%rdi
	movq %rdi,24(%rsi)
	movb %al,(%rdi)
L3:
	popq %rbp
	ret
L23:
.globl _ungetc
