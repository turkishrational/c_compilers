.data
.align 8
_weekdays:
	.quad L2
	.quad L3
	.quad L4
	.quad L5
	.quad L6
	.quad L7
	.quad L8
.align 8
_months:
	.quad L10
	.quad L11
	.quad L12
	.quad L13
	.quad L14
	.quad L15
	.quad L16
	.quad L17
	.quad L18
	.quad L19
	.quad L20
	.quad L21
.local L26
.comm L26, 6, 1
.text
_toasc:
L23:
	pushq %rbp
	movq %rsp,%rbp
L24:
	movq $L26+5,%rcx
	movb $0,L26+5(%rip)
L27:
	movl %esi,%eax
	addl $-1,%esi
	cmpl $0,%eax
	jz L29
L28:
	movl $10,%r8d
	movl %edi,%eax
	cltd
	idivl %r8d
	leal 48(%rdx),%eax
	addq $-1,%rcx
	movb %al,(%rcx)
	movl $10,%r8d
	movl %edi,%eax
	cltd
	idivl %r8d
	movl %eax,%edi
	jmp L27
L29:
	movq %rcx,%rax
L25:
	popq %rbp
	ret
L34:
.local L39
.comm L39, 8, 1
_convert:
L36:
	pushq %rbp
	movq %rsp,%rbp
L42:
	movl %edx,%r8d
	movl $10,%r9d
	movl %edi,%eax
	cltd
	idivl %r9d
	addl $48,%eax
	movb %al,L39(%rip)
	movl $10,%r9d
	movl %edi,%eax
	cltd
	idivl %r9d
	leal 48(%rdx),%edi
	movb %dil,L39+1(%rip)
	movb %cl,L39+2(%rip)
	movl $10,%edi
	movl %esi,%eax
	cltd
	idivl %edi
	leal 48(%rax),%edi
	movb %dil,L39+3(%rip)
	movl $10,%edi
	movl %esi,%eax
	cltd
	idivl %edi
	leal 48(%rdx),%esi
	movb %sil,L39+4(%rip)
	movb %cl,L39+5(%rip)
	movl $10,%esi
	movl %r8d,%eax
	cltd
	idivl %esi
	leal 48(%rax),%esi
	movb %sil,L39+6(%rip)
	movl $10,%esi
	movl %r8d,%eax
	cltd
	idivl %esi
	leal 48(%rdx),%esi
	movb %sil,L39+7(%rip)
	movq $L39,%rax
L38:
	popq %rbp
	ret
L44:
_strftime:
L45:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L182:
	movq %rdx,-24(%rbp)	 # spill
	movq %rsi,-32(%rbp)	 # spill
	movq %rcx,%r12
	movq %rdi,%r15
	xorl %r10d,%r10d
	movq %r10,-16(%rbp)	 # spill
L49:
	movq -24(%rbp),%r10	 # spill
	movzbl (%r10),%esi
	addq $1,-24(%rbp)	 # spill
	movb %sil,-8(%rbp)
	movzbl %sil,%edi
	cmpl $37,%edi
	jz L53
L52:
	addq $1,-16(%rbp)	 # spill
	movq -16(%rbp),%r10	 # spill
	cmpq -32(%rbp),%r10	 # spill
	jbe L57
L55:
	xorl %eax,%eax
	jmp L47
L57:
	movb %sil,(%r15)
	movzbl -8(%rbp),%esi
	addq $1,%r15
	cmpl $0,%esi
	jnz L49
L59:
	addq $-1,-16(%rbp)	 # spill
	movq -16(%rbp),%rax	 # spill
	jmp L47
L53:
	movq -24(%rbp),%r10	 # spill
	movzbl (%r10),%esi
	movb %sil,-8(%rbp)
	movzbl %sil,%esi
	addq $1,-24(%rbp)	 # spill
	cmpl $97,%esi
	jz L68
	jb L138
L161:
	cmpl $112,%esi
	jz L96
	jb L162
L173:
	cmpl $121,%esi
	jz L118
	jb L174
L179:
	cmpl $122,%esi
	jnz L64
L122:
	movl 32(%r12),%esi
	cmpl $1,%esi
	jnz L124
L123:
	movl $1,%esi
	jmp L125
L124:
	xorl %esi,%esi
L125:
	movslq %esi,%rsi
	movq _tzname(,%rsi,8),%rsi
	movq %rsi,%rdi
	movq %rdi,%r14
	movq %rsi,%rdi
	call _strlen
	movq %rax,%rbx
	jmp L65
L174:
	cmpl $120,%esi
	jz L114
	ja L64
L175:
	cmpl $119,%esi
	jnz L64
L107:
	movl 28(%r12),%esi
	leal 8(%rsi),%eax
	movl 24(%r12),%esi
	subl %esi,%eax
	movl $7,%edi
	cltd
	idivl %edi
	movl %eax,%r13d
	cmpl $0,%esi
	jnz L110
L108:
	leal -1(%rax),%r13d
L110:
	movl $2,%ebx
	jmp L129
L114:
	movl 20(%r12),%eax
	movl 12(%r12),%esi
	movl 16(%r12),%edi
	addl $1,%edi
	movl %eax,%edx
	movl $47,%ecx
	call _convert
	movq %rax,%r14
	movl $8,%ebx
	jmp L65
L118:
	movl 20(%r12),%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L162:
	cmpl $100,%esi
	jz L81
	jb L163
L168:
	cmpl $109,%esi
	jz L92
	ja L64
L169:
	cmpl $106,%esi
	jnz L64
L90:
	movl 28(%r12),%esi
	addl $1,%esi
	movl %esi,%r13d
	movl $3,%ebx
	jmp L129
L92:
	movl 16(%r12),%esi
	addl $1,%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L163:
	cmpl $99,%esi
	jz L79
	ja L64
L164:
	cmpl $98,%esi
	jz L74
	jnz L64
L79:
	movq %r12,%rdi
	call _asctime
	movq %rax,%r14
	movl $24,%ebx
	jmp L65
L81:
	movl 12(%r12),%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L96:
	movl 8(%r12),%esi
	cmpl $12,%esi
	jge L100
L99:
	movq $L97,%rsi
	jmp L101
L100:
	movq $L98,%rsi
L101:
	movq %rsi,%r14
	movl $2,%ebx
	jmp L65
L138:
	cmpl $77,%esi
	jz L94
	jb L139
L150:
	cmpl $87,%esi
	jz L112
	jb L151
L156:
	cmpl $89,%esi
	jz L120
	ja L64
L157:
	cmpl $88,%esi
	jnz L64
L116:
	movl (%r12),%eax
	movl 4(%r12),%esi
	movl 8(%r12),%edi
	movl %eax,%edx
	movl $58,%ecx
	call _convert
	movq %rax,%r14
	movl $8,%ebx
	jmp L65
L120:
	movl 20(%r12),%esi
	addl $1900,%esi
	movl %esi,%r13d
	movl $4,%ebx
	jmp L129
L151:
	cmpl $85,%esi
	jz L105
	ja L64
L152:
	cmpl $83,%esi
	jnz L64
L103:
	movl (%r12),%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L105:
	movl 28(%r12),%esi
	leal 7(%rsi),%eax
	movl 24(%r12),%esi
	subl %esi,%eax
	movl $7,%esi
	cltd
	idivl %esi
	movl %eax,%r13d
	movl $2,%ebx
	jmp L129
L112:
	movl 24(%r12),%esi
	movl %esi,%r13d
	movl $1,%ebx
	jmp L129
L139:
	cmpl $66,%esi
	jz L74
	jb L140
L145:
	cmpl $73,%esi
	jz L85
	ja L64
L146:
	cmpl $72,%esi
	jnz L64
L83:
	movl 8(%r12),%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L85:
	movl 8(%r12),%eax
	movl $12,%esi
	cltd
	idivl %esi
	movl %edx,%esi
	movl %esi,%r13d
	cmpl $0,%esi
	jnz L88
L86:
	movl $12,%r13d
L88:
	movl $2,%ebx
	jmp L129
L140:
	cmpl $65,%esi
	jz L68
	ja L64
L141:
	cmpl $37,%esi
L64:
	leaq -8(%rbp),%rsi
	movq %rsi,%r14
	movl $1,%ebx
	jmp L65
L74:
	movl 16(%r12),%esi
	movslq %esi,%rsi
	movq _months(,%rsi,8),%rsi
	movq %rsi,%r14
	movzbl -8(%rbp),%edi
	cmpl $98,%edi
	jnz L76
L75:
	movl $3,%eax
	jmp L77
L76:
	movq %rsi,%rdi
	call _strlen
L77:
	movq %rax,%rbx
	jmp L65
L94:
	movl 4(%r12),%esi
	movl %esi,%r13d
	movl $2,%ebx
	jmp L129
L68:
	movl 24(%r12),%esi
	movslq %esi,%rsi
	movq _weekdays(,%rsi,8),%rsi
	movq %rsi,%r14
	movzbl -8(%rbp),%edi
	cmpl $97,%edi
	jnz L70
L69:
	movl $3,%eax
	jmp L71
L70:
	movq %rsi,%rdi
	call _strlen
L71:
	movq %rax,%rbx
L65:
	cmpq $0,%r14
	jnz L131
L129:
	movl %r13d,%edi
	movl %ebx,%esi
	call _toasc
	movq %rax,%r14
L131:
	addq %rbx,-16(%rbp)	 # spill
	movq -16(%rbp),%r10	 # spill
	cmpq -32(%rbp),%r10	 # spill
	jb L134
L132:
	xorl %eax,%eax
L47:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L134:
	movq %r15,%rdi
	movq %r14,%rsi
	movq %rbx,%rdx
	call _strncpy
	addq %rbx,%r15
	jmp L49
L184:
L21:
	.byte 68,101,99,101,109,98,101,114
	.byte 0
L20:
	.byte 78,111,118,101,109,98,101,114
	.byte 0
L19:
	.byte 79,99,116,111,98,101,114,0
L18:
	.byte 83,101,112,116,101,109,98,101
	.byte 114,0
L17:
	.byte 65,117,103,117,115,116,0
L15:
	.byte 74,117,110,101,0
L14:
	.byte 77,97,121,0
L8:
	.byte 83,97,116,117,114,100,97,121
	.byte 0
L7:
	.byte 70,114,105,100,97,121,0
L6:
	.byte 84,104,117,114,115,100,97,121
	.byte 0
L5:
	.byte 87,101,100,110,101,115,100,97
	.byte 121,0
L4:
	.byte 84,117,101,115,100,97,121,0
L3:
	.byte 77,111,110,100,97,121,0
L2:
	.byte 83,117,110,100,97,121,0
L98:
	.byte 80,77,0
L12:
	.byte 77,97,114,99,104,0
L16:
	.byte 74,117,108,121,0
L11:
	.byte 70,101,98,114,117,97,114,121
	.byte 0
L10:
	.byte 74,97,110,117,97,114,121,0
L13:
	.byte 65,112,114,105,108,0
L97:
	.byte 65,77,0
.globl _tzname
.globl _strftime
.globl _asctime
.globl _strncpy
.globl _strlen
