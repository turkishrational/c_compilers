.data
.align 1
_digits:
	.byte 48
	.byte 49
	.byte 50
	.byte 51
	.byte 52
	.byte 53
	.byte 54
	.byte 55
	.byte 56
	.byte 57
	.byte 65
	.byte 66
	.byte 67
	.byte 68
	.byte 69
	.byte 70
.align 1
_ldigits:
	.byte 48
	.byte 49
	.byte 50
	.byte 51
	.byte 52
	.byte 53
	.byte 54
	.byte 55
	.byte 56
	.byte 57
	.byte 97
	.byte 98
	.byte 99
	.byte 100
	.byte 101
	.byte 102
.text
_convert:
L4:
	pushq %rbp
	movq %rsp,%rbp
	subq $24,%rsp
L5:
	cmpl $120,%ecx
	jnz L8
L7:
	movq $_ldigits,%rcx
	jmp L9
L8:
	movq $_digits,%rcx
L9:
	leaq -2(%rbp),%r8
	movb $0,-2(%rbp)
	movl %edx,%r9d
L10:
	movq %rsi,%rax
	xorl %edx,%edx
	divq %r9
	movzbl (%rcx,%rdx),%eax
	addq $-1,%r8
	movb %al,(%r8)
	movq %rsi,%rax
	xorl %edx,%edx
	divq %r9
	movq %rax,%rsi
	cmpq $0,%rsi
	jnz L10
L13:
	movzbl (%r8),%esi
	cmpl $0,%esi
	jz L15
L14:
	movzbl (%r8),%esi
	addq $1,%r8
	movb %sil,(%rdi)
	addq $1,%rdi
	jmp L13
L15:
	movq %rdi,%rax
L6:
	movq %rbp,%rsp
	popq %rbp
	ret
L21:
_vfprintf:
L22:
	pushq %rbp
	movq %rsp,%rbp
	subq $624,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L341:
	movq %rdx,-576(%rbp)	 # spill
	movq %rsi,%rbx
	movq %rdi,%r12
	movl $0,-536(%rbp)	 # spill
L29:
	movzbl (%rbx),%esi
	addq $1,%rbx
	cmpl $37,%esi
	jz L31
L30:
	cmpl $0,%esi
	jnz L34
L32:
	movl -536(%rbp),%eax	 # spill
L24:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L34:
	movl (%r12),%edi
	addl $1,-536(%rbp)	 # spill
	addl $-1,%edi
	movl %edi,(%r12)
	cmpl $0,%edi
	jl L37
L36:
	movq 24(%r12),%rdi
	movq %rdi,%rax
	addq $1,%rdi
	movq %rdi,24(%r12)
	movb %sil,(%rax)
	jmp L29
L37:
	movl %esi,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L29
L31:
	xorl %eax,%eax
	movl $0,-544(%rbp)	 # spill
	movl $0,-552(%rbp)	 # spill
	movl $0,-560(%rbp)	 # spill
	movl $32,-568(%rbp)	 # spill
L40:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	cmpl $32,%esi
	jz L51
L308:
	cmpl $35,%esi
	jz L53
L309:
	cmpl $43,%esi
	jz L49
L310:
	cmpl $45,%esi
	jz L47
L311:
	cmpl $48,%esi
	jz L55
L42:
	cmpl $42,%esi
	jnz L60
L59:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movl -8(%r10),%esi
	movl %esi,-584(%rbp)	 # spill
	cmpl $0,%esi
	jge L64
L62:
	movl $1,-560(%rbp)	 # spill
	negl %esi
	movl %esi,-584(%rbp)	 # spill
L64:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	jmp L61
L60:
	movl $0,-584(%rbp)	 # spill
L65:
	cmpl $48,%ecx
	jl L61
L69:
	cmpl $57,%ecx
	jg L61
L66:
	movl -584(%rbp),%esi	 # spill
	imull $10,%esi
	leal -48(%rsi,%rcx),%esi
	movl %esi,-584(%rbp)	 # spill
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	jmp L65
L61:
	cmpl $46,%ecx
	jnz L74
L73:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	cmpl $42,%esi
	jnz L77
L76:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movl -8(%r10),%esi
	movl %esi,%r14d
	cmpl $0,%esi
	jge L81
L79:
	movl $-1,%r14d
L81:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	jmp L75
L77:
	xorl %r14d,%r14d
L82:
	cmpl $48,%ecx
	jl L75
L86:
	cmpl $57,%ecx
	jg L75
L83:
	movl %r14d,%esi
	imull $10,%esi
	leal -48(%rsi,%rcx),%esi
	movl %esi,%r14d
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	jmp L82
L74:
	movl $-1,%r14d
L75:
	cmpl $108,%ecx
	jz L90
L101:
	cmpl $104,%ecx
	jz L90
L97:
	cmpl $76,%ecx
	jz L90
L93:
	cmpl $122,%ecx
	jnz L91
L90:
	cmpl $122,%ecx
	jz L107
L106:
	movl %ecx,-592(%rbp)	 # spill
L107:
	movzbl (%rbx),%esi
	addq $1,%rbx
	movl %esi,%ecx
	jmp L92
L91:
	movl $0,-592(%rbp)	 # spill
L92:
	leaq -512(%rbp),%r10
	movq %r10,-600(%rbp)	 # spill
	movq -600(%rbp),%r13	 # spill
	movq %r13,%r15
	movl $0,-608(%rbp)	 # spill
	movl $0,-616(%rbp)	 # spill
	movl $0,-624(%rbp)	 # spill
	movl $0,-520(%rbp)
	cmpl $105,%ecx
	jz L113
	jb L314
L328:
	cmpl $115,%ecx
	jz L176
	jb L329
L336:
	cmpl $120,%ecx
	jz L131
	ja L109
L337:
	cmpl $117,%ecx
	jnz L109
L128:
	movl $10,%edx
	jmp L126
L329:
	cmpl $111,%ecx
	jz L125
	jb L330
L333:
	cmpl $112,%ecx
	jnz L109
L193:
	movl $108,-592(%rbp)	 # spill
	movl $16,%r14d
	addl $1,%eax
	movl $88,%ecx
	movl $16,%edx
	jmp L132
L330:
	cmpl $110,%ecx
	jnz L109
L195:
	cmpl $104,-592(%rbp)	 # spill
	jnz L197
L196:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movl -536(%rbp),%r10d	 # spill
	movw %r10w,(%rsi)
	jmp L110
L197:
	cmpl $108,-592(%rbp)	 # spill
	jnz L200
L199:
	movslq -536(%rbp),%rdi	 # spill
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movq %rdi,(%rsi)
	jmp L110
L200:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movl -536(%rbp),%r10d	 # spill
	movl %r10d,(%rsi)
	jmp L110
L125:
	movl $8,%edx
	jmp L126
L176:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rdi
	movq %rdi,%rsi
	cmpq $0,%rdi
	jnz L179
L177:
	movq $L180,%rsi
L179:
	movq %rsi,%r13
	movq %r13,%r15
	movslq %r14d,%rax
L181:
	movzbl (%r15),%edi
	addq $1,%r15
	cmpl $0,%edi
	jz L183
L182:
	cmpl $0,%r14d
	jl L181
L187:
	movq %r15,%rdi
	subq %rsi,%rdi
	cmpq %rax,%rdi
	jle L181
L183:
	addq $-1,%r15
	jmp L110
L314:
	cmpl $99,%ecx
	jz L174
	jb L315
L322:
	cmpl $101,%ecx
	jae L327
L323:
	cmpl $100,%ecx
	jz L113
	jnz L109
L327:
	cmpl $103,%ecx
	jbe L172
	ja L109
L315:
	cmpl $71,%ecx
	jz L172
	jb L316
L319:
	cmpl $88,%ecx
	jnz L109
L131:
	movl $16,%edx
L126:
	cmpl $108,-592(%rbp)	 # spill
	jnz L133
L132:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	jmp L137
L133:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movl -8(%r10),%esi
	movslq %esi,%rdi
	movq %rdi,%rsi
	cmpl $104,-592(%rbp)	 # spill
	jnz L137
L135:
	movzwq %di,%rsi
L137:
	cmpl $0,%eax
	jz L123
L141:
	cmpq $0,%rsi
	jz L145
L149:
	cmpl $8,%edx
	jz L138
L145:
	cmpl $16,%edx
	jnz L123
L138:
	movl %ecx,-616(%rbp)	 # spill
	jmp L123
L316:
	cmpl $69,%ecx
	jz L172
L109:
	movl (%r12),%esi
	addl $1,-536(%rbp)	 # spill
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L204
L203:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb %cl,(%rdi)
	jmp L29
L204:
	movl %ecx,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L29
L172:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movsd -8(%r10),%xmm0
	leaq -528(%rbp),%rsi
	movsd %xmm0,-528(%rbp)
	leaq -520(%rbp),%r9
	movq -600(%rbp),%rdi	 # spill
	movl %ecx,%edx
	movl %r14d,%ecx
	movl %eax,%r8d
	call ___dtefg
	movq %rax,%rsi
	movq %rsi,%r15
	jmp L110
L174:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movl -8(%r10),%esi
	movb %sil,(%r13)
	leaq -511(%rbp),%r15
	jmp L110
L113:
	movl $10,%edx
	cmpl $108,-592(%rbp)	 # spill
	jnz L115
L114:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	jmp L119
L115:
	addq $8,-576(%rbp)	 # spill
	movq -576(%rbp),%r10	 # spill
	movl -8(%r10),%esi
	movslq %esi,%rdi
	movq %rdi,%rsi
	cmpl $104,-592(%rbp)	 # spill
	jnz L119
L117:
	movswq %di,%rsi
L119:
	cmpq $0,%rsi
	jge L121
L120:
	negq %rsi
	addl $-1,-520(%rbp)
	jmp L123
L121:
	addl $1,-520(%rbp)
L123:
	cmpl $0,%r14d
	jnz L155
L156:
	cmpq $0,%rsi
	jz L110
L155:
	cmpl $-1,%r14d
	jz L163
L161:
	movl $32,-568(%rbp)	 # spill
L163:
	movq -600(%rbp),%rdi	 # spill
	call _convert
	movq %rax,%rsi
	movq %rsi,%r15
	subq -600(%rbp),%rsi	 # spill
	movq %rsi,%rdi
	movl %r14d,%esi
	subl %edi,%esi
	movl %esi,-624(%rbp)	 # spill
	cmpl $0,%esi
	jge L110
L164:
	movl $0,-624(%rbp)	 # spill
L110:
	movq %r15,%rsi
	subq %r13,%rsi
	movq %rsi,%rax
	movl -520(%rbp),%edi
	cmpl $0,%edi
	jz L209
L210:
	cmpl $-1,%edi
	jz L207
L218:
	cmpl $0,-552(%rbp)	 # spill
	jnz L207
L214:
	cmpl $0,-544(%rbp)	 # spill
	jz L209
L207:
	leal 1(%rax),%esi
	movl $1,-608(%rbp)	 # spill
L209:
	cmpl $0,-616(%rbp)	 # spill
	jz L224
L222:
	addl $1,%esi
	addl $1,-608(%rbp)	 # spill
	cmpl $111,-616(%rbp)	 # spill
	jz L224
L225:
	addl $1,%esi
L224:
	movl -584(%rbp),%edi	 # spill
	subl -624(%rbp),%edi	 # spill
	subl %esi,%edi
	movl %edi,%r14d
	cmpl $0,%edi
	jge L230
L228:
	xorl %r14d,%r14d
L230:
	addl %r14d,%esi
	addl -624(%rbp),%esi	 # spill
	addl %esi,-536(%rbp)	 # spill
	cmpl $0,-560(%rbp)	 # spill
	jnz L233
L234:
	cmpl $0,%r14d
	jle L233
L231:
	cmpl $0,-608(%rbp)	 # spill
	jz L245
L241:
	cmpl $48,-568(%rbp)	 # spill
	jnz L245
L238:
	addl %r14d,-624(%rbp)	 # spill
	jmp L233
L245:
	movl %r14d,%esi
	addl $-1,%r14d
	cmpl $0,%esi
	jle L233
L246:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L249
L248:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movl -568(%rbp),%r10d	 # spill
	movb %r10b,(%rdi)
	jmp L245
L249:
	movl -568(%rbp),%edi	 # spill
	movq %r12,%rsi
	call ___flushbuf
	jmp L245
L233:
	movl -520(%rbp),%esi
	cmpl $0,%esi
	jz L253
L251:
	cmpl $-1,%esi
	jnz L255
L254:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L258
L257:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $45,(%rdi)
	jmp L253
L258:
	movl $45,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L253
L255:
	cmpl $0,-552(%rbp)	 # spill
	jz L261
L260:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L264
L263:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $43,(%rdi)
	jmp L253
L264:
	movl $43,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L253
L261:
	cmpl $0,-544(%rbp)	 # spill
	jz L253
L266:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L270
L269:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $32,(%rdi)
	jmp L253
L270:
	movl $32,%edi
	movq %r12,%rsi
	call ___flushbuf
L253:
	cmpl $0,-616(%rbp)	 # spill
	jz L284
L272:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L276
L275:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $48,(%rdi)
	jmp L277
L276:
	movl $48,%edi
	movq %r12,%rsi
	call ___flushbuf
L277:
	cmpl $111,-616(%rbp)	 # spill
	jz L284
L278:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L282
L281:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movl -616(%rbp),%r10d	 # spill
	movb %r10b,(%rdi)
	jmp L284
L282:
	movl -616(%rbp),%edi	 # spill
	movq %r12,%rsi
	call ___flushbuf
L284:
	movl -624(%rbp),%esi	 # spill
	addl $-1,-624(%rbp)	 # spill
	cmpl $0,%esi
	jle L286
L285:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L288
L287:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $48,(%rdi)
	jmp L284
L288:
	movl $48,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L284
L286:
	movq %r15,%rsi
	subq %r13,%rsi
	movl %esi,%r15d
L290:
	movl %r15d,%esi
	addl $-1,%r15d
	cmpl $0,%esi
	jle L292
L291:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L294
L293:
	movzbl (%r13),%esi
	addq $1,%r13
	movq 24(%r12),%rdi
	movq %rdi,%rax
	addq $1,%rdi
	movq %rdi,24(%r12)
	movb %sil,(%rax)
	jmp L290
L294:
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L290
L292:
	cmpl $0,-560(%rbp)	 # spill
	jz L29
L299:
	movl %r14d,%esi
	addl $-1,%r14d
	cmpl $0,%esi
	jle L29
L300:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L303
L302:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movb $32,(%rdi)
	jmp L299
L303:
	movl $32,%edi
	movq %r12,%rsi
	call ___flushbuf
	jmp L299
L55:
	movl $48,-568(%rbp)	 # spill
	jmp L40
L47:
	addl $1,-560(%rbp)	 # spill
	jmp L40
L49:
	addl $1,-552(%rbp)	 # spill
	jmp L40
L53:
	addl $1,%eax
	jmp L40
L51:
	addl $1,-544(%rbp)	 # spill
	jmp L40
L343:
L180:
	.byte 123,78,85,76,76,125,0
.globl _vfprintf
.globl ___dtefg
.globl ___flushbuf
