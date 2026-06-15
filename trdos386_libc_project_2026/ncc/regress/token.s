.data
.align 8
_pool:
	.quad 0
	.quad _pool
.text
_alloc:
L3:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L20:
	movl %edi,%ebx
	movq _pool(%rip),%rsi
	cmpq $0,%rsi
	jnz L7
L6:
	movl $48,%edi
	call _safe_malloc
	movq %rax,%r12
	jmp L8
L7:
	movq %rsi,%r12
	movq 32(%rsi),%rdi
	cmpq $0,%rdi
	jz L13
L12:
	movq 40(%rsi),%rax
	movq %rax,40(%rdi)
	jmp L14
L13:
	movq 40(%rsi),%rdi
	movq %rdi,_pool+8(%rip)
L14:
	movq 32(%rsi),%rdi
	movq 40(%rsi),%rsi
	movq %rdi,(%rsi)
L8:
	movl %ebx,(%r12)
	testl $2147483648,%ebx
	jnz L17
L15:
	leaq 8(%r12),%rdi
	call _vstring_init
L17:
	movq %r12,%rax
L5:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L22:
_token_number:
L23:
	pushq %rbp
	movq %rsp,%rbp
	subq $24,%rsp
	pushq %rbx
	pushq %r12
L24:
	leaq -24(%rbp),%rbx
	pushq %rdi
	pushq $L26
	pushq %rbx
	call _sprintf
	addq $24,%rsp
	movl $51,%edi
	call _alloc
	movq %rax,%r12
	leaq 8(%r12),%rdi
	movq %rbx,%rsi
	call _vstring_puts
	movq %r12,%rax
L25:
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L31:
_backslash:
L33:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L44:
	movl %esi,%ebx
	movq %rdi,%r12
	cmpl $92,%ebx
	jz L36
L39:
	cmpl $34,%ebx
	jnz L38
L36:
	movq %r12,%rdi
	movl $92,%esi
	call _vstring_putc
L38:
	movq %r12,%rdi
	movl %ebx,%esi
	call _vstring_putc
L35:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L46:
_token_string:
L47:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L55:
	movq %rdi,%rbx
	movl $52,%edi
	call _alloc
	movq %rax,%r12
	leaq 8(%rax),%rdi
	movl $34,%esi
	call _vstring_putc
L50:
	movzbl (%rbx),%esi
	cmpl $0,%esi
	jz L52
L51:
	movzbl (%rbx),%esi
	addq $1,%rbx
	leaq 8(%r12),%rdi
	call _backslash
	jmp L50
L52:
	leaq 8(%r12),%rdi
	movl $34,%esi
	call _vstring_putc
	movq %r12,%rax
L49:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L57:
_token_int:
L58:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L63:
	movq %rdi,%rbx
	movl $-2147483592,%edi
	call _alloc
	movq %rbx,8(%rax)
L60:
	popq %rbx
	popq %rbp
	ret
L65:
_token_convert_number:
L66:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
L90:
	movq %rdi,%r12
	movl $-2147483592,%r13d
	movl $0,_errno(%rip)
	leaq -8(%rbp),%rsi
	movl 8(%r12),%edi
	testl $1,%edi
	jz L70
L69:
	leaq 9(%r12),%rdi
	jmp L71
L70:
	movq 24(%r12),%rdi
L71:
	xorl %edx,%edx
	call _strtoul
	movq %rax,%rbx
	movq -8(%rbp),%rsi
	movzbl (%rsi),%edi
	call _toupper
	cmpl $76,%eax
	jnz L74
L72:
	addq $1,-8(%rbp)
L74:
	movq -8(%rbp),%rsi
	movzbl (%rsi),%edi
	call _toupper
	cmpl $85,%eax
	jnz L77
L75:
	movl $-2147483591,%r13d
L77:
	movq -8(%rbp),%rsi
	movzbl (%rsi),%edi
	call _toupper
	cmpl $76,%eax
	jnz L80
L78:
	addq $1,-8(%rbp)
L80:
	movq -8(%rbp),%rsi
	movzbl (%rsi),%esi
	cmpl $0,%esi
	jnz L81
L84:
	movl _errno(%rip),%esi
	cmpl $0,%esi
	jz L83
L81:
	pushq $L88
	call _error
	addq $8,%rsp
L83:
	leaq 8(%r12),%rdi
	call _vstring_free
	movl %r13d,(%r12)
	movq %rbx,8(%r12)
L68:
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L92:
_token_convert_char:
L93:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
L108:
	movq %rdi,%r12
	movl 8(%r12),%esi
	testl $1,%esi
	jz L97
L96:
	leaq 9(%r12),%rsi
	jmp L98
L97:
	movq 24(%r12),%rsi
L98:
	leaq -8(%rbp),%rdi
	movq %rsi,-8(%rbp)
	addq $1,%rsi
	movq %rsi,-8(%rbp)
	call _escape
	movl %eax,%ebx
	cmpl $-1,%ebx
	jnz L101
L99:
	pushq $L102
	call _error
	addq $8,%rsp
L101:
	movq -8(%rbp),%rsi
	movzbl (%rsi),%esi
	cmpl $39,%esi
	jz L105
L103:
	pushq $L106
	call _error
	addq $8,%rsp
L105:
	leaq 8(%r12),%rdi
	call _vstring_free
	movl $-2147483592,(%r12)
	movslq %ebx,%rsi
	movq %rsi,8(%r12)
L95:
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L110:
_token_free:
L111:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L124:
	movq %rdi,%rbx
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L117
L114:
	leaq 8(%rbx),%rdi
	call _vstring_free
L117:
	movq _pool(%rip),%rdi
	leaq 32(%rbx),%rsi
	movq %rdi,32(%rbx)
	cmpq $0,%rdi
	jz L121
L120:
	movq _pool(%rip),%rdi
	movq %rsi,40(%rdi)
	jmp L122
L121:
	movq %rsi,_pool+8(%rip)
L122:
	movq %rbx,_pool(%rip)
	movq $_pool,40(%rbx)
L113:
	popq %rbx
	popq %rbp
	ret
L126:
_token_space:
L127:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L128:
	movl $48,%edi
	call _alloc
	movq %rax,%rbx
	leaq 8(%rbx),%rdi
	movl $32,%esi
	call _vstring_putc
	movq %rbx,%rax
L129:
	popq %rbx
	popq %rbp
	ret
L134:
.data
.align 4
_modifiers:
	.int 536870935
	.int 536870937
	.int 536870936
	.int 536870938
	.int 0
	.int 536870939
	.int 0
	.int 536870940
	.int 0
	.int 536870941
	.int 536873247
	.int 536870942
	.int 536873505
	.int 536870944
	.int 0
	.int 536870946
	.int 536871689
	.int 536871971
	.int 0
	.int 536870948
	.int 536871691
	.int 536871973
	.int 0
	.int 536870950
	.int 1610612778
	.int 0
	.int 0
	.int 536872213
	.int 0
	.int 536872214
.text
_token_separate:
L136:
	pushq %rbp
	movq %rsp,%rbp
L246:
	cmpl $49,%edi
	jz L143
L247:
	cmpl $51,%edi
	jz L152
L248:
	cmpl $536870952,%edi
	jz L170
L249:
	cmpl $536871425,%edi
	jnz L140
L184:
	cmpl $536871944,%esi
	jz L185
L196:
	cmpl $536871971,%esi
	jz L185
L192:
	cmpl $536871689,%esi
	jz L185
L188:
	cmpl $536870948,%esi
	jnz L140
L185:
	movl $1,%eax
	jmp L138
L140:
	movl %edi,%eax
	andl $255,%eax
	movslq %eax,%rcx
	cmpq $15,%rcx
	jae L141
L203:
	movslq %eax,%rcx
	movl _modifiers+4(,%rcx,8),%ecx
	cmpl $0,%ecx
	jz L207
L208:
	cmpl $536870925,%esi
	jz L205
L212:
	cmpl $536872213,%esi
	jnz L207
L205:
	movl $1,%eax
	jmp L138
L207:
	movslq %eax,%rcx
	movl _modifiers(,%rcx,8),%eax
	cmpl $0,%eax
	jz L141
L217:
	cmpl %edi,%esi
	jz L220
L227:
	cmpl %eax,%esi
	jz L220
L223:
	movl _modifiers+4(,%rcx,8),%edi
	cmpl %edi,%esi
	jnz L222
L220:
	movl $1,%eax
	jmp L138
L222:
	andl $255,%eax
	movslq %eax,%rdi
	cmpq $15,%rdi
	jae L141
L234:
	movslq %eax,%rdi
	movl _modifiers(,%rdi,8),%eax
	cmpl %eax,%esi
	jz L236
L239:
	movl _modifiers+4(,%rdi,8),%edi
	cmpl %edi,%esi
	jnz L141
L236:
	movl $1,%eax
	jmp L138
L170:
	cmpl $536870952,%esi
	jz L171
L178:
	cmpl $536870953,%esi
	jz L171
L174:
	cmpl $51,%esi
	jnz L141
L171:
	movl $1,%eax
	jmp L138
L143:
	cmpl $49,%esi
	jz L144
L147:
	cmpl $51,%esi
	jnz L152
L144:
	movl $1,%eax
	jmp L138
L152:
	cmpl $49,%esi
	jz L153
L164:
	cmpl $51,%esi
	jz L153
L160:
	cmpl $536870952,%esi
	jz L153
L156:
	cmpl $536870953,%esi
	jnz L141
L153:
	movl $1,%eax
	jmp L138
L141:
	xorl %eax,%eax
L138:
	popq %rbp
	ret
L253:
.data
.align 4
_classes:
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 48
	.int 0
	.int 48
	.int 48
	.int 48
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 0
	.int 48
	.int 536870926
	.int 52
	.int 1610612748
	.int 1073741883
	.int 536871172
	.int 536872453
	.int 53
	.int 536870927
	.int 536870928
	.int 536871170
	.int 536871424
	.int 536870959
	.int 536871425
	.int 536870952
	.int 536871171
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 51
	.int 536870956
	.int 536870955
	.int 536871946
	.int 536870925
	.int 536871944
	.int 536870957
	.int 1073741883
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 536870931
	.int 1073741883
	.int 536870932
	.int 536872711
	.int 49
	.int 1073741883
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 49
	.int 536870929
	.int 536872966
	.int 536870930
	.int 536870958
	.int 0
.text
_token_scan:
L255:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L385:
	movq %rsi,%r15
	movq %rdi,%r14
	movq %r14,%rbx
	movzbl (%r14),%esi
	movl %esi,%edi
	cmpl $0,%edi
	jle L259
L261:
	movzbq %sil,%rdi
	cmpq $128,%rdi
	jae L259
L258:
	movzbq %sil,%rsi
	movl _classes(,%rsi,4),%r12d
	jmp L376
L259:
	xorl %r12d,%r12d
L376:
	cmpl $0,%r12d
	jz L371
L377:
	cmpl $48,%r12d
	jz L334
L378:
	cmpl $49,%r12d
	jz L339
L379:
	cmpl $51,%r12d
	jz L310
L380:
	cmpl $52,%r12d
	jb L381
L382:
	cmpl $53,%r12d
	ja L381
L270:
	movl $1,%r13d
L272:
	movzbl (%rbx),%esi
	cmpl $0,%esi
	jnz L277
L275:
	cmpl $52,%r12d
	jnz L279
L278:
	pushq $L281
	call _error
	addq $8,%rsp
	jmp L277
L279:
	pushq $L282
	call _error
	addq $8,%rsp
L277:
	movzbl (%rbx),%esi
	movzbl (%r14),%edi
	addq $1,%rbx
	cmpl %edi,%esi
	jnz L285
L286:
	cmpl $0,%r13d
	jz L267
L285:
	cmpl $0,%r13d
	jnz L293
L291:
	movzbl -1(%rbx),%esi
	cmpl $92,%esi
	jnz L293
L292:
	movl $1,%r13d
	jmp L272
L293:
	xorl %r13d,%r13d
	jmp L272
L381:
	cmpl $536870952,%r12d
	jz L296
L383:
	cmpl $536871425,%r12d
	jnz L266
L347:
	movzbl 1(%r14),%esi
	cmpl $62,%esi
	jnz L266
L348:
	movl $536870951,%r12d
	leaq 2(%r14),%rbx
	jmp L267
L266:
	leaq 1(%r14),%rbx
	movzbl (%r14),%edi
L352:
	movl %r12d,%esi
	andl $255,%esi
	movslq %esi,%rax
	cmpq $15,%rax
	jae L267
L353:
	movzbl (%rbx),%eax
	cmpl %edi,%eax
	jnz L356
L358:
	movslq %esi,%rax
	movl _modifiers(,%rax,8),%eax
	cmpl $0,%eax
	jz L356
L355:
	movl %eax,%r12d
	addq $1,%rbx
	jmp L352
L356:
	movzbl (%rbx),%eax
	cmpl $61,%eax
	jnz L267
L365:
	movslq %esi,%rsi
	movl _modifiers+4(,%rsi,8),%esi
	cmpl $0,%esi
	jz L267
L362:
	movl %esi,%r12d
	addq $1,%rbx
	jmp L352
L296:
	movzbl 1(%r14),%esi
	cmpl $46,%esi
	jnz L298
L300:
	movzbl 2(%r14),%esi
	cmpl $46,%esi
	jnz L298
L297:
	leaq 3(%r14),%rbx
	movl $536870953,%r12d
	jmp L267
L298:
	movzbl (%r14),%esi
	addl $-48,%esi
	cmpl $10,%esi
	jb L310
L305:
	leaq 1(%r14),%rbx
	jmp L267
L310:
	movzbq (%rbx),%rsi
	movl %esi,%edi
	movzbl ___ctype+1(%rdi),%edi
	testl $7,%edi
	jnz L311
L317:
	movzbl %sil,%esi
	cmpl $46,%esi
	jz L311
L313:
	cmpl $95,%esi
	jnz L267
L311:
	movzbl (%rbx),%edi
	call _toupper
	cmpl $69,%eax
	jnz L323
L324:
	movzbl 1(%rbx),%esi
	cmpl $45,%esi
	jz L321
L328:
	cmpl $43,%esi
	jnz L323
L321:
	addq $1,%rbx
L323:
	addq $1,%rbx
	jmp L310
L339:
	movzbq (%rbx),%rsi
	movl %esi,%edi
	movzbl ___ctype+1(%rdi),%edi
	testl $7,%edi
	jnz L340
L342:
	movzbl %sil,%esi
	cmpl $95,%esi
	jnz L267
L340:
	addq $1,%rbx
	jmp L339
L334:
	movzbq (%rbx),%rsi
	movzbl ___ctype+1(%rsi),%esi
	testl $8,%esi
	jz L267
L335:
	addq $1,%rbx
	jmp L334
L371:
	movzbl (%r14),%esi
	andl $255,%esi
	pushq %rsi
	pushq $L372
	call _error
	addq $16,%rsp
L267:
	movl %r12d,%edi
	call _alloc
	movq %rax,%r12
	movq %rbx,%rdx
	subq %r14,%rdx
	leaq 8(%r12),%rdi
	movq %r14,%rsi
	call _vstring_put
	movq %rbx,(%r15)
	movq %r12,%rax
L257:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L387:
_token_paste:
L388:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
	pushq %r12
L407:
	movq %rsi,%rbx
	leaq -24(%rbp),%r12
	xorps %xmm0,%xmm0
	movups %xmm0,-24(%rbp)
	movq $0,-8(%rbp)
	orl $1,-24(%rbp)
	movq %r12,%rsi
	call _token_text
	movq %rbx,%rdi
	movq %r12,%rsi
	call _token_text
	leaq -32(%rbp),%rsi
	movl -24(%rbp),%edi
	testl $1,%edi
	jz L392
L391:
	leaq -23(%rbp),%rdi
	jmp L393
L392:
	movq -8(%rbp),%rdi
L393:
	call _token_scan
	movq %rax,%rbx
	movq -32(%rbp),%rsi
	movzbl (%rsi),%esi
	cmpl $0,%esi
	jnz L394
L397:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jz L396
L394:
	movl -24(%rbp),%esi
	testl $1,%esi
	jz L403
L402:
	leaq -23(%rbp),%rsi
	jmp L404
L403:
	movq -8(%rbp),%rsi
L404:
	pushq %rsi
	pushq $L401
	call _error
	addq $16,%rsp
L396:
	movq %rbx,%rax
L390:
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L409:
_token_same:
L410:
	pushq %rbp
	movq %rsp,%rbp
L411:
	movl (%rdi),%eax
	movl (%rsi),%ecx
	cmpl %ecx,%eax
	jnz L415
L413:
	testl $536870912,%eax
	jz L418
L416:
	movl $1,%eax
	jmp L412
L418:
	movl (%rdi),%eax
	testl $2147483648,%eax
	jz L422
L420:
	movq 8(%rdi),%rdi
	movq 8(%rsi),%rsi
	cmpq %rsi,%rdi
	setz %sil
	movzbl %sil,%eax
	jmp L412
L422:
	addq $8,%rsi
	addq $8,%rdi
	call _vstring_same
	jmp L412
L415:
	xorl %eax,%eax
L412:
	popq %rbp
	ret
L429:
_token_copy:
L430:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L438:
	movq %rdi,%rbx
	movl (%rbx),%edi
	call _alloc
	movq %rax,%r12
	movl (%r12),%esi
	testl $2147483648,%esi
	jz L434
L433:
	movq 8(%rbx),%rsi
	movq %rsi,8(%r12)
	jmp L435
L434:
	leaq 8(%rbx),%rsi
	leaq 8(%r12),%rdi
	call _vstring_concat
L435:
	movq %r12,%rax
L432:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L440:
_token_text:
L441:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L455:
	movq %rsi,%r12
	movq %rdi,%rbx
	movl (%rbx),%esi
	testl $2147483648,%esi
	jz L446
L444:
	pushq $L447
	call _error
	addq $8,%rsp
L446:
	movl 8(%rbx),%esi
	testl $1,%esi
	jz L449
L448:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rdx
	jmp L450
L449:
	movq 16(%rbx),%rdx
L450:
	movl 8(%rbx),%esi
	testl $1,%esi
	jz L452
L451:
	leaq 9(%rbx),%rsi
	jmp L453
L452:
	movq 24(%rbx),%rsi
L453:
	movq %r12,%rdi
	call _vstring_put
L443:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L457:
_token_dequote:
L458:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L476:
	movq %rsi,%r14
	movq %rdi,%rbx
	movl (%rbx),%esi
	cmpl $52,%esi
	jz L463
L461:
	pushq $L464
	call _error
	addq $8,%rsp
L463:
	movl 8(%rbx),%esi
	testl $1,%esi
	jz L466
L465:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%r12
	jmp L467
L466:
	movq 16(%rbx),%r12
L467:
	movl 8(%rbx),%esi
	testl $1,%esi
	jz L469
L468:
	addq $9,%rbx
	jmp L470
L469:
	movq 24(%rbx),%rbx
L470:
	movl $1,%r13d
L471:
	leaq -1(%r12),%rsi
	cmpq %rsi,%r13
	jae L460
L472:
	addq $1,%rbx
	movzbl (%rbx),%esi
	movq %r14,%rdi
	call _vstring_putc
	addq $1,%r13
	jmp L471
L460:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L478:
_list_cut:
L479:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L496:
	movq %rdi,%rbx
	movq %rsi,%r12
L482:
	movq (%rbx),%rdi
	cmpq $0,%rdi
	jz L481
L483:
	cmpq %rdi,%r12
	jz L481
L489:
	movq 32(%rdi),%rsi
	cmpq $0,%rsi
	jz L493
L492:
	movq 40(%rdi),%rax
	movq %rax,40(%rsi)
	jmp L494
L493:
	movq 40(%rdi),%rsi
	movq %rsi,8(%rbx)
L494:
	movq 32(%rdi),%rsi
	movq 40(%rdi),%rax
	movq %rsi,(%rax)
	call _token_free
	jmp L482
L481:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L498:
_list_skip_spaces:
L499:
	pushq %rbp
	movq %rsp,%rbp
L511:
	movq %rdi,%rax
L502:
	cmpq $0,%rax
	jz L501
L505:
	movl (%rax),%esi
	cmpl $48,%esi
	jnz L501
L503:
	movq 32(%rax),%rax
	jmp L502
L501:
	popq %rbp
	ret
L513:
_list_fold_spaces:
L514:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L538:
	movq %rdi,%r12
	movq (%r12),%rbx
L517:
	cmpq $0,%rbx
	jz L516
L518:
	movl (%rbx),%esi
	cmpl $48,%esi
	jnz L519
L521:
	leaq 8(%rbx),%r13
	movq %r13,%rdi
	call _vstring_free
	movq %r13,%rdi
	call _vstring_init
	movq %r13,%rdi
	movl $32,%esi
	call _vstring_putc
L524:
	movq 32(%rbx),%rdi
	cmpq $0,%rdi
	jz L519
L527:
	movl (%rdi),%esi
	cmpl $48,%esi
	jnz L519
L531:
	movq 32(%rdi),%rsi
	cmpq $0,%rsi
	jz L535
L534:
	movq 40(%rdi),%rax
	movq %rax,40(%rsi)
	jmp L536
L535:
	movq 40(%rdi),%rsi
	movq %rsi,8(%r12)
L536:
	movq 32(%rdi),%rsi
	movq 40(%rdi),%rax
	movq %rsi,(%rax)
	call _token_free
	jmp L524
L519:
	movq 32(%rbx),%rbx
	jmp L517
L516:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L540:
_list_strip_ends:
L541:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L566:
	movq %rdi,%rbx
L544:
	movq (%rbx),%rsi
	movq %rsi,%rdi
	cmpq $0,%rsi
	jz L547
L551:
	movl (%rsi),%esi
	cmpl $48,%esi
	jz L559
L547:
	movq 8(%rbx),%rsi
	movq 8(%rsi),%rsi
	movq (%rsi),%rsi
	movq %rsi,%rdi
	cmpq $0,%rsi
	jz L543
L555:
	movl (%rsi),%esi
	cmpl $48,%esi
	jnz L543
L559:
	movq 32(%rdi),%rsi
	cmpq $0,%rsi
	jz L563
L562:
	movq 40(%rdi),%rax
	movq %rax,40(%rsi)
	jmp L564
L563:
	movq 40(%rdi),%rsi
	movq %rsi,8(%rbx)
L564:
	movq 32(%rdi),%rsi
	movq 40(%rdi),%rax
	movq %rsi,(%rax)
	call _token_free
	jmp L544
L543:
	popq %rbx
	popq %rbp
	ret
L568:
_list_strip_all:
L569:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L596:
	movq %rdi,%rbx
	movq (%rbx),%rdi
L572:
	cmpq $0,%rdi
	jz L571
L573:
	movq 32(%rdi),%r12
	movl (%rdi),%esi
	cmpl $48,%esi
	jz L589
L578:
	cmpl $1073741883,%esi
	jnz L577
L582:
	movl 8(%rdi),%esi
	testl $1,%esi
	jz L587
L586:
	shll $24,%esi
	sarl $25,%esi
	movslq %esi,%rsi
	jmp L588
L587:
	movq 16(%rdi),%rsi
L588:
	cmpq $0,%rsi
	jnz L577
L589:
	movq 32(%rdi),%rsi
	cmpq $0,%rsi
	jz L593
L592:
	movq 40(%rdi),%rax
	movq %rax,40(%rsi)
	jmp L594
L593:
	movq 40(%rdi),%rsi
	movq %rsi,8(%rbx)
L594:
	movq 32(%rdi),%rsi
	movq 40(%rdi),%rax
	movq %rsi,(%rax)
	call _token_free
L577:
	movq %r12,%rdi
	jmp L572
L571:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L598:
_list_strip_around:
L599:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L617:
	movq %rdi,%r12
	movq %rsi,%rbx
L602:
	movq 40(%rbx),%rsi
	movq 8(%rsi),%rsi
	movq (%rsi),%rsi
	cmpq $0,%rsi
	jz L609
L605:
	movl (%rsi),%edi
	cmpl $48,%edi
	jnz L609
L603:
	movq %r12,%rdi
	call _list_drop
	jmp L602
L609:
	movq 32(%rbx),%rsi
	cmpq $0,%rsi
	jz L601
L612:
	movl (%rsi),%edi
	cmpl $48,%edi
	jnz L601
L610:
	movq %r12,%rdi
	call _list_drop
	jmp L609
L601:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L619:
_list_pop:
L620:
	pushq %rbp
	movq %rsp,%rbp
L633:
	movq %rdi,%rcx
	movq (%rcx),%rdi
	movq 32(%rdi),%rax
	cmpq $0,%rax
	jz L627
L626:
	movq 40(%rdi),%rcx
	movq %rcx,40(%rax)
	jmp L628
L627:
	movq 40(%rdi),%rax
	movq %rax,8(%rcx)
L628:
	movq 32(%rdi),%rax
	movq 40(%rdi),%rcx
	movq %rax,(%rcx)
	cmpq $0,%rsi
	jz L630
L629:
	movq %rdi,(%rsi)
	jmp L622
L630:
	call _token_free
L622:
	popq %rbp
	ret
L635:
_list_drop:
L636:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L647:
	movq %rdi,%rax
	movq %rsi,%rdi
	movq 32(%rdi),%rbx
	cmpq $0,%rbx
	jz L643
L642:
	movq 40(%rdi),%rsi
	movq %rsi,40(%rbx)
	jmp L644
L643:
	movq 40(%rdi),%rsi
	movq %rsi,8(%rax)
L644:
	movq 32(%rdi),%rsi
	movq 40(%rdi),%rax
	movq %rsi,(%rax)
	call _token_free
	movq %rbx,%rax
L638:
	popq %rbx
	popq %rbp
	ret
L649:
_list_match:
L650:
	pushq %rbp
	movq %rsp,%rbp
L662:
	movl %esi,%eax
	movq %rdx,%rsi
	movq (%rdi),%rcx
	cmpq $0,%rcx
	jz L654
L656:
	movl (%rcx),%ecx
	cmpl %eax,%ecx
	jnz L654
L653:
	call _list_pop
	jmp L652
L654:
	pushq $L660
	call _error
	addq $8,%rsp
L652:
	popq %rbp
	ret
L664:
_list_same:
L665:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L666:
	movq (%rdi),%rbx
	movq (%rsi),%r12
L668:
	cmpq $0,%rbx
	jz L670
L675:
	cmpq $0,%r12
	jz L670
L671:
	movq %rbx,%rdi
	movq %r12,%rsi
	call _token_same
	cmpl $0,%eax
	jz L670
L669:
	movq 32(%rbx),%rbx
	movq 32(%r12),%r12
	jmp L668
L670:
	cmpq $0,%rbx
	jnz L679
L682:
	cmpq $0,%r12
	jz L680
L679:
	xorl %eax,%eax
	jmp L667
L680:
	movl $1,%eax
L667:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L691:
_list_normalize:
L692:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L718:
	movq %rdi,%r15
	movq %rsi,%rbx
	movq %r15,%rdi
	call _list_strip_ends
	movq %r15,%rdi
	call _list_fold_spaces
	movq (%r15),%rsi
L695:
	cmpq $0,%rsi
	jz L698
L696:
	movl (%rsi),%edi
	cmpl $1610612748,%edi
	jnz L701
L699:
	movl $1610612790,(%rsi)
L701:
	movl (%rsi),%edi
	cmpl $1610612778,%edi
	jnz L697
L702:
	movl $1610612791,(%rsi)
L697:
	movq 32(%rsi),%rsi
	jmp L695
L698:
	movq (%rbx),%r12
	xorl %r14d,%r14d
L705:
	cmpq $0,%r12
	jz L694
L706:
	movq (%r15),%rbx
	movslq %r14d,%r13
L709:
	cmpq $0,%rbx
	jz L707
L710:
	movq %r12,%rdi
	movq %rbx,%rsi
	call _token_same
	cmpl $0,%eax
	jz L711
L713:
	leaq 8(%rbx),%rdi
	call _vstring_free
	movl $-2147483590,(%rbx)
	movq %r13,8(%rbx)
L711:
	movq 32(%rbx),%rbx
	jmp L709
L707:
	movq 32(%r12),%r12
	addl $1,%r14d
	jmp L705
L694:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L720:
_list_stringize:
L721:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L759:
	movq %rdi,%r14
	movl $52,%edi
	call _alloc
	movq %rax,%r13
	leaq 8(%rax),%rdi
	movl $34,%esi
	call _vstring_putc
	movq (%r14),%rbx
L724:
	cmpq $0,%rbx
	jz L727
L725:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jz L730
L728:
	pushq $L731
	call _error
	addq $8,%rsp
L730:
	movl (%rbx),%esi
	cmpl $48,%esi
	jnz L734
L735:
	cmpq %rbx,(%r14)
	jz L726
L739:
	cmpq $0,32(%rbx)
	jz L726
L734:
	movl 8(%rbx),%esi
	testl $1,%esi
	jz L745
L744:
	leaq 9(%rbx),%r12
	jmp L747
L745:
	movq 24(%rbx),%r12
L747:
	movzbl (%r12),%esi
	cmpl $0,%esi
	jz L726
L748:
	movl (%rbx),%esi
	cmpl $52,%esi
	jz L750
L753:
	cmpl $53,%esi
	jnz L751
L750:
	movzbl (%r12),%esi
	addq $1,%r12
	leaq 8(%r13),%rdi
	call _backslash
	jmp L747
L751:
	movzbl (%r12),%esi
	addq $1,%r12
	leaq 8(%r13),%rdi
	call _vstring_putc
	jmp L747
L726:
	movq 32(%rbx),%rbx
	jmp L724
L727:
	leaq 8(%r13),%rdi
	movl $34,%esi
	call _vstring_putc
	movq %r13,%rax
L723:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L761:
_list_ennervate:
L762:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L777:
	movq %rsi,%r12
	movq (%rdi),%rbx
L765:
	cmpq $0,%rbx
	jz L764
L766:
	movl (%rbx),%esi
	cmpl $49,%esi
	jnz L767
L772:
	leaq 8(%rbx),%rdi
	movq %r12,%rsi
	call _vstring_same
	cmpl $0,%eax
	jz L767
L769:
	movl $1073741883,(%rbx)
L767:
	movq 32(%rbx),%rbx
	jmp L765
L764:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L779:
_list_copy:
L780:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L791:
	movq %rdi,%r12
	movq (%rsi),%rbx
L783:
	cmpq $0,%rbx
	jz L782
L784:
	movq %rbx,%rdi
	call _token_copy
	leaq 32(%rax),%rsi
	movq $0,32(%rax)
	movq 8(%r12),%rdi
	movq %rdi,40(%rax)
	movq 8(%r12),%rdi
	movq %rax,(%rdi)
	movq %rsi,8(%r12)
	movq 32(%rbx),%rbx
	jmp L783
L782:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L793:
_list_move:
L794:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L814:
	movq %rsi,%r14
	movq %rdi,%rbx
	movl %edx,%r13d
L797:
	movl %r13d,%esi
	addl $-1,%r13d
	cmpl $0,%esi
	jz L796
L798:
	movq (%r14),%r12
	cmpq $0,%r12
	jnz L804
L800:
	pushq $L803
	call _error
	addq $8,%rsp
L804:
	movq 32(%r12),%rsi
	cmpq $0,%rsi
	jz L808
L807:
	movq 40(%r12),%rdi
	movq %rdi,40(%rsi)
	jmp L809
L808:
	movq 40(%r12),%rsi
	movq %rsi,8(%r14)
L809:
	leaq 32(%r12),%rsi
	movq 32(%r12),%rdi
	movq 40(%r12),%rax
	movq %rdi,(%rax)
	movq $0,32(%r12)
	movq 8(%rbx),%rdi
	movq %rdi,40(%r12)
	movq 8(%rbx),%rdi
	movq %r12,(%rdi)
	movq %rsi,8(%rbx)
	jmp L797
L796:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L816:
_list_next_is:
L817:
	pushq %rbp
	movq %rsp,%rbp
L818:
	movq 32(%rsi),%rsi
L820:
	cmpq $0,%rsi
	jz L822
L823:
	movl (%rsi),%edi
	cmpl $48,%edi
	jnz L822
L821:
	movq 32(%rsi),%rsi
	jmp L820
L822:
	cmpq $0,%rsi
	jz L828
L830:
	movl (%rsi),%esi
	cmpl %edx,%esi
	jnz L828
L827:
	movl $1,%eax
	jmp L819
L828:
	xorl %eax,%eax
L819:
	popq %rbp
	ret
L839:
_list_prev_is:
L840:
	pushq %rbp
	movq %rsp,%rbp
L841:
	movq 40(%rsi),%rsi
	movq 8(%rsi),%rsi
	movq (%rsi),%rsi
L843:
	cmpq $0,%rsi
	jz L845
L846:
	movl (%rsi),%edi
	cmpl $48,%edi
	jnz L845
L844:
	movq 40(%rsi),%rsi
	movq 8(%rsi),%rsi
	movq (%rsi),%rsi
	jmp L843
L845:
	cmpq $0,%rsi
	jz L851
L853:
	movl (%rsi),%esi
	cmpl %edx,%esi
	jnz L851
L850:
	movl $1,%eax
	jmp L842
L851:
	xorl %eax,%eax
L842:
	popq %rbp
	ret
L862:
_list_insert:
L863:
	pushq %rbp
	movq %rsp,%rbp
L864:
	cmpq $0,%rsi
	jz L872
L869:
	movq 40(%rsi),%rdi
	movq %rdi,40(%rdx)
	leaq 32(%rdx),%rdi
	movq %rsi,32(%rdx)
	movq 40(%rsi),%rax
	movq %rdx,(%rax)
	movq %rdi,40(%rsi)
	jmp L865
L872:
	leaq 32(%rdx),%rsi
	movq $0,32(%rdx)
	movq 8(%rdi),%rax
	movq %rax,40(%rdx)
	movq 8(%rdi),%rax
	movq %rdx,(%rax)
	movq %rsi,8(%rdi)
L865:
	popq %rbp
	ret
L878:
_list_insert_list:
L879:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L892:
	movq %rdx,%rbx
	movq %rdi,%r13
	movq %rsi,%r12
L882:
	movq (%rbx),%rdx
	cmpq $0,%rdx
	jz L881
L885:
	movq 32(%rdx),%rsi
	cmpq $0,%rsi
	jz L889
L888:
	movq 40(%rdx),%rdi
	movq %rdi,40(%rsi)
	jmp L890
L889:
	movq 40(%rdx),%rsi
	movq %rsi,8(%rbx)
L890:
	movq 32(%rdx),%rsi
	movq 40(%rdx),%rdi
	movq %rsi,(%rdi)
	movq %r13,%rdi
	movq %r12,%rsi
	call _list_insert
	jmp L882
L881:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L894:
_list_placeholder:
L895:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L905:
	movq %rdi,%rbx
	cmpq $0,(%rbx)
	jnz L897
L898:
	movl $1073741883,%edi
	call _alloc
	leaq 32(%rax),%rsi
	movq $0,32(%rax)
	movq 8(%rbx),%rdi
	movq %rdi,40(%rax)
	movq 8(%rbx),%rdi
	movq %rax,(%rdi)
	movq %rsi,8(%rbx)
L897:
	popq %rbx
	popq %rbp
	ret
L907:
L372:
	.byte 105,110,118,97,108,105,100,32
	.byte 99,104,97,114,97,99,116,101
	.byte 114,32,40,65,83,67,73,73
	.byte 32,37,100,41,32,105,110,32
	.byte 105,110,112,117,116,0
L803:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,108,105
	.byte 115,116,95,109,111,118,101,0
L660:
	.byte 115,121,110,116,97,120,0
L282:
	.byte 117,110,116,101,114,109,105,110
	.byte 97,116,101,100,32,99,104,97
	.byte 114,32,99,111,110,115,116,97
	.byte 110,116,0
L106:
	.byte 109,117,108,116,105,45,99,104
	.byte 97,114,97,99,116,101,114,32
	.byte 99,111,110,115,116,97,110,116
	.byte 115,32,117,110,115,117,112,112
	.byte 111,114,116,101,100,0
L88:
	.byte 109,97,108,102,111,114,109,101
	.byte 100,32,105,110,116,101,103,101
	.byte 114,32,99,111,110,115,116,97
	.byte 110,116,0
L26:
	.byte 37,100,0
L102:
	.byte 105,110,118,97,108,105,100,32
	.byte 101,115,99,97,112,101,32,115
	.byte 101,113,117,101,110,99,101,0
L281:
	.byte 117,110,116,101,114,109,105,110
	.byte 97,116,101,100,32,115,116,114
	.byte 105,110,103,32,108,105,116,101
	.byte 114,97,108,0
L731:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,99,97
	.byte 110,39,116,32,115,116,114,105
	.byte 110,103,105,122,101,32,97,32
	.byte 116,101,120,116,108,101,115,115
	.byte 32,116,111,107,101,110,0
L464:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,99,97
	.byte 110,39,116,32,100,101,113,117
	.byte 111,116,101,32,110,111,110,45
	.byte 115,116,114,105,110,103,32,116
	.byte 111,107,101,110,0
L447:
	.byte 67,80,80,32,73,78,84,69
	.byte 82,78,65,76,58,32,99,97
	.byte 110,39,116,32,103,101,116,32
	.byte 116,101,120,116,32,111,102,32
	.byte 110,111,110,45,116,101,120,116
	.byte 32,116,111,107,101,110,0
L401:
	.byte 114,101,115,117,108,116,32,111
	.byte 102,32,112,97,115,116,101,32
	.byte 40,35,35,41,32,39,37,115
	.byte 39,32,105,115,32,110,111,116
	.byte 32,97,32,116,111,107,101,110
	.byte 0
.globl _list_drop
.globl _list_pop
.globl _list_placeholder
.globl _token_convert_char
.globl _token_convert_number
.globl _token_number
.globl _error
.globl _toupper
.globl _list_fold_spaces
.globl _list_skip_spaces
.globl _list_prev_is
.globl _list_next_is
.globl _vstring_putc
.globl _list_insert_list
.globl _list_strip_around
.globl _list_cut
.globl _vstring_concat
.globl _vstring_init
.globl _vstring_put
.globl _list_ennervate
.globl _list_move
.globl _list_stringize
.globl _list_normalize
.globl _token_separate
.globl _token_dequote
.globl _token_paste
.globl _escape
.globl ___ctype
.globl _sprintf
.globl _token_string
.globl _list_strip_all
.globl _errno
.globl _list_strip_ends
.globl _vstring_puts
.globl _safe_malloc
.globl _list_insert
.globl _token_text
.globl _token_int
.globl _list_same
.globl _token_space
.globl _token_same
.globl _token_free
.globl _vstring_same
.globl _vstring_free
.globl _list_match
.globl _list_copy
.globl _token_copy
.globl _strtoul
.globl _token_scan
