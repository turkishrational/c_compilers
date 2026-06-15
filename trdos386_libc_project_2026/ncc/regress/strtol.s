.text
___strtoul:
L2:
	pushq %rbp
	movq %rsp,%rbp
	subq $40,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L137:
	movq %rdi,-8(%rbp)	 # spill
	movq %rsi,-32(%rbp)	 # spill
	movl %edx,-24(%rbp)	 # spill
	movl %ecx,%r12d
	movq -8(%rbp),%rsi	 # spill
	movl $0,-16(%rbp)	 # spill
	xorl %ecx,%ecx
	xorl %r14d,%r14d
	xorl %r9d,%r9d
L5:
	movzbl (%rsi),%eax
	addq $1,%rsi
	movl %eax,%edi
	movslq %eax,%rax
	movzbl ___ctype+1(%rax),%eax
	testl $8,%eax
	jnz L5
L134:
	cmpl $43,%edi
	jz L13
L135:
	cmpl $45,%edi
	jnz L10
L12:
	movl $1,-16(%rbp)	 # spill
L13:
	movzbl (%rsi),%edi
	addq $1,%rsi
L10:
	cmpl $0,-24(%rbp)	 # spill
	jnz L16
L14:
	cmpl $48,%edi
	jnz L18
L17:
	movzbl (%rsi),%eax
	cmpl $120,%eax
	jz L20
L23:
	cmpl $88,%eax
	jnz L21
L20:
	movl $16,%eax
	jmp L22
L21:
	movl $8,%eax
L22:
	movl %eax,-24(%rbp)	 # spill
	jmp L16
L18:
	leal -48(%rdi),%eax
	cmpl $10,%eax
	jae L28
L27:
	movl $10,-24(%rbp)	 # spill
	jmp L34
L28:
	movq -8(%rbp),%rsi	 # spill
	jmp L30
L16:
	cmpl $16,-24(%rbp)	 # spill
	jnz L34
L39:
	cmpl $48,%edi
	jnz L34
L35:
	movzbl (%rsi),%eax
	cmpl $120,%eax
	jz L32
L43:
	cmpl $88,%eax
	jnz L34
L32:
	movl $1,%r14d
	addq $1,%rsi
	movzbl (%rsi),%edi
	addq $1,%rsi
L34:
	movl -24(%rbp),%r10d	 # spill
	leal 48(%r10),%eax
	movl %eax,-40(%rbp)	 # spill
	movl -24(%rbp),%r10d	 # spill
	leal 55(%r10),%edx
	movl %edx,%r8d
	movl -24(%rbp),%r10d	 # spill
	leal 87(%r10),%r13d
	movl %r13d,%ebx
	leal -48(%rdi),%r15d
	cmpl $10,%r15d
	jae L54
L58:
	cmpl %eax,%edi
	jl L49
L54:
	leal -65(%rdi),%eax
	cmpl $26,%eax
	jae L50
L62:
	cmpl %edx,%edi
	jl L49
L50:
	leal -97(%rdi),%eax
	cmpl $26,%eax
	jae L47
L66:
	cmpl %r13d,%edi
	jge L47
L49:
	cmpl $0,%r12d
	jz L75
L74:
	movslq -24(%rbp),%r14	 # spill
	movq $-1,%rax
	xorl %edx,%edx
	divq %r14
	movq %rax,%r13
	movq $-1,%rax
	xorl %edx,%edx
	divq %r14
	movq %rdx,%rax
	jmp L133
L75:
	movslq -24(%rbp),%r14	 # spill
	movq $9223372036854775807,%rax
	cqto
	idivq %r14
	movq %rax,%r13
	movq $9223372036854775807,%rax
	cqto
	idivq %r14
	movq %rdx,%rax
L133:
	movslq -24(%rbp),%rdx	 # spill
L78:
	leal -48(%rdi),%r14d
	cmpl $10,%r14d
	jae L82
L84:
	cmpl -40(%rbp),%edi	 # spill
	jge L82
L81:
	addl $-48,%edi
	jmp L83
L82:
	leal -65(%rdi),%r14d
	cmpl $26,%r14d
	jae L89
L91:
	cmpl %r8d,%edi
	jge L89
L88:
	addl $-55,%edi
	jmp L83
L89:
	leal -97(%rdi),%r14d
	cmpl $26,%r14d
	jae L96
L98:
	cmpl %ebx,%edi
	jge L96
L95:
	addl $-87,%edi
L83:
	cmpq %r13,%r9
	jb L103
L106:
	cmpq %r13,%r9
	jnz L104
L110:
	movslq %edi,%r14
	cmpq %rax,%r14
	ja L104
L103:
	imulq %rdx,%r9
	movslq %edi,%rdi
	addq %r9,%rdi
	movq %rdi,%r9
	jmp L79
L104:
	addl $1,%ecx
L79:
	movzbl (%rsi),%edi
	addq $1,%rsi
	jmp L78
L96:
	addq $-1,%rsi
	jmp L30
L47:
	cmpl $0,%r14d
	jz L71
L70:
	addq $-2,%rsi
	jmp L30
L71:
	movq -8(%rbp),%rsi	 # spill
L30:
	cmpq $0,-32(%rbp)	 # spill
	jz L116
L114:
	movq -32(%rbp),%r10	 # spill
	movq %rsi,(%r10)
L116:
	cmpl $0,%ecx
	jz L119
L117:
	movl $34,_errno(%rip)
	cmpl $0,%r12d
	jz L122
L120:
	movq $-1,%rax
	jmp L4
L122:
	cmpl $0,-16(%rbp)	 # spill
	jz L125
L124:
	movq $-9223372036854775808,%rsi
	jmp L126
L125:
	movq $9223372036854775807,%rsi
L126:
	movq %rsi,%rax
	jmp L4
L119:
	cmpl $0,-16(%rbp)	 # spill
	jz L129
L128:
	movq %r9,%rsi
	negq %rsi
	jmp L130
L129:
	movq %r9,%rsi
L130:
	movq %rsi,%rax
L4:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L139:
_strtol:
L140:
	pushq %rbp
	movq %rsp,%rbp
L141:
	xorl %ecx,%ecx
	call ___strtoul
L142:
	popq %rbp
	ret
L147:
_strtoul:
L148:
	pushq %rbp
	movq %rsp,%rbp
L149:
	movl $1,%ecx
	call ___strtoul
L150:
	popq %rbp
	ret
L155:
.globl ___ctype
.globl _errno
.globl _strtoul
.globl _strtol
