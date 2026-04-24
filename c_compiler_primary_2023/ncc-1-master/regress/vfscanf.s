.text
_dscan:
L2:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L101:
	movl %edx,-16(%rbp)	 # spill
	movq %rsi,%r12
	movq %rcx,%r15
	movq %rdi,-8(%rbp)	 # spill
	movq -8(%rbp),%r13	 # spill
	xorl %r14d,%r14d
	xorl %ebx,%ebx
L5:
	cmpl -16(%rbp),%r14d	 # spill
	jge L8
L6:
	movl (%r12),%esi
	addl $-1,%esi
	movl %esi,(%r12)
	cmpl $0,%esi
	jl L10
L9:
	movq 24(%r12),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r12)
	movzbl (%rdi),%eax
	jmp L11
L10:
	movq %r12,%rdi
	call ___fillbuf
L11:
	movl %eax,%esi
	movb %al,(%r13)
	addq $1,%r13
	cmpl $43,%esi
	jz L17
L95:
	cmpl $45,%esi
	jz L17
L96:
	cmpl $48,%esi
	jb L97
L98:
	cmpl $57,%esi
	ja L97
L36:
	cmpl $0,%ebx
	jz L37
L44:
	cmpl $1,%ebx
	jz L37
L40:
	cmpl $3,%ebx
	jnz L38
L37:
	movl $3,%ebx
	jmp L7
L38:
	cmpl $2,%ebx
	jz L48
L55:
	cmpl $4,%ebx
	jz L48
L51:
	cmpl $5,%ebx
	jnz L49
L48:
	movl $5,%ebx
	jmp L7
L49:
	cmpl $6,%ebx
	jz L59
L62:
	cmpl $7,%ebx
	jnz L14
L59:
	movl $7,%ebx
	jmp L7
L97:
	cmpl $69,%esi
	jz L69
L99:
	cmpl $101,%esi
	jz L69
L13:
	cmpl $46,%eax
	jnz L14
L81:
	cmpl $1,%ebx
	jg L84
L83:
	movl $2,%ebx
	jmp L7
L84:
	cmpl $3,%ebx
	jnz L14
L86:
	movl $4,%ebx
	jmp L7
L69:
	cmpl $3,%ebx
	jl L14
L73:
	movl $5,%esi
	cmpl %ebx,%esi
	jl L14
L72:
	movl $6,%ebx
	jmp L7
L17:
	cmpl $0,%ebx
	jz L20
L21:
	cmpl $6,%ebx
	jz L20
L14:
	addq $-1,%r13
	movl %eax,%edi
	movq %r12,%rsi
	call _ungetc
	jmp L8
L20:
	addl $1,%ebx
L7:
	addl $1,%r14d
	jmp L5
L8:
	movb $0,(%r13)
	movq -8(%rbp),%rdi	 # spill
	xorl %esi,%esi
	call _strtod
	movsd %xmm0,(%r15)
	subq -8(%rbp),%r13	 # spill
	movl %r13d,%eax
L4:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L103:
_lscan:
L105:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L210:
	movl %ecx,-16(%rbp)	 # spill
	movq %rsi,%r13
	movq %r9,-32(%rbp)	 # spill
	movl %edx,%ebx
	movq %rdi,-8(%rbp)	 # spill
	movl %r8d,-24(%rbp)	 # spill
	movq -8(%rbp),%r15	 # spill
	cmpl $0,%ebx
	jnz L109
L108:
	xorl %esi,%esi
	jmp L110
L109:
	cmpl $16,%ebx
	jnz L112
L111:
	movl $6,%esi
	jmp L110
L112:
	movl $3,%esi
L110:
	movl %esi,%r12d
	xorl %r14d,%r14d
L114:
	cmpl -16(%rbp),%r14d	 # spill
	jge L117
L115:
	movl (%r13),%esi
	addl $-1,%esi
	movl %esi,(%r13)
	cmpl $0,%esi
	jl L119
L118:
	movq 24(%r13),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%r13)
	movzbl (%rdi),%eax
	jmp L120
L119:
	movq %r13,%rdi
	call ___fillbuf
L120:
	movl %eax,%esi
	movb %al,(%r15)
	addq $1,%r15
	cmpl $43,%esi
	jz L126
L205:
	cmpl $45,%esi
	jz L126
L206:
	cmpl $48,%esi
	jz L140
L207:
	cmpl $88,%esi
	jz L156
L208:
	cmpl $120,%esi
	jz L156
L122:
	cmpl $2,%r12d
	jg L167
L165:
	leal -48(%rax),%esi
	cmpl $10,%esi
	jae L123
L168:
	movl $10,%ebx
	movl $5,%r12d
	jmp L116
L167:
	leal -48(%rax),%esi
	cmpl $10,%esi
	jae L180
L184:
	leal -48(%rax),%esi
	cmpl %ebx,%esi
	jl L173
L180:
	leal -97(%rax),%esi
	cmpl $26,%esi
	jae L176
L188:
	leal -87(%rax),%esi
	cmpl %ebx,%esi
	jl L173
L176:
	leal -65(%rax),%esi
	cmpl $26,%esi
	jae L123
L192:
	leal -55(%rax),%esi
	cmpl %ebx,%esi
	jge L123
L173:
	movl $5,%r12d
	jmp L116
L156:
	cmpl $2,%r12d
	jnz L158
L157:
	movl $16,%ebx
	movl $5,%r12d
	jmp L116
L158:
	cmpl $8,%r12d
	jnz L123
L160:
	movl $5,%r12d
	jmp L116
L140:
	cmpl $1,%r12d
	jg L142
L141:
	movl $2,%r12d
	jmp L116
L142:
	cmpl $2,%r12d
	jnz L145
L144:
	movl $8,%ebx
	movl $5,%r12d
	jmp L116
L145:
	cmpl $5,%r12d
	jle L147
L150:
	cmpl $8,%r12d
	jnz L148
L147:
	movl $5,%r12d
	jmp L116
L148:
	movl $8,%r12d
	jmp L116
L126:
	cmpl $0,%r12d
	jz L129
L134:
	cmpl $3,%r12d
	jz L129
L130:
	cmpl $6,%r12d
	jz L129
L123:
	addq $-1,%r15
	movl %eax,%edi
	movq %r13,%rsi
	call _ungetc
	jmp L117
L129:
	addl $1,%r12d
L116:
	addl $1,%r14d
	jmp L114
L117:
	movb $0,(%r15)
	cmpl $0,-24(%rbp)	 # spill
	jz L200
L199:
	movq -8(%rbp),%rdi	 # spill
	xorl %esi,%esi
	movl %ebx,%edx
	call _strtol
	movq -32(%rbp),%r10	 # spill
	movq %rax,(%r10)
	jmp L201
L200:
	movq -8(%rbp),%rdi	 # spill
	xorl %esi,%esi
	movl %ebx,%edx
	call _strtoul
	movq -32(%rbp),%r10	 # spill
	movq %rax,(%r10)
L201:
	movq %r15,%rax
	subq -8(%rbp),%rax	 # spill
L107:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L212:
_vfscanf:
L213:
	pushq %rbp
	movq %rsp,%rbp
	subq $200,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L499:
	movq %rdx,-192(%rbp)	 # spill
	movq %rsi,%r13
	movq %rdi,%rbx
	movl $0,-152(%rbp)	 # spill
	movl $0,-160(%rbp)	 # spill
L216:
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%r14d
	cmpl $0,%esi
	jz L219
L217:
	movslq %esi,%rdi
	movzbl ___ctype+1(%rdi),%edi
	testl $8,%edi
	jz L221
L223:
	movl (%rbx),%esi
	addl $-1,%esi
	movl %esi,(%rbx)
	cmpl $0,%esi
	jl L227
L226:
	movq 24(%rbx),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%rbx)
	movzbl (%rdi),%esi
	jmp L228
L227:
	movq %rbx,%rdi
	call ___fillbuf
	movl %eax,%esi
L228:
	movslq %esi,%rdi
	movzbl ___ctype+1(%rdi),%edi
	testl $8,%edi
	jz L225
L224:
	addl $1,-152(%rbp)	 # spill
	jmp L223
L225:
	cmpl $-1,%esi
	jz L219
L231:
	movl %esi,%edi
	movq %rbx,%rsi
	call _ungetc
	jmp L216
L221:
	cmpl $37,%esi
	jnz L237
L235:
	xorl %r15d,%r15d
	movl $0,-168(%rbp)	 # spill
	movl $0,-176(%rbp)	 # spill
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%r14d
	cmpl $42,%esi
	jnz L248
L246:
	movl $1,-168(%rbp)	 # spill
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%r14d
L248:
	leal -48(%r14),%esi
	cmpl $10,%esi
	jae L250
L249:
	movl $0,-184(%rbp)	 # spill
L252:
	leal -48(%r14),%esi
	cmpl $10,%esi
	jae L251
L253:
	movl -184(%rbp),%esi	 # spill
	imull $10,%esi
	leal -48(%rsi,%r14),%esi
	movl %esi,-184(%rbp)	 # spill
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%r14d
	jmp L252
L250:
	movl $-1,-184(%rbp)	 # spill
L251:
	cmpl $104,%r14d
	jz L256
L263:
	cmpl $108,%r14d
	jz L256
L259:
	cmpl $76,%r14d
	jnz L258
L256:
	movl %r14d,%r15d
	movzbl (%r13),%esi
	addq $1,%r13
	movl %esi,%r14d
L258:
	cmpl $91,%r14d
	jz L465
L274:
	cmpl $99,%r14d
	jz L465
L270:
	cmpl $110,%r14d
	jz L465
L278:
	movl (%rbx),%esi
	addl $-1,%esi
	movl %esi,(%rbx)
	cmpl $0,%esi
	jl L282
L281:
	movq 24(%rbx),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%rbx)
	movzbl (%rdi),%esi
	jmp L283
L282:
	movq %rbx,%rdi
	call ___fillbuf
	movl %eax,%esi
L283:
	movslq %esi,%rdi
	movzbl ___ctype+1(%rdi),%edi
	testl $8,%edi
	jz L280
L279:
	addl $1,-152(%rbp)	 # spill
	jmp L278
L280:
	movl %esi,%edi
	movq %rbx,%rsi
	call _ungetc
L465:
	cmpl $101,%r14d
	jae L498
L466:
	cmpl $88,%r14d
	jz L300
	jb L467
L476:
	cmpl $99,%r14d
	jz L437
	jb L477
L480:
	cmpl $100,%r14d
	jnz L219
L290:
	movl $10,%eax
	movl $1,-176(%rbp)	 # spill
	jmp L291
L477:
	cmpl $91,%r14d
	jnz L219
L405:
	movl $0,-176(%rbp)	 # spill
	movzbl (%r13),%edi
	addq $1,%r13
	movl %edi,%esi
	cmpl $94,%edi
	jnz L408
L406:
	movl $1,-176(%rbp)	 # spill
	movzbl (%r13),%esi
	addq $1,%r13
L408:
	leaq -136(%rbp),%rdi
	movq %rdi,%r12
	cmpl $93,%esi
	jnz L412
L409:
	movl $93,-136(%rbp)
	leaq -135(%rbp),%r12
	movzbl (%r13),%esi
	addq $1,%r13
L412:
	cmpl $0,%esi
	jz L414
L415:
	cmpl $93,%esi
	jz L414
L413:
	cmpl $45,%esi
	jnz L420
L426:
	leaq -136(%rbp),%rdi
	cmpq %r12,%rdi
	jz L420
L422:
	movzbl (%r13),%edi
	cmpl $93,%edi
	jz L420
L419:
	movzbl -1(%r12),%esi
	movl %esi,%edi
	movzbl (%r13),%eax
	addq $1,%r13
	movl %eax,%ecx
	cmpl %eax,%edi
	jle L433
L430:
	addq $-1,%r12
	jmp L421
L433:
	addl $1,%esi
	cmpl %ecx,%esi
	jg L421
L434:
	movb %sil,(%r12)
	addq $1,%r12
	jmp L433
L420:
	movb %sil,(%r12)
	addq $1,%r12
L421:
	movzbl (%r13),%esi
	addq $1,%r13
	jmp L412
L414:
	movb $0,(%r12)
	movl $91,%r14d
	jmp L355
L437:
	cmpl $-1,-184(%rbp)	 # spill
	jnz L355
L438:
	movl $1,-184(%rbp)	 # spill
	jmp L355
L467:
	cmpl $69,%r14d
	jz L330
	jb L468
L473:
	cmpl $71,%r14d
	jz L330
	jnz L219
L468:
	cmpl $37,%r14d
	jz L237
	ja L219
L469:
	cmpl $0,%r14d
	jz L219
	jnz L219
L237:
	movl (%rbx),%esi
	addl $-1,%esi
	movl %esi,(%rbx)
	cmpl $0,%esi
	jl L242
L241:
	movq 24(%rbx),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%rbx)
	movzbl (%rdi),%esi
	jmp L243
L242:
	movq %rbx,%rdi
	call ___fillbuf
	movl %eax,%esi
L243:
	cmpl %r14d,%esi
	jz L240
L238:
	movl %esi,%edi
	movq %rbx,%rsi
	call _ungetc
	jmp L219
L240:
	addl $1,-152(%rbp)	 # spill
	jmp L216
L498:
	cmpl $103,%r14d
	jbe L330
L483:
	cmpl $112,%r14d
	jz L442
	jb L484
L491:
	cmpl $117,%r14d
	jz L297
	jb L492
L495:
	cmpl $120,%r14d
	jnz L219
L300:
	movl $16,%eax
	jmp L291
L492:
	cmpl $115,%r14d
	jnz L219
L355:
	cmpl $0,-168(%rbp)	 # spill
	jnz L358
L356:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%r12
L358:
	movl $0,-200(%rbp)	 # spill
L359:
	cmpl $0,-184(%rbp)	 # spill
	jl L360
L363:
	movl -184(%rbp),%r10d	 # spill
	cmpl %r10d,-200(%rbp)	 # spill
	jge L362
L360:
	movl (%rbx),%esi
	addl $-1,%esi
	movl %esi,(%rbx)
	cmpl $0,%esi
	jl L371
L370:
	movq 24(%rbx),%rsi
	movq %rsi,%rdi
	addq $1,%rsi
	movq %rsi,24(%rbx)
	movzbl (%rdi),%esi
	movl %esi,%r15d
	jmp L372
L371:
	movq %rbx,%rdi
	call ___fillbuf
	movl %eax,%esi
	movl %esi,%r15d
L372:
	cmpl $-1,%r15d
	jz L362
L369:
	cmpl $115,%r14d
	jnz L377
L381:
	movslq %r15d,%rsi
	movzbl ___ctype+1(%rsi),%esi
	testl $8,%esi
	jnz L374
L377:
	cmpl $91,%r14d
	jnz L376
L385:
	leaq -136(%rbp),%rsi
	movq %rsi,%rdi
	movl %r15d,%esi
	call _strchr
	movq %rax,%rsi
	cmpq $0,%rsi
	setz %sil
	movzbl %sil,%esi
	cmpl %esi,-176(%rbp)	 # spill
	jz L376
L374:
	movl %r15d,%edi
	movq %rbx,%rsi
	call _ungetc
	jmp L362
L376:
	cmpl $0,-168(%rbp)	 # spill
	jnz L361
L390:
	movb %r15b,(%r12)
	addq $1,%r12
L361:
	addl $1,-200(%rbp)	 # spill
	jmp L359
L362:
	cmpl $0,-200(%rbp)	 # spill
	jz L219
L395:
	movl -152(%rbp),%r10d	 # spill
	addl -200(%rbp),%r10d	 # spill
	movl %r10d,-152(%rbp)	 # spill
	cmpl $0,-168(%rbp)	 # spill
	jnz L216
L399:
	cmpl $99,%r14d
	jz L403
L401:
	movb $0,(%r12)
L403:
	addl $1,-160(%rbp)	 # spill
	jmp L216
L297:
	movl $10,%eax
	jmp L291
L484:
	cmpl $110,%r14d
	jz L444
	jb L485
L488:
	cmpl $111,%r14d
	jnz L219
L295:
	movl $8,%eax
	jmp L291
L485:
	cmpl $105,%r14d
	jnz L219
L293:
	xorl %eax,%eax
	movl $1,-176(%rbp)	 # spill
L291:
	cmpl $-1,-184(%rbp)	 # spill
	jz L301
L304:
	cmpl $128,-184(%rbp)	 # spill
	jle L303
L301:
	movl $128,-184(%rbp)	 # spill
	jmp L303
L444:
	cmpl $108,%r15d
	jnz L446
L445:
	movslq -152(%rbp),%rsi	 # spill
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rdi
	movq %rsi,(%rdi)
	jmp L216
L446:
	cmpl $104,%r15d
	jnz L449
L448:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movl -152(%rbp),%r10d	 # spill
	movw %r10w,(%rsi)
	jmp L216
L449:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movl -152(%rbp),%r10d	 # spill
	movl %r10d,(%rsi)
	jmp L216
L442:
	movl $18,-184(%rbp)	 # spill
	movl $16,%eax
	movl $112,%r15d
L303:
	leaq -8(%rbp),%r9
	leaq -136(%rbp),%rsi
	movq %rsi,%rdi
	movq %rbx,%rsi
	movl %eax,%edx
	movl -184(%rbp),%ecx	 # spill
	movl -176(%rbp),%r8d	 # spill
	call _lscan
	movl %eax,%esi
	cmpl $0,%esi
	jz L219
L310:
	addl %esi,-152(%rbp)	 # spill
	cmpl $0,-168(%rbp)	 # spill
	jnz L216
L314:
	cmpl $112,%r15d
	jnz L317
L316:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movq -8(%rbp),%rdi
	movq %rdi,(%rsi)
	jmp L318
L317:
	cmpl $108,%r15d
	jnz L320
L319:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movq -8(%rbp),%rdi
	movq %rdi,(%rsi)
	jmp L318
L320:
	cmpl $104,%r15d
	jnz L323
L322:
	movq -8(%rbp),%rdi
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movw %di,(%rsi)
	jmp L318
L323:
	movq -8(%rbp),%rdi
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movl %edi,(%rsi)
L318:
	addl $1,-160(%rbp)	 # spill
	jmp L216
L330:
	cmpl $-1,-184(%rbp)	 # spill
	jz L331
L334:
	cmpl $128,-184(%rbp)	 # spill
	jle L333
L331:
	movl $128,-184(%rbp)	 # spill
L333:
	leaq -144(%rbp),%rax
	leaq -136(%rbp),%rsi
	movq %rsi,%rdi
	movq %rbx,%rsi
	movl -184(%rbp),%edx	 # spill
	movq %rax,%rcx
	call _dscan
	movl %eax,%esi
	cmpl $0,%esi
	jz L219
L340:
	addl %esi,-152(%rbp)	 # spill
	cmpl $0,-168(%rbp)	 # spill
	jnz L216
L344:
	cmpl $108,%r15d
	jz L346
L349:
	cmpl $76,%r15d
	jnz L347
L346:
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movsd -144(%rbp),%xmm0
	movsd %xmm0,(%rsi)
	jmp L348
L347:
	movsd -144(%rbp),%xmm0
	cvtsd2ss %xmm0,%xmm0
	addq $8,-192(%rbp)	 # spill
	movq -192(%rbp),%r10	 # spill
	movq -8(%r10),%rsi
	movss %xmm0,(%rsi)
L348:
	addl $1,-160(%rbp)	 # spill
	jmp L216
L219:
	cmpl $0,-160(%rbp)	 # spill
	jnz L457
L459:
	movl 8(%rbx),%esi
	testl $16,%esi
	jz L457
L456:
	movl $-1,%esi
	jmp L458
L457:
	movl -160(%rbp),%esi	 # spill
L458:
	movl %esi,%eax
L215:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L501:
.globl _ungetc
.globl ___ctype
.globl _vfscanf
.globl _strchr
.globl _strtod
.globl ___fillbuf
.globl _strtoul
.globl _strtol
