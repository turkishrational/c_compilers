.text
_setbuf:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	cmpq $0,%rsi
	jz L5
L4:
	xorl %edx,%edx
	jmp L6
L5:
	movl $4,%edx
L6:
	movl $1024,%ecx
	call _setvbuf
L3:
	popq %rbp
	ret
L10:
.globl _setvbuf
.globl _setbuf
