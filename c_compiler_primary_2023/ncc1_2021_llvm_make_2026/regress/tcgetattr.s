.text
_tcgetattr:
L1:
	pushq %rbp
	movq %rsp,%rbp
L6:
	movq %rsi,%rdx
	movl $21505,%esi
	call _ioctl
L3:
	popq %rbp
	ret
L8:
.globl _ioctl
.globl _tcgetattr
