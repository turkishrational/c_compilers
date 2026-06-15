.text
_vsprintf:
L1:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
L9:
	movq %rdi,%rax
	leaq -32(%rbp),%rdi
	movl $-1,-28(%rbp)
	movl $262,-24(%rbp)
	movq %rax,-16(%rbp)
	movq %rax,-8(%rbp)
	movl $2147483647,-32(%rbp)
	call _vfprintf
	movl $1,-32(%rbp)
	movl $0,-32(%rbp)
	movq -8(%rbp),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,-8(%rbp)
	movb $0,(%rdi)
	movl %ebx,%eax
L3:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L11:
.globl _vsprintf
.globl _vfprintf
.globl ___flushbuf
