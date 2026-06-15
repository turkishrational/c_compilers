.data
.align 4
_current_scope:
	.int 2
.local L6
.comm L6, 4, 4
.text
_symbol_new:
L3:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L33:
	movl %esi,%r12d
	movq %rdi,%r13
	movl $112,%edi
	call _safe_malloc
	movq %rax,%rbx
	movq %r13,(%rbx)
	movl %r12d,12(%rbx)
	movl _current_scope(%rip),%esi
	movl %esi,8(%rbx)
	movq _error_path(%rip),%rsi
	movq %rsi,24(%rbx)
	movl _error_line_no(%rip),%esi
	movl %esi,20(%rbx)
	movl L6(%rip),%esi
	addl $1,%esi
	movl %esi,L6(%rip)
	movl %esi,16(%rbx)
	movl $2147483649,48(%rbx)
	cmpl $1,%r12d
	jb L28
L29:
	cmpl $2,%r12d
	jbe L14
L28:
	cmpl $4,%r12d
	jz L14
L30:
	cmpl $16,%r12d
	jz L20
L31:
	cmpl $2048,%r12d
	jnz L21
L18:
	call _block_new
	movq %rax,64(%rbx)
	jmp L9
L20:
	movl _last_asm_label(%rip),%esi
	addl $1,%esi
	movl %esi,_last_asm_label(%rip)
	movl %esi,64(%rbx)
L21:
	leaq 32(%rbx),%rsi
	movq $0,32(%rbx)
	movq %rsi,40(%rbx)
	movl $2147483649,48(%rbx)
	movl $2147483649,56(%rbx)
	jmp L9
L14:
	movq $0,48(%rbx)
L9:
	movq %rbx,%rax
L5:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L35:
_symbol_free:
L36:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L52:
	movq %rdi,%rbx
	movl 12(%rbx),%esi
	testl $7,%esi
	jnz L41
L39:
	leaq 32(%rbx),%rdi
	call _type_clear
	movl 48(%rbx),%esi
	cmpl $2147483649,%esi
	jz L41
L45:
	movq 96(%rbx),%rsi
	cmpq $0,%rsi
	jz L50
L48:
	movq 104(%rbx),%rdi
	movq %rdi,104(%rsi)
L50:
	movq 96(%rbx),%rsi
	movq 104(%rbx),%rdi
	movq %rsi,(%rdi)
L41:
	movq %rbx,%rdi
	call _free
L38:
	popq %rbx
	popq %rbp
	ret
L54:
_symbol_here:
L55:
	pushq %rbp
	movq %rsp,%rbp
L56:
	movq _error_path(%rip),%rsi
	movq %rsi,24(%rdi)
	movl _error_line_no(%rip),%esi
	movl %esi,20(%rdi)
L57:
	popq %rbp
	ret
L61:
_members_append:
L63:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L96:
	movq %rsi,%rbx
	cmpq $0,(%rbx)
	jnz L68
L66:
	movq %rbx,%rdi
	call _symbol_free
	jmp L65
L68:
	movq (%rdi),%rsi
	movq %rsi,%r12
	cmpq $0,%rsi
	jnz L77
L73:
	movq $0,72(%rbx)
	movq %rbx,(%rdi)
	jmp L72
L77:
	movq (%rbx),%rsi
	movq (%r12),%rdi
	cmpq %rdi,%rsi
	jnz L82
L80:
	pushq %r12
	pushq %rdi
	pushq $L83
	pushq $1
	call _error
	addq $32,%rsp
L82:
	movq 32(%r12),%rsi
	movq (%rsi),%rsi
	movq $2305843009213693952,%rdi
	testq %rdi,%rsi
	jz L86
L84:
	pushq $L87
	pushq $1
	call _error
	addq $16,%rsp
L86:
	movq 72(%r12),%rsi
	cmpq $0,%rsi
	jz L92
L88:
	movq %rsi,%r12
	jmp L77
L92:
	movq $0,72(%rbx)
	movq %rbx,72(%r12)
L72:
	movl _current_scope(%rip),%esi
	movq %rbx,%rdi
	call _symbol_insert
L65:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L98:
_members_absorb:
L100:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L108:
	movq %rdi,%r13
	movq %rdx,%r14
	movq 48(%rsi),%rbx
L103:
	cmpq $0,%rbx
	jz L102
L104:
	movq (%rbx),%rdi
	movl $1024,%esi
	call _symbol_new
	movq %rax,%r12
	leaq 32(%rbx),%rsi
	leaq 32(%r12),%rdi
	call _type_copy
	movl 64(%rbx),%esi
	addl %r14d,%esi
	movl %esi,64(%r12)
	leaq 48(%r13),%rdi
	movq %r12,%rsi
	call _members_append
	movq 72(%rbx),%rbx
	jmp L103
L102:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L110:
_member_insert:
L111:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L159:
	movq %rdx,%r14
	movq %rsi,%r15
	movq %rdi,%r12
	movq %r15,%rdi
	movl $1024,%esi
	call _symbol_new
	movq %rax,-8(%rbp)	 # spill
	movq -8(%rbp),%r10	 # spill
	leaq 32(%r10),%rdi
	movq %r14,%rsi
	call _type_copy
	movq (%r14),%rsi
	movq (%rsi),%rdi
	testq $8192,%rdi
	jz L115
L117:
	cmpq $0,8(%rsi)
	jnz L115
L114:
	xorl %r10d,%r10d
	movq %r10,-16(%rbp)	 # spill
	movq -8(%rbp),%r10	 # spill
	movq 32(%r10),%rsi
	movq (%rsi),%rdi
	movq $2305843009213693952,%rax
	orq %rax,%rdi
	movq %rdi,(%rsi)
	jmp L116
L115:
	movq %r14,%rdi
	xorl %esi,%esi
	call _type_sizeof
	movq %rax,%rsi
	shlq $3,%rsi
	movq %rsi,-16(%rbp)	 # spill
L116:
	movq %r14,%rdi
	call _type_alignof
	movl %eax,%r13d
	movl %r13d,%ebx
	movl 12(%r12),%esi
	testl $2,%esi
	jz L122
L121:
	xorl %edi,%edi
	jmp L123
L122:
	movq 32(%r12),%rsi
	movq %rsi,%rdi
L123:
	movq (%r14),%rsi
	movq (%rsi),%rsi
	movl $2147483648,%eax
	testq %rax,%rsi
	jz L125
L124:
	movq $34909494181888,%rax
	andq %rax,%rsi
	shrq $38,%rsi
	movq %rsi,%rcx
	cmpq $0,%rsi
	jz L127
L130:
	movq %rdi,%rax
	xorl %edx,%edx
	movq -16(%rbp),%r10	 # spill
	divq %r10
	imulq -16(%rbp),%rax	 # spill
	movq %rax,%r8
	leaq -1(%rdi,%rsi),%rsi
	movq %rsi,%rax
	xorl %edx,%edx
	movq -16(%rbp),%r10	 # spill
	divq %r10
	movq %rax,%rsi
	imulq -16(%rbp),%rsi	 # spill
	cmpq %rsi,%r8
	jz L129
L127:
	movq -16(%rbp),%r10	 # spill
	leaq -1(%rdi,%r10),%rsi
	movq %rsi,%rax
	xorl %edx,%edx
	movq -16(%rbp),%r10	 # spill
	divq %r10
	movq %rax,%rsi
	imulq -16(%rbp),%rsi	 # spill
	movq %rsi,%rdi
L129:
	movq %rdi,%rax
	xorl %edx,%edx
	movq -16(%rbp),%r10	 # spill
	divq %r10
	movq %rdx,%rsi
	shlq $32,%rsi
	movq $270582939648,%rax
	andq %rax,%rsi
	movq -8(%rbp),%r10	 # spill
	movq 32(%r10),%rax
	orq %rsi,(%rax)
	jmp L126
L125:
	leal (,%r13,8),%esi
	leal -1(,%r13,8),%eax
	movslq %eax,%rax
	movslq %esi,%rsi
	addq %rax,%rdi
	movq %rdi,%rax
	cqto
	idivq %rsi
	movq %rax,%rdi
	imulq %rsi,%rdi
	movq -16(%rbp),%rcx	 # spill
L126:
	movl $8,%esi
	movq %rdi,%rax
	cqto
	idivq %rsi
	movslq %r13d,%rsi
	cqto
	idivq %rsi
	movq %rax,%rsi
	imull %r13d,%esi
	movq -8(%rbp),%r10	 # spill
	movl %esi,64(%r10)
	movl 12(%r12),%esi
	testl $2,%esi
	jz L135
L134:
	movq 32(%r12),%rsi
	cmpq %rcx,%rsi
	ja L139
L138:
	movq %rcx,%rsi
L139:
	movq %rsi,32(%r12)
	jmp L136
L135:
	addq %rcx,%rdi
	movq %rdi,32(%r12)
L136:
	cmpq $1073741824,%rdi
	jle L142
L140:
	pushq %r12
	pushq $L143
	pushq $1
	call _error
	addq $24,%rsp
L142:
	movl 40(%r12),%esi
	cmpl %r13d,%esi
	jg L146
L145:
	movl %ebx,%esi
L146:
	movl %esi,40(%r12)
	movq (%r14),%rsi
	movq (%rsi),%rdi
	testq $65536,%rdi
	jz L149
L154:
	movq 8(%rsi),%rsi
	cmpq $0,(%rsi)
	jnz L149
L150:
	cmpq $0,%r15
	jnz L149
L147:
	movq -8(%rbp),%r10	 # spill
	movl 64(%r10),%edi
	movslq %edi,%rax
	movq %r12,%rdi
	movq %rax,%rdx
	call _members_absorb
L149:
	leaq 48(%r12),%rdi
	movq -8(%rbp),%rsi	 # spill
	call _members_append
L113:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L161:
_member_lookup:
L162:
	pushq %rbp
	movq %rsp,%rbp
L163:
	movq 48(%rdi),%rax
L165:
	cmpq $0,%rax
	jz L164
L166:
	cmpq (%rax),%rsi
	jz L164
L167:
	movq 72(%rax),%rax
	jmp L165
L164:
	popq %rbp
	ret
L177:
_symbol_complete_strun:
L178:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L186:
	movq %rdi,%rbx
	cmpq $0,32(%rbx)
	jnz L183
L181:
	pushq %rbx
	pushq $L184
	pushq $1
	call _error
	addq $24,%rsp
L183:
	movq 32(%rbx),%rdi
	movl 40(%rbx),%eax
	leal (,%rax,8),%esi
	leal -1(,%rax,8),%eax
	movslq %eax,%rax
	movslq %esi,%rsi
	addq %rdi,%rax
	xorl %edx,%edx
	divq %rsi
	imulq %rsi,%rax
	movq %rax,32(%rbx)
	shrq $3,%rax
	movq %rax,32(%rbx)
	orl $-2147483648,12(%rbx)
L180:
	popq %rbx
	popq %rbp
	ret
L188:
_symbol_insert:
L189:
	pushq %rbp
	movq %rsp,%rbp
L190:
	movl %esi,8(%rdi)
	movq (%rdi),%rax
	cmpq $0,%rax
	jz L193
L192:
	movl (%rax),%eax
	andl $31,%eax
	movl %eax,%eax
	shlq $4,%rax
	addq $_buckets,%rax
	jmp L194
L193:
	movq $_buckets+512,%rax
L194:
	movq %rax,%rcx
	movq (%rax),%rax
L195:
	cmpq $0,%rax
	jz L206
L196:
	movl 8(%rax),%edx
	cmpl %esi,%edx
	jg L197
L202:
	movq 88(%rax),%rsi
	movq %rsi,88(%rdi)
	leaq 80(%rdi),%rsi
	movq %rax,80(%rdi)
	movq 88(%rax),%rcx
	movq %rdi,(%rcx)
	movq %rsi,88(%rax)
	jmp L191
L197:
	movq 80(%rax),%rax
	jmp L195
L206:
	leaq 80(%rdi),%rsi
	movq $0,80(%rdi)
	movq 8(%rcx),%rax
	movq %rax,88(%rdi)
	movq 8(%rcx),%rax
	movq %rdi,(%rax)
	movq %rsi,8(%rcx)
L191:
	popq %rbp
	ret
L212:
_symbol_remove:
L214:
	pushq %rbp
	movq %rsp,%rbp
L215:
	movq (%rdi),%rsi
	cmpq $0,%rsi
	jz L218
L217:
	movl (%rsi),%esi
	andl $31,%esi
	movl %esi,%esi
	shlq $4,%rsi
	leaq _buckets(%rsi),%rax
	jmp L220
L218:
	movq $_buckets+512,%rax
L220:
	movq 80(%rdi),%rsi
	cmpq $0,%rsi
	jz L224
L223:
	movq 88(%rdi),%rax
	movq %rax,88(%rsi)
	jmp L225
L224:
	movq 88(%rdi),%rsi
	movq %rsi,8(%rax)
L225:
	movq 80(%rdi),%rsi
	movq 88(%rdi),%rdi
	movq %rsi,(%rdi)
L216:
	popq %rbp
	ret
L229:
_symbol_move:
L231:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L235:
	movl %esi,%r12d
	movq %rdi,%rbx
	call _symbol_remove
	movq %rbx,%rdi
	movl %r12d,%esi
	call _symbol_insert
L233:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L237:
_symbol_init:
L238:
	pushq %rbp
	movq %rsp,%rbp
L239:
	xorl %esi,%esi
L245:
	movslq %esi,%rdi
	shlq $4,%rdi
	leaq _buckets(%rdi),%rax
	movq $0,_buckets(%rdi)
	movq %rax,_buckets+8(%rdi)
	addl $1,%esi
	cmpl $32,%esi
	jle L245
L244:
	xorl %edi,%edi
L252:
	movslq %edi,%rsi
	movq $0,_reg_buckets(,%rsi,8)
	addl $1,%edi
	cmpl $16,%edi
	jl L252
L240:
	popq %rbp
	ret
L258:
_scope_enter:
L259:
	pushq %rbp
	movq %rsp,%rbp
L267:
	movl _current_scope(%rip),%esi
	cmpl $1000,%esi
	jnz L264
L262:
	pushq $L265
	pushq $1
	call _error
	addq $16,%rsp
L264:
	addl $1,_current_scope(%rip)
L261:
	popq %rbp
	ret
L269:
_symbol_lookup:
L270:
	pushq %rbp
	movq %rsp,%rbp
L271:
	cmpq $0,%rdx
	jz L274
L273:
	movl (%rdx),%eax
	andl $31,%eax
	movl %eax,%eax
	shlq $4,%rax
	addq $_buckets,%rax
	jmp L275
L274:
	movq $_buckets+512,%rax
L275:
	movq (%rax),%rax
L276:
	cmpq $0,%rax
	jz L279
L277:
	movq 80(%rax),%r8
	movl 8(%rax),%r9d
	cmpl %esi,%r9d
	jl L279
L282:
	cmpl %edi,%r9d
	jg L278
L286:
	movl 12(%rax),%r9d
	testl $268435456,%r9d
	jz L290
L288:
	movq 64(%rax),%rax
L290:
	cmpq (%rax),%rdx
	jnz L278
L294:
	movl 12(%rax),%r9d
	testl %ecx,%r9d
	jnz L272
L278:
	movq %r8,%rax
	jmp L276
L279:
	xorl %eax,%eax
L272:
	popq %rbp
	ret
L303:
_symbol_typename:
L304:
	pushq %rbp
	movq %rsp,%rbp
L317:
	movq %rdi,%rdx
	movl _current_scope(%rip),%edi
	movl $2,%esi
	movl $268436472,%ecx
	call _symbol_lookup
	cmpq $0,%rax
	jz L309
L310:
	movl 12(%rax),%esi
	testl $8,%esi
	jnz L306
L309:
	xorl %eax,%eax
L306:
	popq %rbp
	ret
L319:
_symbol_check_labels:
L320:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L323:
	xorl %ebx,%ebx
L327:
	movslq %ebx,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rsi
L330:
	cmpq $0,%rsi
	jz L328
L331:
	movq 80(%rsi),%r12
	movl 8(%rsi),%edi
	cmpl $1001,%edi
	jl L328
L336:
	cmpl $1001,%edi
	jg L332
L340:
	movl 12(%rsi),%edi
	testl $2147483648,%edi
	jnz L332
L342:
	movq (%rsi),%rdi
	pushq %rsi
	pushq %rdi
	pushq $L345
	pushq $1
	call _error
	addq $32,%rsp
L332:
	movq %r12,%rsi
	jmp L330
L328:
	addl $1,%ebx
	cmpl $31,%ebx
	jle L327
L322:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L349:
_symbol_finalize:
L350:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L351:
	movl _debug_flag_s(%rip),%esi
	cmpl $0,%esi
	jz L356
L353:
	xorl %edi,%edi
	call _scope_debug
	movl $1,%edi
	call _scope_debug
	movl $2,%edi
	call _scope_debug
L356:
	xorl %r13d,%r13d
L360:
	movslq %r13d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rbx
L363:
	cmpq $0,%rbx
	jz L361
L364:
	movq 80(%rbx),%r12
	movl 8(%rbx),%esi
	cmpl $1,%esi
	jl L361
L369:
	cmpl $2,%esi
	jg L365
L373:
	movl 12(%rbx),%esi
	testl $536870912,%esi
	jz L377
L375:
	movq %rbx,%rdi
	call _init_bss
L377:
	movl 12(%rbx),%esi
	testl $32,%esi
	jz L365
L381:
	movl 12(%rbx),%esi
	testl $3221225472,%esi
	jz L365
L378:
	pushq %rbx
	pushq $L385
	call _output
	addq $16,%rsp
L365:
	movq %r12,%rbx
	jmp L363
L361:
	addl $1,%r13d
	cmpl $31,%r13d
	jle L360
L352:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L389:
_symbol_function_class:
L390:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L412:
	movq %rsi,%rbx
	movq (%rdi),%rsi
	movq (%rsi),%rsi
	andq $131071,%rsi
	cmpq $16384,%rsi
	jnz L392
L393:
	movl (%rbx),%esi
	testl $192,%esi
	jnz L396
L399:
	testl $16,%esi
	jz L398
L403:
	movl _current_scope(%rip),%esi
	cmpl $2,%esi
	jz L398
L396:
	pushq $L407
	pushq $1
	call _error
	addq $16,%rsp
L398:
	movl (%rbx),%esi
	testl $48,%esi
	jnz L392
L408:
	orl $32,%esi
	movl %esi,(%rbx)
L392:
	popq %rbx
	popq %rbp
	ret
L414:
_symbol_global:
L415:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L446:
	movl %esi,%r13d
	movq %rdx,%r15
	movl %ecx,%r12d
	movq %rdi,%r14
	movl $2,%edi
	movl $1,%esi
	movq %r14,%rdx
	movl $268436472,%ecx
	call _symbol_lookup
	movq %rax,%rbx
	movq %rbx,-8(%rbp)	 # spill
	cmpq $0,%rbx
	jz L419
L418:
	movl 12(%rbx),%esi
	testl $520,%esi
	jnz L421
L428:
	cmpl $8,%r13d
	jz L421
L424:
	leaq 32(%rbx),%rdi
	movq %r15,%rsi
	movl $3,%edx
	call _type_compat
	cmpl $0,%eax
	jz L423
L421:
	movq (%rbx),%rsi
	pushq %rbx
	pushq %rsi
	pushq $L432
	pushq $1
	call _error
	addq $32,%rsp
L423:
	movl 12(%rbx),%esi
	testl $32,%esi
	jz L435
L436:
	testl $16,%r13d
	jz L435
L433:
	movq (%rbx),%rsi
	pushq %rbx
	pushq %rsi
	pushq $L440
	pushq $1
	call _error
	addq $32,%rsp
L435:
	movl 8(%rbx),%esi
	cmpl %esi,%r12d
	jle L420
L441:
	movq %rbx,%rdi
	movl %r12d,%esi
	call _symbol_move
	jmp L420
L419:
	movq %r14,%rdi
	movl %r13d,%esi
	call _symbol_new
	movq %rax,%rbx
	movq %rbx,-8(%rbp)	 # spill
	leaq 32(%rbx),%rdi
	movq %r15,%rsi
	call _type_copy
	movq %rbx,%rdi
	movl %r12d,%esi
	call _symbol_insert
L420:
	movq -8(%rbp),%rax	 # spill
L417:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L448:
_symbol_redirect:
L449:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L453:
	movq %rdi,%rbx
	movq (%rbx),%rdi
	movl $268435456,%esi
	call _symbol_new
	movq %rbx,64(%rax)
	movl _current_scope(%rip),%esi
	movq %rax,%rdi
	call _symbol_insert
L451:
	popq %rbx
	popq %rbp
	ret
L455:
_symbol_implicit:
L456:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
	pushq %r12
L465:
	movq %rdi,%r12
	leaq -16(%rbp),%rbx
	xorps %xmm0,%xmm0
	movups %xmm0,-16(%rbp)
	movq %rbx,-8(%rbp)
	movq %rbx,%rdi
	movq $4611686018427404288,%rsi
	call _type_append_bits
	movq %rbx,%rdi
	movl $64,%esi
	call _type_append_bits
	movq %r12,%rdi
	movl $32,%esi
	movq %rbx,%rdx
	movl $1,%ecx
	call _symbol_global
	movq %rax,%rbx
	movl 12(%rbx),%esi
	testl $67108864,%esi
	jnz L461
L459:
	leaq 40(%r12),%rsi
	pushq %rsi
	pushq $L462
	pushq $0
	call _error
	addq $24,%rsp
	orl $67108864,12(%rbx)
L461:
	leaq -16(%rbp),%rdi
	call _type_clear
	movq %rbx,%rdi
	call _symbol_redirect
	movq %rbx,%rax
L458:
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L467:
_symbol_anonymous:
L468:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L476:
	movl %edi,%ebx
	xorl %edi,%edi
	movl $16,%esi
	call _symbol_new
	movq %rax,%r12
	cmpl $0,%ebx
	jz L473
L471:
	movl %ebx,64(%r12)
L473:
	movq %r12,%rdi
	movl $1002,%esi
	call _symbol_insert
	movq %r12,%rax
L470:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L478:
_symbol_temp:
L479:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L484:
	movq %rdi,%r12
	xorl %edi,%edi
	movl $256,%esi
	call _symbol_new
	movq %rax,%rbx
	leaq 32(%rbx),%rdi
	movq %r12,%rsi
	call _type_copy
	movq 32(%rbx),%rsi
	andq $-393217,(%rsi)
	movl _current_scope(%rip),%esi
	movq %rbx,%rdi
	call _symbol_insert
	movq %rbx,%rax
L481:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L486:
_symbol_temp_reg:
L487:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L492:
	movq %rdi,%r12
	xorl %edi,%edi
	movl $128,%esi
	call _symbol_new
	movq %rax,%rbx
	leaq 32(%rbx),%rdi
	movq %r12,%rsi
	call _type_append_bits
	movq %rbx,%rdi
	movl $1002,%esi
	call _symbol_insert
	movq %rbx,%rdi
	call _symbol_reg
L489:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L494:
_symbol_reg:
L495:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L509:
	movq %rdi,%rbx
	movl 48(%rbx),%esi
	cmpl $2147483649,%esi
	jnz L500
L498:
	movq _target(%rip),%rsi
	movq 64(%rsi),%rsi
	movq %rbx,%rdi
	call *%rsi
	movl 48(%rbx),%esi
	andl $15,%esi
	movl %esi,%esi
	movq _reg_buckets(,%rsi,8),%rdi
	leaq 96(%rbx),%rsi
	movq %rdi,96(%rbx)
	cmpq $0,%rdi
	jz L506
L504:
	movl 48(%rbx),%edi
	andl $15,%edi
	movl %edi,%edi
	movq _reg_buckets(,%rdi,8),%rdi
	movq %rsi,104(%rdi)
L506:
	movl 48(%rbx),%esi
	andl $15,%esi
	movl %esi,%esi
	movq %rbx,_reg_buckets(,%rsi,8)
	movl 48(%rbx),%esi
	andl $15,%esi
	movl %esi,%esi
	leaq _reg_buckets(,%rsi,8),%rsi
	movq %rsi,104(%rbx)
L500:
	movl 48(%rbx),%eax
L497:
	popq %rbx
	popq %rbp
	ret
L511:
_symbol_storage:
L512:
	pushq %rbp
	movq %rsp,%rbp
L513:
	movl 12(%rdi),%esi
	testl $448,%esi
	jz L514
L518:
	movl 64(%rdi),%esi
	cmpl $0,%esi
	jnz L514
L515:
	movq _target(%rip),%rsi
	movq 72(%rsi),%rsi
	call *%rsi
L514:
	popq %rbp
	ret
L525:
_reg_symbol:
L526:
	pushq %rbp
	movq %rsp,%rbp
L527:
	andl $2149580799,%edi
	movl %edi,%esi
	andl $15,%edi
	movl %edi,%edi
	movq _reg_buckets(,%rdi,8),%rax
L529:
	cmpq $0,%rax
	jz L528
L532:
	movl 48(%rax),%edi
	cmpl %esi,%edi
	jz L528
L530:
	movq 96(%rax),%rax
	jmp L529
L528:
	popq %rbp
	ret
L540:
_label_lookup:
L541:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L549:
	movq %rdi,%rbx
	movl $1001,%edi
	movl $1001,%esi
	movq %rbx,%rdx
	movl $2048,%ecx
	call _symbol_lookup
	movq %rax,%r12
	cmpq $0,%rax
	jnz L546
L544:
	movq %rbx,%rdi
	movl $2048,%esi
	call _symbol_new
	movq %rax,%r12
	movq %rax,%rdi
	movl $1001,%esi
	call _symbol_insert
L546:
	movq %r12,%rax
L543:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L551:
_label_goto:
L552:
	pushq %rbp
	movq %rsp,%rbp
L553:
	call _label_lookup
	movq 64(%rax),%rax
L554:
	popq %rbp
	ret
L559:
_label_define:
L560:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L569:
	movq %rdi,%rbx
	call _label_lookup
	movq %rax,%r12
	movl 12(%r12),%esi
	testl $2147483648,%esi
	jz L565
L563:
	pushq %r12
	pushq %rbx
	pushq $L566
	pushq $1
	call _error
	addq $32,%rsp
L565:
	orl $-2147483648,12(%r12)
	movq %r12,%rdi
	call _symbol_here
	movq 64(%r12),%rax
L562:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L571:
_scope_clear:
L573:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L602:
	movl %edi,%r12d
	xorl %r14d,%r14d
L580:
	movslq %r14d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rbx
L583:
	cmpq $0,%rbx
	jz L581
L584:
	movq 80(%rbx),%r13
	movl 8(%rbx),%esi
	cmpl %r12d,%esi
	jl L581
L589:
	cmpl %r12d,%esi
	jg L585
L593:
	movq %rbx,%rdi
	call _symbol_remove
	movl 12(%rbx),%esi
	testl $7,%esi
	jz L596
L598:
	movq $0,48(%rbx)
	movq %rbx,%rdi
	xorl %esi,%esi
	call _symbol_insert
	jmp L585
L596:
	movq %rbx,%rdi
	call _symbol_free
L585:
	movq %r13,%rbx
	jmp L583
L581:
	addl $1,%r14d
	cmpl $32,%r14d
	jle L580
L575:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L604:
_scope_move:
L606:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L629:
	movl %esi,%r12d
	movl %edi,%ebx
	xorl %r14d,%r14d
L613:
	movslq %r14d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rdi
L616:
	cmpq $0,%rdi
	jz L614
L617:
	movq 80(%rdi),%r13
	movl 8(%rdi),%esi
	cmpl %ebx,%esi
	jl L614
L622:
	cmpl %ebx,%esi
	jg L618
L626:
	movl %r12d,%esi
	call _symbol_move
L618:
	movq %r13,%rdi
	jmp L616
L614:
	addl $1,%r14d
	cmpl $32,%r14d
	jle L613
L608:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L631:
.data
.align 8
_storage_class_text:
	.int 1
	.space 4, 0
	.quad L633
	.int 2
	.space 4, 0
	.quad L634
	.int 4
	.space 4, 0
	.quad L635
	.int 8
	.space 4, 0
	.quad L636
	.int 16
	.space 4, 0
	.quad L637
	.int 32
	.space 4, 0
	.quad L638
	.int 64
	.space 4, 0
	.quad L639
	.int 128
	.space 4, 0
	.quad L640
	.int 256
	.space 4, 0
	.quad L641
	.int 512
	.space 4, 0
	.quad L642
	.int 1024
	.space 4, 0
	.quad L643
	.int 2048
	.space 4, 0
	.quad L644
	.int 268435456
	.space 4, 0
	.quad L645
	.int 536870912
	.space 4, 0
	.quad L646
	.int 1073741824
	.space 4, 0
	.quad L647
	.int -2147483648
	.space 4, 0
	.quad L648
	.int 134217728
	.space 4, 0
	.quad L649
	.int 67108864
	.space 4, 0
	.quad L650
.text
_debug0:
L652:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L697:
	movq %rdi,%rbx
	pushq %rbx
	pushq $L655
	call _output
	addq $16,%rsp
	movl 12(%rbx),%esi
	testl $48,%esi
	jz L658
L656:
	pushq %rbx
	pushq $L659
	call _output
	addq $16,%rsp
L658:
	xorl %r12d,%r12d
L660:
	movslq %r12d,%rsi
	cmpq $18,%rsi
	jae L663
L661:
	movl 12(%rbx),%esi
	movslq %r12d,%rdi
	shlq $4,%rdi
	movl _storage_class_text(%rdi),%eax
	testl %eax,%esi
	jz L662
L664:
	movq _storage_class_text+8(%rdi),%rsi
	pushq %rsi
	call _output
	addq $8,%rsp
L662:
	addl $1,%r12d
	jmp L660
L663:
	movl 12(%rbx),%esi
	testl $7,%esi
	jz L668
L667:
	movl 12(%rbx),%esi
	testl $2147483648,%esi
	jz L672
L670:
	movl 40(%rbx),%esi
	movq 32(%rbx),%rdi
	pushq %rsi
	pushq %rdi
	pushq $L673
	call _output
	addq $24,%rsp
L672:
	movq 48(%rbx),%rbx
L674:
	cmpq $0,%rbx
	jz L654
L675:
	movq %rbx,%rdi
	call _debug0
	movq 72(%rbx),%rbx
	jmp L674
L668:
	movl 48(%rbx),%esi
	cmpl $2147483649,%esi
	jz L680
L678:
	pushq %rsi
	pushq $L681
	call _output
	addq $16,%rsp
L680:
	leaq 32(%rbx),%rsi
	pushq %rsi
	pushq $L682
	call _output
	addq $16,%rsp
	movl 12(%rbx),%esi
	testl $512,%esi
	jz L685
L683:
	movl 64(%rbx),%esi
	pushq %rsi
	pushq $L686
	call _output
	addq $16,%rsp
L685:
	movl 12(%rbx),%esi
	testl $1472,%esi
	jz L689
L690:
	movl 64(%rbx),%esi
	cmpl $0,%esi
	jz L689
L687:
	pushq %rsi
	pushq $L694
	call _output
	addq $16,%rsp
L689:
	pushq $L695
	call _output
	addq $8,%rsp
L654:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L699:
_scope_debug:
L700:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L753:
	movl %edi,%ebx
	pushq %rbx
	pushq $L703
	call _output
	addq $16,%rsp
	cmpl $0,%ebx
	jz L708
L748:
	cmpl $1,%ebx
	jz L711
L749:
	cmpl $2,%ebx
	jz L714
L750:
	cmpl $1001,%ebx
	jz L720
L751:
	cmpl $1002,%ebx
	jz L717
L705:
	pushq %rbx
	pushq $L723
	call _output
	addq $16,%rsp
	jmp L724
L717:
	pushq $L718
	call _output
	addq $8,%rsp
	jmp L724
L720:
	pushq $L721
	call _output
	addq $8,%rsp
	jmp L724
L714:
	pushq $L715
	call _output
	addq $8,%rsp
	jmp L724
L711:
	pushq $L712
	call _output
	addq $8,%rsp
	jmp L724
L708:
	pushq $L709
	call _output
	addq $8,%rsp
L724:
	xorl %r13d,%r13d
L728:
	movslq %r13d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rdi
L731:
	cmpq $0,%rdi
	jz L729
L732:
	movq 80(%rdi),%r12
	movl 8(%rdi),%esi
	cmpl %ebx,%esi
	jl L729
L737:
	cmpl %ebx,%esi
	jg L733
L741:
	movl 12(%rdi),%esi
	testl $1024,%esi
	jnz L733
L743:
	call _debug0
L733:
	movq %r12,%rdi
	jmp L731
L729:
	addl $1,%r13d
	cmpl $32,%r13d
	jle L728
L702:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L755:
_scope_exit:
L756:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L759:
	xorl %r12d,%r12d
L763:
	movslq %r12d,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rdi
L766:
	cmpq $0,%rdi
	jz L764
L767:
	movq 80(%rdi),%rbx
	movl 8(%rdi),%esi
	movl _current_scope(%rip),%eax
	cmpl %eax,%esi
	jl L764
L772:
	cmpl %eax,%esi
	jg L768
L776:
	movl 12(%rdi),%esi
	testl $256,%esi
	jz L780
L778:
	andl $-257,%esi
	movl %esi,12(%rdi)
	orl $128,%esi
	movl %esi,12(%rdi)
L780:
	movl $1002,%esi
	call _symbol_move
L768:
	movq %rbx,%rdi
	jmp L766
L764:
	addl $1,%r12d
	cmpl $32,%r12d
	jle L763
L760:
	addl $-1,_current_scope(%rip)
L758:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L784:
_scope_walk_args:
L785:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L811:
	movq %rdi,%r13
	xorl %ebx,%ebx
L792:
	movslq %ebx,%rsi
	shlq $4,%rsi
	movq _buckets(%rsi),%rdi
L795:
	cmpq $0,%rdi
	jz L793
L796:
	movq 80(%rdi),%r12
	movl 8(%rdi),%esi
	cmpl $1002,%esi
	jl L793
L801:
	cmpl $1002,%esi
	jg L797
L805:
	movl 12(%rdi),%esi
	testl $134217728,%esi
	jz L797
L807:
	call *%r13
L797:
	movq %r12,%rdi
	jmp L795
L793:
	addl $1,%ebx
	cmpl $32,%ebx
	jle L792
L787:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L813:
_scope_end_func:
L814:
	pushq %rbp
	movq %rsp,%rbp
L818:
	movl $1002,%edi
	call _scope_clear
	movl $1001,%edi
	call _scope_clear
L816:
	popq %rbp
	ret
L820:
_scope_exit_proto:
L821:
	pushq %rbp
	movq %rsp,%rbp
L822:
	movl $1002,%edi
	call _scope_clear
	movl _current_scope(%rip),%edi
	movl $1002,%esi
	call _scope_move
	addl $-1,_current_scope(%rip)
L823:
	popq %rbp
	ret
L827:
_scope_resurrect:
L828:
	pushq %rbp
	movq %rsp,%rbp
L829:
	movl _current_scope(%rip),%esi
	movl $1002,%edi
	call _scope_move
L830:
	popq %rbp
	ret
L834:
L655:
	.byte 35,32,37,90,32,0
L566:
	.byte 108,97,98,101,108,32,39,37
	.byte 83,39,32,97,108,114,101,97
	.byte 100,121,32,100,101,102,105,110
	.byte 101,100,32,37,49,0
L440:
	.byte 39,37,83,39,32,112,114,101
	.byte 118,105,111,117,115,108,121,32
	.byte 100,101,99,108,97,114,101,100
	.byte 32,39,101,120,116,101,114,110
	.byte 39,32,37,49,0
L432:
	.byte 105,110,99,111,109,112,97,116
	.byte 105,98,108,101,32,114,101,100
	.byte 101,99,108,97,114,97,116,105
	.byte 111,110,32,111,102,32,39,37
	.byte 83,39,32,37,49,0
L345:
	.byte 108,97,98,101,108,32,39,37
	.byte 83,39,32,110,101,118,101,114
	.byte 32,100,101,102,105,110,101,100
	.byte 32,37,49,0
L83:
	.byte 100,117,112,108,105,99,97,116
	.byte 101,32,109,101,109,98,101,114
	.byte 32,39,37,83,39,32,37,49
	.byte 0
L681:
	.byte 32,37,114,0
L643:
	.byte 77,69,77,66,69,82,0
L640:
	.byte 82,69,71,73,83,84,69,82
	.byte 0
L682:
	.byte 32,37,84,0
L645:
	.byte 82,69,68,73,82,69,67,84
	.byte 0
L642:
	.byte 67,79,78,83,84,0
L633:
	.byte 83,84,82,85,67,84,0
L184:
	.byte 37,65,32,104,97,115,32,122
	.byte 101,114,111,32,115,105,122,101
	.byte 0
L143:
	.byte 37,65,32,101,120,99,101,101
	.byte 100,115,32,109,97,120,105,109
	.byte 117,109,32,111,98,106,101,99
	.byte 116,32,115,105,122,101,0
L650:
	.byte 32,40,105,109,112,108,105,99
	.byte 105,116,41,0
L648:
	.byte 32,40,100,101,102,105,110,101
	.byte 100,41,0
L647:
	.byte 32,40,114,101,102,101,114,101
	.byte 110,99,101,100,41,0
L462:
	.byte 105,109,112,108,105,99,105,116
	.byte 32,100,101,99,108,97,114,97
	.byte 116,105,111,110,32,111,102,32
	.byte 37,115,40,41,0
L87:
	.byte 110,111,32,109,101,109,98,101
	.byte 114,115,32,97,108,108,111,119
	.byte 101,100,32,97,102,116,101,114
	.byte 32,102,108,101,120,105,98,108
	.byte 101,32,97,114,114,97,121,0
L723:
	.byte 108,101,118,101,108,32,37,78
	.byte 10,0
L721:
	.byte 76,65,66,69,76,10,0
L718:
	.byte 82,69,84,73,82,69,68,10
	.byte 0
L715:
	.byte 71,76,79,66,65,76,10,0
L712:
	.byte 76,85,82,75,69,82,10,0
L709:
	.byte 75,69,69,80,10,0
L695:
	.byte 10,0
L673:
	.byte 32,115,105,122,101,32,61,32
	.byte 37,122,44,32,97,108,105,103
	.byte 110,32,61,32,37,100,10,0
L638:
	.byte 69,88,84,69,82,78,0
L639:
	.byte 65,85,84,79,0
L703:
	.byte 10,35,32,115,99,111,112,101
	.byte 32,0
L659:
	.byte 91,37,103,93,32,0
L637:
	.byte 83,84,65,84,73,67,0
L694:
	.byte 32,111,102,102,115,101,116,32
	.byte 61,32,37,100,0
L686:
	.byte 32,118,97,108,117,101,32,61
	.byte 32,37,100,0
L265:
	.byte 115,99,111,112,101,115,32,116
	.byte 111,111,32,100,101,101,112,108
	.byte 121,32,110,101,115,116,101,100
	.byte 0
L636:
	.byte 84,89,80,69,68,69,70,0
L649:
	.byte 32,40,97,114,103,41,0
L646:
	.byte 32,40,116,101,110,116,97,116
	.byte 105,118,101,41,0
L385:
	.byte 46,103,108,111,98,108,32,37
	.byte 103,10,0
L644:
	.byte 76,65,66,69,76,0
L641:
	.byte 76,79,67,65,76,0
L635:
	.byte 69,78,85,77,0
L634:
	.byte 85,78,73,79,78,0
L407:
	.byte 105,108,108,101,103,97,108,32
	.byte 115,116,111,114,97,103,101,32
	.byte 99,108,97,115,115,32,102,111
	.byte 114,32,102,117,110,99,116,105
	.byte 111,110,0
.globl _label_lookup
.globl _member_lookup
.globl _symbol_temp
.globl _symbol_lookup
.globl _scope_enter
.globl _type_clear
.globl _error
.globl _init_bss
.globl _scope_end_func
.globl _scope_walk_args
.globl _symbol_anonymous
.globl _symbol_function_class
.globl _debug_flag_s
.globl _target
.globl _output
.globl _scope_resurrect
.globl _scope_exit
.globl _symbol_implicit
.globl _symbol_redirect
.globl _symbol_init
.globl _type_compat
.globl _label_define
.globl _symbol_here
.globl _symbol_finalize
.globl _current_scope
.globl _block_new
.globl _symbol_new
.globl _error_path
.globl _error_line_no
.globl _scope_exit_proto
.globl _label_goto
.local _reg_buckets
.comm _reg_buckets, 128, 8
.local _buckets
.comm _buckets, 528, 8
.globl _symbol_check_labels
.globl _type_append_bits
.globl _safe_malloc
.globl _member_insert
.globl _symbol_insert
.globl _symbol_storage
.globl _symbol_typename
.globl _symbol_free
.globl _free
.globl _type_sizeof
.globl _type_alignof
.globl _scope_debug
.globl _symbol_reg
.globl _symbol_temp_reg
.globl _type_copy
.globl _reg_symbol
.globl _symbol_global
.globl _last_asm_label
.globl _symbol_complete_strun
