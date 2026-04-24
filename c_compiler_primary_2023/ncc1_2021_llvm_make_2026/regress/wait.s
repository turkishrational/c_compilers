.text
_wait:
L1:
	pushq %rbp
	movq %rsp,%rbp
L6:
	movq %rdi,%rsi
	movl $-1,%edi
	xorl %edx,%edx
	call _waitpid
L3:
	popq %rbp
	ret
L8:
.globl _wait
.globl _waitpid
