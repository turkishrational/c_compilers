.text
_strrchr:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	xorl %eax,%eax
	movzbl %sil,%esi
L4:
	movzbl (%rdi),%ecx
	cmpl %ecx,%esi
	jnz L6
L7:
	movq %rdi,%rax
L6:
	movzbl (%rdi),%ecx
	addq $1,%rdi
	cmpl $0,%ecx
	jnz L4
L3:
	popq %rbp
	ret
L14:
.globl _strrchr
