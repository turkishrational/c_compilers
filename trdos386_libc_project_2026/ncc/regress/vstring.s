.text
_vstring_clear:
L1:
	pushq %rbp
	movq %rsp,%rbp
L2:
	movl (%rdi),%esi
	testl $1,%esi
	jz L5
L4:
	andl $-255,%esi
	movl %esi,(%rdi)
	movb $0,1(%rdi)
	jmp L3
L5:
	movq $0,8(%rdi)
	movq 16(%rdi),%rsi
	movb $0,(%rsi)
L3:
	popq %rbp
	ret
L10:
_vstring_init:
L11:
	pushq %rbp
	movq %rsp,%rbp
L12:
	movl (%rdi),%esi
	orl $1,%esi
	movl %esi,(%rdi)
	andl $-255,%esi
	movl %esi,(%rdi)
	movb $0,1(%rdi)
L13:
	popq %rbp
	ret
L17:
_vstring_free:
L18:
	pushq %rbp
	movq %rsp,%rbp
L19:
	movl (%rdi),%esi
	testl $1,%esi
	jnz L20
L21:
	movq 16(%rdi),%rdi
	call _free
L20:
	popq %rbp
	ret
L27:
_vstring_rubout:
L28:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L42:
	movq %rdi,%rbx
	movl (%rbx),%esi
	testl $1,%esi
	jz L35
L34:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	jmp L36
L35:
	movq 8(%rbx),%rsi
L36:
	cmpq $0,%rsi
	jnz L33
L31:
	pushq $L37
	call _error
	addq $8,%rsp
L33:
	movl (%rbx),%esi
	testl $1,%esi
	jz L39
L38:
	movl %esi,%edi
	shll $24,%edi
	sarl $25,%edi
	addl $-1,%edi
	shll $1,%edi
	andl $254,%edi
	andl $4294967041,%esi
	orl %edi,%esi
	movl %esi,(%rbx)
	jmp L30
L39:
	addq $-1,8(%rbx)
L30:
	popq %rbx
	popq %rbp
	ret
L44:
_vstring_last:
L45:
	pushq %rbp
	movq %rsp,%rbp
L46:
	movl (%rdi),%esi
	testl $1,%esi
	jz L52
L51:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	jmp L53
L52:
	movq 8(%rdi),%rsi
L53:
	cmpq $0,%rsi
	jnz L50
L48:
	xorl %eax,%eax
	jmp L47
L50:
	movl (%rdi),%esi
	testl $1,%esi
	jz L56
L55:
	shll $24,%esi
	sarl $25,%esi
	addl $-1,%esi
	movslq %esi,%rsi
	movzbl 1(%rdi,%rsi),%eax
	jmp L47
L56:
	movq 16(%rdi),%rsi
	movq 8(%rdi),%rdi
	movzbl -1(%rsi,%rdi),%eax
L47:
	popq %rbp
	ret
L63:
_vstring_put:
L64:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L98:
	movq %rdi,%rbx
	movq %rsi,%r14
	movq %rdx,%r15
	movl (%rbx),%esi
	testl $1,%esi
	jz L68
L67:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	jmp L69
L68:
	movq 8(%rbx),%rsi
L69:
	leaq 1(%rsi,%r15),%r13
	cmpq %r15,%r13
	jae L72
L70:
	pushq $L73
	call _error
	addq $8,%rsp
L72:
	movl (%rbx),%esi
	testl $1,%esi
	jz L78
L77:
	movl $23,%esi
	jmp L79
L78:
	movq (%rbx),%rsi
L79:
	cmpq %rsi,%r13
	jbe L76
L74:
	movl $32,%r12d
L83:
	cmpq %r13,%r12
	jae L82
L81:
	shlq $1,%r12
	jnz L83
L82:
	cmpq $0,%r12
	jnz L89
L87:
	pushq $L73
	call _error
	addq $8,%rsp
L89:
	movq %r12,%rdi
	call _safe_malloc
	movq %rax,%r13
	movl (%rbx),%esi
	testl $1,%esi
	jz L92
L91:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rdx
	leaq 1(%rbx),%rsi
	movq %r13,%rdi
	call _memcpy
	movl (%rbx),%esi
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	movq %rsi,8(%rbx)
	jmp L93
L92:
	movq 8(%rbx),%rdx
	movq 16(%rbx),%rsi
	movq %r13,%rdi
	call _memcpy
	movq 16(%rbx),%rdi
	call _free
L93:
	movq %r12,(%rbx)
	movq %r13,16(%rbx)
L76:
	movl (%rbx),%esi
	testl $1,%esi
	jz L95
L94:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	leaq 1(%rbx,%rsi),%rdi
	movq %r14,%rsi
	movq %r15,%rdx
	call _memcpy
	movl (%rbx),%esi
	shll $24,%esi
	sarl $25,%esi
	addl %r15d,%esi
	shll $1,%esi
	andl $254,%esi
	movl (%rbx),%edi
	andl $4294967041,%edi
	orl %esi,%edi
	movl %edi,(%rbx)
	shll $24,%edi
	sarl $25,%edi
	movslq %edi,%rsi
	movb $0,1(%rbx,%rsi)
	jmp L66
L95:
	movq 16(%rbx),%rsi
	movq 8(%rbx),%rdi
	addq %rsi,%rdi
	movq %r14,%rsi
	movq %r15,%rdx
	call _memcpy
	addq %r15,8(%rbx)
	movq 16(%rbx),%rsi
	movq 8(%rbx),%rdi
	movb $0,(%rsi,%rdi)
L66:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L100:
_vstring_putc:
L101:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
L117:
	movb %sil,-8(%rbp)
	movl (%rdi),%esi
	testl $1,%esi
	jz L108
L107:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	jmp L109
L108:
	movq 8(%rdi),%rsi
L109:
	movl (%rdi),%eax
	testl $1,%eax
	jz L111
L110:
	movl $23,%eax
	jmp L112
L111:
	movq (%rdi),%rax
L112:
	addq $-2,%rax
	cmpq %rax,%rsi
	ja L105
L104:
	movl (%rdi),%eax
	testl $1,%eax
	jz L114
L113:
	movl %eax,%ecx
	shll $24,%ecx
	sarl $25,%ecx
	movl %ecx,%esi
	addl $1,%ecx
	shll $1,%ecx
	andl $254,%ecx
	andl $4294967041,%eax
	orl %ecx,%eax
	movl %eax,(%rdi)
	movslq %esi,%rsi
	movzbl -8(%rbp),%eax
	movb %al,1(%rdi,%rsi)
	movl (%rdi),%esi
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	movb $0,1(%rdi,%rsi)
	jmp L103
L114:
	movq 16(%rdi),%rsi
	movq 8(%rdi),%rcx
	movq %rcx,%rax
	addq $1,%rcx
	movq %rcx,8(%rdi)
	movzbl -8(%rbp),%ecx
	movb %cl,(%rsi,%rax)
	movq 16(%rdi),%rsi
	movq 8(%rdi),%rdi
	movb $0,(%rsi,%rdi)
	jmp L103
L105:
	leaq -8(%rbp),%rsi
	movl $1,%edx
	call _vstring_put
L103:
	movq %rbp,%rsp
	popq %rbp
	ret
L119:
_vstring_concat:
L120:
	pushq %rbp
	movq %rsp,%rbp
L121:
	movl (%rsi),%eax
	testl $1,%eax
	jz L124
L123:
	shll $24,%eax
	sarl $25,%eax
	movslq %eax,%rdx
	jmp L125
L124:
	movq 8(%rsi),%rdx
L125:
	movl (%rsi),%eax
	testl $1,%eax
	jz L127
L126:
	addq $1,%rsi
	jmp L128
L127:
	movq 16(%rsi),%rsi
L128:
	call _vstring_put
L122:
	popq %rbp
	ret
L132:
_vstring_puts:
L133:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L137:
	movq %rdi,%r12
	movq %rsi,%rbx
	movq %rbx,%rdi
	call _strlen
	movq %r12,%rdi
	movq %rbx,%rsi
	movq %rax,%rdx
	call _vstring_put
L135:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L139:
_vstring_same:
L140:
	pushq %rbp
	movq %rsp,%rbp
L141:
	movl (%rdi),%eax
	testl $1,%eax
	jz L151
L150:
	shll $24,%eax
	sarl $25,%eax
	movslq %eax,%rax
	jmp L152
L151:
	movq 8(%rdi),%rax
L152:
	movl (%rsi),%ecx
	testl $1,%ecx
	jz L154
L153:
	shll $24,%ecx
	sarl $25,%ecx
	movslq %ecx,%rcx
	jmp L155
L154:
	movq 8(%rsi),%rcx
L155:
	cmpq %rcx,%rax
	jnz L144
L146:
	movl (%rdi),%eax
	testl $1,%eax
	jz L157
L156:
	shll $24,%eax
	sarl $25,%eax
	movslq %eax,%rdx
	jmp L158
L157:
	movq 8(%rdi),%rdx
L158:
	movl (%rsi),%eax
	testl $1,%eax
	jz L160
L159:
	addq $1,%rsi
	jmp L161
L160:
	movq 16(%rsi),%rsi
L161:
	movl (%rdi),%eax
	testl $1,%eax
	jz L163
L162:
	addq $1,%rdi
	jmp L164
L163:
	movq 16(%rdi),%rdi
L164:
	call _memcmp
	cmpl $0,%eax
	jnz L144
L143:
	movl $1,%eax
	jmp L142
L144:
	xorl %eax,%eax
L142:
	popq %rbp
	ret
L170:
L73:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,118,115
	.byte 116,114,105,110,103,32,111,118
	.byte 101,114,102,108,111,119,0
L37:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,118,115
	.byte 116,114,105,110,103,32,117,110
	.byte 100,101,114,102,108,111,119,0
.globl _memcmp
.globl _vstring_clear
.globl _error
.globl _vstring_putc
.globl _vstring_last
.globl _vstring_concat
.globl _vstring_init
.globl _vstring_put
.globl _vstring_rubout
.globl _vstring_puts
.globl _safe_malloc
.globl _vstring_same
.globl _vstring_free
.globl _free
.globl _memcpy
.globl _strlen
