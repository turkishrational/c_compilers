.text
_string_new:
L2:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L42:
	movq %rdi,%r14
	movq %rsi,%r12
	xorl %ebx,%ebx
	xorl %esi,%esi
L5:
	cmpq %r12,%rsi
	jae L8
L6:
	shll $4,%ebx
	movzbl (%r14,%rsi),%edi
	xorl %edi,%ebx
	addq $1,%rsi
	jmp L5
L8:
	movl %ebx,%esi
	andl $31,%esi
	movl %esi,%esi
	shlq $4,%rsi
	leaq _buckets(%rsi),%r10
	movq %r10,-8(%rbp)	 # spill
	movq _buckets(%rsi),%r13
L9:
	cmpq $0,%r13
	jz L12
L10:
	cmpq %r12,8(%r13)
	jnz L11
L20:
	movl (%r13),%esi
	cmpl %ebx,%esi
	jnz L11
L16:
	leaq 40(%r13),%rdi
	movq %r14,%rsi
	movq %r12,%rdx
	call _memcmp
	cmpl $0,%eax
	jnz L11
L24:
	movq 24(%r13),%rsi
	cmpq $0,%rsi
	jz L28
L27:
	movq 32(%r13),%rdi
	movq %rdi,32(%rsi)
	jmp L29
L28:
	movq 32(%r13),%rsi
	movq -8(%rbp),%r10	 # spill
	movq %rsi,8(%r10)
L29:
	movq 24(%r13),%rsi
	movq 32(%r13),%rdi
	movq %rsi,(%rdi)
	jmp L12
L11:
	movq 24(%r13),%r13
	jmp L9
L12:
	cmpq $0,%r13
	jnz L34
L31:
	leaq 41(%r12),%rdi
	call _safe_malloc
	movq %rax,%r15
	movq %r15,%r13
	movl %ebx,(%r15)
	movq %r12,8(%r15)
	movl $262145,16(%r15)
	movl $0,20(%r15)
	leaq 40(%r15),%rdi
	movq %r14,%rsi
	movq %r12,%rdx
	call _memcpy
	movb $0,40(%r15,%r12)
L34:
	movq -8(%rbp),%r10	 # spill
	movq (%r10),%rdi
	leaq 24(%r13),%rsi
	movq %rdi,24(%r13)
	cmpq $0,%rdi
	jz L38
L37:
	movq -8(%rbp),%r10	 # spill
	movq (%r10),%rdi
	movq %rsi,32(%rdi)
	jmp L39
L38:
	movq -8(%rbp),%r10	 # spill
	movq %rsi,8(%r10)
L39:
	movq -8(%rbp),%r10	 # spill
	movq %r13,(%r10)
	movq -8(%rbp),%r10	 # spill
	movq %r10,32(%r13)
	movq %r13,%rax
L4:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L44:
.data
.align 4
_keywords:
	.int 58
	.byte 95,95,97,115,109,0,0,0
	.byte 0
	.space 3, 0
	.int 131131
	.byte 97,117,116,111,0,0,0,0
	.byte 0
	.space 3, 0
	.int 60
	.byte 98,114,101,97,107,0,0,0
	.byte 0
	.space 3, 0
	.int 61
	.byte 99,97,115,101,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131646
	.byte 99,104,97,114,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131135
	.byte 99,111,110,115,116,0,0,0
	.byte 0
	.space 3, 0
	.int 64
	.byte 99,111,110,116,105,110,117,101
	.byte 0
	.space 3, 0
	.int 65
	.byte 100,101,102,97,117,108,116,0
	.byte 0
	.space 3, 0
	.int 66
	.byte 100,111,0,0,0,0,0,0
	.byte 0
	.space 3, 0
	.int 147523
	.byte 100,111,117,98,108,101,0,0
	.byte 0
	.space 3, 0
	.int 68
	.byte 101,108,115,101,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131141
	.byte 101,110,117,109,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131142
	.byte 101,120,116,101,114,110,0,0
	.byte 0
	.space 3, 0
	.int 139335
	.byte 102,108,111,97,116,0,0,0
	.byte 0
	.space 3, 0
	.int 72
	.byte 102,111,114,0,0,0,0,0
	.byte 0
	.space 3, 0
	.int 73
	.byte 103,111,116,111,0,0,0,0
	.byte 0
	.space 3, 0
	.int 74
	.byte 105,102,0,0,0,0,0,0
	.byte 0
	.space 3, 0
	.int 133195
	.byte 105,110,116,0,0,0,0,0
	.byte 0
	.space 3, 0
	.int 135244
	.byte 108,111,110,103,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131149
	.byte 114,101,103,105,115,116,101,114
	.byte 0
	.space 3, 0
	.int 78
	.byte 114,101,116,117,114,110,0,0
	.byte 0
	.space 3, 0
	.int 132175
	.byte 115,104,111,114,116,0,0,0
	.byte 0
	.space 3, 0
	.int 196688
	.byte 115,105,103,110,101,100,0,0
	.byte 0
	.space 3, 0
	.int 81
	.byte 115,105,122,101,111,102,0,0
	.byte 0
	.space 3, 0
	.int 131154
	.byte 115,116,97,116,105,99,0,0
	.byte 0
	.space 3, 0
	.int 131155
	.byte 115,116,114,117,99,116,0,0
	.byte 0
	.space 3, 0
	.int 84
	.byte 115,119,105,116,99,104,0,0
	.byte 0
	.space 3, 0
	.int 131157
	.byte 116,121,112,101,100,101,102,0
	.byte 0
	.space 3, 0
	.int 131158
	.byte 117,110,105,111,110,0,0,0
	.byte 0
	.space 3, 0
	.int 163927
	.byte 117,110,115,105,103,110,101,100
	.byte 0
	.space 3, 0
	.int 131416
	.byte 118,111,105,100,0,0,0,0
	.byte 0
	.space 3, 0
	.int 131161
	.byte 118,111,108,97,116,105,108,101
	.byte 0
	.space 3, 0
	.int 90
	.byte 119,104,105,108,101,0,0,0
	.byte 0
	.space 3, 0
.text
_string_init:
L45:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L46:
	xorl %esi,%esi
L52:
	movslq %esi,%rdi
	shlq $4,%rdi
	leaq _buckets(%rdi),%rax
	movq $0,_buckets(%rdi)
	movq %rax,_buckets+8(%rdi)
	addl $1,%esi
	cmpl $32,%esi
	jl L52
L51:
	xorl %r12d,%r12d
L55:
	movslq %r12d,%rsi
	cmpq $33,%rsi
	jae L47
L56:
	movslq %r12d,%rbx
	shlq $4,%rbx
	leaq _keywords+4(%rbx),%r13
	movq %r13,%rdi
	call _strlen
	movq %r13,%rdi
	movq %rax,%rsi
	call _string_new
	movl _keywords(%rbx),%esi
	movl %esi,16(%rax)
	addl $1,%r12d
	jmp L55
L47:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L62:
_string_print_k:
L63:
	pushq %rbp
	movq %rsp,%rbp
L64:
	xorl %eax,%eax
L66:
	movslq %eax,%rcx
	cmpq $33,%rcx
	jae L65
L67:
	movslq %eax,%rcx
	shlq $4,%rcx
	movl _keywords(%rcx),%edx
	cmpl %esi,%edx
	jnz L68
L70:
	leaq _keywords+4(%rcx),%rsi
	pushq %rsi
	pushq $L73
	pushq %rdi
	call _fprintf
	addq $24,%rsp
	jmp L65
L68:
	addl $1,%eax
	jmp L66
L65:
	popq %rbp
	ret
L78:
_string_emit:
L79:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L101:
	movq %rsi,%r14
	movq %rdi,%r13
	xorl %ebx,%ebx
L82:
	cmpq %r14,%rbx
	jae L85
L83:
	cmpq 8(%r13),%rbx
	jae L87
L86:
	movzbl 40(%r13,%rbx),%r12d
	jmp L88
L87:
	xorl %r12d,%r12d
L88:
	testq $7,%rbx
	jnz L90
L89:
	cmpq $0,%rbx
	jz L94
L92:
	pushq $L95
	call _output
	addq $8,%rsp
L94:
	pushq $L96
	call _output
	addq $8,%rsp
	jmp L91
L90:
	pushq $L97
	call _output
	addq $8,%rsp
L91:
	movzbl %r12b,%esi
	andl $255,%esi
	pushq %rsi
	pushq $L98
	call _output
	addq $16,%rsp
	addq $1,%rbx
	jmp L82
L85:
	pushq $L95
	call _output
	addq $8,%rsp
L81:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L103:
_string_emit_literals:
L104:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L105:
	xorl %r12d,%r12d
L108:
	movslq %r12d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rbx
L111:
	cmpq $0,%rbx
	jz L109
L112:
	movl 20(%rbx),%esi
	cmpl $0,%esi
	jz L113
L115:
	movl $1,%edi
	call _output_select
	movl 20(%rbx),%esi
	pushq %rsi
	pushq $L118
	call _output
	addq $16,%rsp
	movq 8(%rbx),%rsi
	addq $1,%rsi
	movq %rbx,%rdi
	call _string_emit
L113:
	movq 24(%rbx),%rbx
	jmp L111
L109:
	addl $1,%r12d
	cmpl $32,%r12d
	jl L108
L106:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L122:
_string_symbol:
L123:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L128:
	movq %rdi,%r13
	movl 20(%r13),%edi
	call _symbol_anonymous
	movq %rax,%rbx
	movl 64(%rbx),%esi
	movl %esi,20(%r13)
	leaq 32(%rbx),%r12
	movq %r12,%rdi
	movl $8192,%esi
	call _type_append_bits
	movq 8(%r13),%rsi
	addq $1,%rsi
	movq %rsi,8(%rax)
	movq %r12,%rdi
	movl $2,%esi
	call _type_append_bits
	movq %rbx,%rax
L125:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L130:
L118:
	.byte 37,76,58,10,0
L95:
	.byte 10,0
L97:
	.byte 44,0
L96:
	.byte 9,46,98,121,116,101,32,0
L98:
	.byte 37,100,0
L73:
	.byte 39,37,115,39,0
.globl _memcmp
.globl _symbol_anonymous
.globl _string_emit
.globl _string_init
.globl _output_select
.globl _output
.globl _fprintf
.globl _string_new
.globl _keywords
.local _buckets
.comm _buckets, 512, 8
.globl _string_emit_literals
.globl _type_append_bits
.globl _safe_malloc
.globl _memcpy
.globl _string_print_k
.globl _string_symbol
.globl _strlen
