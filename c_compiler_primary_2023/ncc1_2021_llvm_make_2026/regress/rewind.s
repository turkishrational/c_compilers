.text
_rewind:
L1:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L5:
	movq %rdi,%rbx
	xorl %esi,%esi
	xorl %edx,%edx
	call _fseek
	andl $-49,8(%rbx)
L3:
	popq %rbx
	popq %rbp
	ret
L7:
.globl _rewind
.globl _fseek
