.text
_scanf:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	movq 16(%rbp),%rsi
	leaq 24(%rbp),%rdx
	movq $___stdin,%rdi
	call _vfscanf
L3:
	popq %rbp
	ret
L8:
.globl _vfscanf
.globl _scanf
.globl ___stdin
