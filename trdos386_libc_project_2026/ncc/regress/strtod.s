.text
.align 8
L149:
	.quad 0x0
.align 8
L150:
	.quad 0xbff0000000000000
_strtod:
L1:
	pushq %rbp
	movq %rsp,%rbp
	subq $24,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movsd %xmm8,-24(%rbp)
L147:
	movq %rdi,%rax
	movq %rsi,-16(%rbp)	 # spill
	movq %rax,%rbx
	xorl %edi,%edi
	movl $0,-8(%rbp)	 # spill
	xorl %r15d,%r15d
	xorl %r12d,%r12d
	xorl %r14d,%r14d
	movsd L149(%rip),%xmm8
L4:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%r13d
	movslq %esi,%rsi
	movzbl ___ctype+1(%rsi),%esi
	testl $8,%esi
	jnz L4
L141:
	cmpl $43,%r13d
	jz L12
L142:
	cmpl $45,%r13d
	jnz L9
L11:
	movl $1,%r12d
L12:
	movzbl (%rbx),%r13d
	addq $1,%rbx
L9:
	leal -48(%r13),%esi
	cmpl $10,%esi
	jb L31
L16:
	cmpl $46,%r13d
	jnz L13
L24:
	movzbl (%rbx),%esi
	addl $-48,%esi
	cmpl $10,%esi
	jb L31
L13:
	movq %rax,%rbx
	jmp L28
L31:
	leal -48(%r13),%esi
	cmpl $10,%esi
	jae L35
L34:
	addl $-48,%r13d
	jnz L39
L40:
	testl $2,%r12d
	jz L39
L37:
	movq %rbx,%rsi
L44:
	movzbl (%rsi),%ecx
	addq $1,%rsi
	movl %ecx,%eax
	cmpl $48,%ecx
	jz L44
L47:
	addl $-48,%ecx
	cmpl $10,%ecx
	jb L39
L48:
	movq %rsi,%rbx
	movl %eax,%r13d
	jmp L33
L39:
	cmpl $0,-8(%rbp)	 # spill
	jnz L52
L55:
	cmpl $0,%r13d
	jz L54
L52:
	addl $1,-8(%rbp)	 # spill
L54:
	movq $1844674407370955160,%rsi
	cmpq %rsi,%r14
	jbe L60
L59:
	testl $8,%r12d
	jz L63
L62:
	call ___pow10
	mulsd %xmm0,%xmm8
	cvtsi2sdq %r14,%xmm0
	addsd %xmm0,%xmm8
	jmp L64
L63:
	cvtsi2sdq %r14,%xmm8
	orl $8,%r12d
L64:
	movl $1,%edi
	movslq %r13d,%r14
	jmp L61
L60:
	addl $1,%edi
	imulq $10,%r14
	movslq %r13d,%rsi
	addq %rsi,%r14
L61:
	testl $2,%r12d
	jz L32
L65:
	addl $-1,%r15d
	jmp L32
L35:
	cmpl $46,%r13d
	jnz L33
L71:
	testl $2,%r12d
	jnz L33
L68:
	orl $2,%r12d
L32:
	movzbl (%rbx),%r13d
	addq $1,%rbx
	jmp L31
L33:
	testl $8,%r12d
	jz L77
L76:
	call ___pow10
	mulsd %xmm0,%xmm8
	cvtsi2sdq %r14,%xmm0
	addsd %xmm0,%xmm8
	jmp L78
L77:
	cvtsi2sdq %r14,%xmm8
L78:
	cmpl $101,%r13d
	jz L79
L82:
	cmpl $69,%r13d
	jnz L81
L79:
	leaq -1(%rbx),%rsi
	movzbl (%rbx),%edi
	addq $1,%rbx
	movl %edi,%eax
	cmpl $43,%edi
	jz L91
L145:
	cmpl $45,%edi
	jnz L88
L90:
	orl $4,%r12d
L91:
	movzbl (%rbx),%eax
	addq $1,%rbx
L88:
	leal -48(%rax),%edi
	cmpl $10,%edi
	jb L94
L92:
	movq %rsi,%rbx
	jmp L28
L94:
	xorl %esi,%esi
	xorl %edi,%edi
L96:
	leal -48(%rax),%ecx
	cmpl $10,%ecx
	jae L99
L97:
	cmpl $0,%esi
	jnz L100
L103:
	cmpl $48,%eax
	jz L102
L100:
	addl $1,%esi
L102:
	imull $10,%edi
	leal -48(%rdi,%rax),%edi
	movzbl (%rbx),%eax
	addq $1,%rbx
	jmp L96
L99:
	cmpl $3,%esi
	jle L109
L107:
	testl $4,%r12d
	jz L111
L110:
	movl $32,%esi
	jmp L112
L111:
	movl $16,%esi
L112:
	orl %esi,%r12d
	addq $-1,%rbx
	jmp L28
L109:
	testl $4,%r12d
	jz L115
L114:
	subl %edi,%r15d
	jmp L81
L115:
	addl %edi,%r15d
L81:
	addq $-1,%rbx
	cmpl $-307,%r15d
	jg L118
L117:
	orl $32,%r12d
	jmp L28
L118:
	movl -8(%rbp),%r10d	 # spill
	leal -1(%r15,%r10),%esi
	cmpl $308,%esi
	jl L121
L120:
	orl $16,%r12d
	jmp L28
L121:
	cmpl $0,%r15d
	jz L28
L123:
	movl %r15d,%edi
	call ___pow10
	mulsd %xmm0,%xmm8
L28:
	cmpq $0,-16(%rbp)	 # spill
	jz L128
L126:
	movq -16(%rbp),%r10	 # spill
	movq %rbx,(%r10)
L128:
	testl $32,%r12d
	jz L131
L129:
	movl $34,_errno(%rip)
	movsd L149(%rip),%xmm0
	jmp L3
L131:
	testl $16,%r12d
	jz L135
L133:
	movl $34,_errno(%rip)
	movsd ___huge_val(%rip),%xmm8
L135:
	testl $1,%r12d
	jz L138
L136:
	mulsd L150(%rip),%xmm8
L138:
	movsd %xmm8,%xmm0
L3:
	movsd -24(%rbp),%xmm8
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L152:
.globl ___pow10
.globl ___ctype
.globl _errno
.globl _strtod
.globl ___huge_val
