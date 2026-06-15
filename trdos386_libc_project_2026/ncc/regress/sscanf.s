.text
_sscanf:
L1:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
	pushq %r12
L6:
	movq 16(%rbp),%rdi
	leaq 32(%rbp),%rbx
	leaq -32(%rbp),%r12
	movl $-1,-28(%rbp)
	movl $133,-24(%rbp)
	movq %rdi,-16(%rbp)
	movq %rdi,-8(%rbp)
	call _strlen
	movl %eax,-32(%rbp)
	movq 24(%rbp),%rsi
	movq %r12,%rdi
	movq %rbx,%rdx
	call _vfscanf
L3:
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L8:
.globl _vfscanf
.globl _sscanf
.globl _strlen
