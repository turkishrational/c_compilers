.text
_strtof:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	call _strtod
	cvtsd2ss %xmm0,%xmm0
L3:
	popq %rbp
	ret
L8:
.globl _strtod
.globl _strtof
