	.text
	.globl	Cmain
Cmain:	pushl	%ebp
	movl	%esp,%ebp
	addl	$-16,%esp
	movl	$1,%eax
	negl	%eax
	movl	%eax,-4(%ebp)
	movl	$0,%eax
	movl	%eax,-8(%ebp)
	movl	$65536,%eax
	movl	%eax,-12(%ebp)
	movl	$1000000,%eax
	movl	%eax,-16(%ebp)
	movl	-4(%ebp),%eax
	pushl	%eax
	movl	-12(%ebp),%eax
	popl	%ecx
	imull	%ecx,%eax
	movl	%eax,-8(%ebp)
	.data
L2:	.byte	'H'
	.byte	'e'
	.byte	'l'
	.byte	'l'
	.byte	'o'
	.byte	44
	.byte	32
	.byte	'W'
	.byte	'o'
	.byte	'r'
	.byte	'l'
	.byte	'd'
	.byte	33
	.byte	10
	.byte	0
	.text
	movl	$L2,%eax
	pushl	%eax
	pushl	$1
	call	Cprintf
	addl	$8,%esp
L1:	addl	$16,%esp
	popl	%ebp
	ret
