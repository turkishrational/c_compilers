.text
_setvbuf:
L1:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L49:
	movl %edx,%ebx
	movq %rcx,%r13
	movq %rdi,%r15
	movq %rsi,%r12
	xorl %r14d,%r14d
	movq $___stdio_cleanup,___exit_cleanup(%rip)
	cmpl $0,%ebx
	jz L6
L11:
	cmpl $64,%ebx
	jz L6
L7:
	cmpl $4,%ebx
	jz L6
L4:
	movl $-1,%eax
	jmp L3
L6:
	movq 16(%r15),%rdi
	cmpq $0,%rdi
	jz L18
L19:
	movl 8(%r15),%esi
	testl $8,%esi
	jz L18
L16:
	call _free
L18:
	andl $-77,8(%r15)
	cmpq $0,%r12
	jz L25
L26:
	cmpq $0,%r13
	jnz L25
L23:
	movl $-1,%r14d
L25:
	cmpq $0,%r12
	jnz L32
L33:
	cmpl $4,%ebx
	jz L32
L30:
	cmpq $0,%r13
	jz L37
L40:
	movq %r13,%rdi
	call _malloc
	movq %rax,%r12
	cmpq $0,%rax
	jnz L38
L37:
	movl $-1,%r14d
	jmp L32
L38:
	orl $8,8(%r15)
L32:
	movq %r12,16(%r15)
	movl $0,(%r15)
	orl %ebx,8(%r15)
	movq 16(%r15),%rsi
	movq %rsi,24(%r15)
	cmpq $0,%r12
	jnz L45
L44:
	movl $1,12(%r15)
	jmp L46
L45:
	movl %r13d,12(%r15)
L46:
	movl %r14d,%eax
L3:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L51:
.globl ___stdio_cleanup
.globl ___exit_cleanup
.globl _malloc
.globl _free
.globl _setvbuf
