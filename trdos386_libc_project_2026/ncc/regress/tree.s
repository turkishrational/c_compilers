.text
_tree_new:
L2:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L20:
	movl %edi,%ebx
	movl $64,%edi
	call _safe_malloc
	movl %ebx,(%rax)
	leaq 8(%rax),%rsi
	movq $0,8(%rax)
	movq %rsi,16(%rax)
	movl (%rax),%esi
	testl $2147483648,%esi
	jnz L4
L11:
	movl (%rax),%esi
	testl $1073741824,%esi
	jz L4
L15:
	leaq 32(%rax),%rsi
	movq $0,32(%rax)
	movq %rsi,40(%rax)
L4:
	popq %rbx
	popq %rbp
	ret
L22:
_tree_free:
L23:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L48:
	movq %rdi,%rbx
	cmpq $0,%rbx
	jz L25
L26:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L30
L32:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jz L30
L29:
	movq 24(%rbx),%rdi
	call _tree_free
	leaq 32(%rbx),%rdi
	call _forest_clear
	jmp L31
L30:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L31
L43:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jnz L31
L36:
	movq 24(%rbx),%rdi
	call _tree_free
	movq 32(%rbx),%rdi
	call _tree_free
L31:
	leaq 8(%rbx),%rdi
	call _type_clear
	movq %rbx,%rdi
	call _free
L25:
	popq %rbx
	popq %rbp
	ret
L50:
_forest_clear:
L51:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L64:
	movq %rdi,%rbx
L54:
	movq (%rbx),%rdi
	cmpq $0,%rdi
	jz L53
L57:
	movq 48(%rdi),%rsi
	cmpq $0,%rsi
	jz L61
L60:
	movq 56(%rdi),%rax
	movq %rax,56(%rsi)
	jmp L62
L61:
	movq 56(%rdi),%rsi
	movq %rsi,8(%rbx)
L62:
	movq 48(%rdi),%rsi
	movq 56(%rdi),%rax
	movq %rsi,(%rax)
	call _tree_free
	jmp L54
L53:
	popq %rbx
	popq %rbp
	ret
L66:
_tree_commute:
L67:
	pushq %rbp
	movq %rsp,%rbp
L68:
	movq 24(%rdi),%rsi
	movq 32(%rdi),%rax
	movq %rax,24(%rdi)
	movq %rsi,32(%rdi)
L69:
	popq %rbp
	ret
L73:
_tree_v:
L74:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L75:
	movl $-2147483648,%edi
	call _tree_new
	movq %rax,%rbx
	leaq 8(%rbx),%rdi
	movl $1,%esi
	call _type_append_bits
	movq %rbx,%rax
L76:
	popq %rbx
	popq %rbp
	ret
L81:
_tree_i:
L82:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L87:
	movq %rsi,%r12
	movq %rdi,%r13
	movl $-2147483647,%edi
	call _tree_new
	movq %rax,%rbx
	movq %r12,24(%rbx)
	leaq 8(%rbx),%rdi
	movq %r13,%rsi
	call _type_append_bits
	movq %rbx,%rax
L84:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L89:
_tree_f:
L90:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
	movsd %xmm8,-8(%rbp)
L95:
	movsd %xmm0,%xmm8
	movq %rdi,%r12
	movl $-2147483647,%edi
	call _tree_new
	movq %rax,%rbx
	movsd %xmm8,24(%rbx)
	leaq 8(%rbx),%rdi
	movq %r12,%rsi
	call _type_append_bits
	movq %rbx,%rax
L92:
	movsd -8(%rbp),%xmm8
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L97:
_tree_sym:
L98:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L103:
	movq %rdi,%r12
	movl $-2147483646,%edi
	call _tree_new
	movq %rax,%rbx
	movq %r12,32(%rbx)
	leaq 32(%r12),%rsi
	leaq 8(%rbx),%rdi
	call _type_copy
	movq %rbx,%rax
L100:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L105:
_tree_unary:
L106:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L111:
	movq %rsi,%rbx
	call _tree_new
	movq %rbx,24(%rax)
L108:
	popq %rbx
	popq %rbp
	ret
L113:
_tree_binary:
L114:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L119:
	movq %rsi,%rbx
	movq %rdx,%r12
	call _tree_new
	movq %rbx,24(%rax)
	movq %r12,32(%rax)
L116:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L121:
_tree_rvalue:
L122:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L127:
	movq %rdi,%rsi
	movl $1073741832,%edi
	call _tree_unary
	movq %rax,%rbx
	movq 24(%rbx),%rsi
	addq $8,%rsi
	leaq 8(%rbx),%rdi
	call _type_copy
	movq %rbx,%rax
L124:
	popq %rbx
	popq %rbp
	ret
L129:
_tree_chop_unary:
L130:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L144:
	movq %rdi,%rbx
	movq 24(%rbx),%r12
	movq $0,24(%rbx)
	leaq 8(%r12),%rdi
	call _type_clear
	leaq 8(%rbx),%rsi
	movq 8(%rbx),%rdi
	cmpq $0,%rdi
	jz L134
L136:
	movq 16(%r12),%rax
	movq %rdi,(%rax)
	movq 16(%rbx),%rdi
	movq %rdi,16(%r12)
	movq $0,8(%rbx)
	movq %rsi,16(%rbx)
L134:
	movq %rbx,%rdi
	call _tree_free
	movq %r12,%rax
L132:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L146:
_tree_chop_binary:
L147:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L161:
	movq %rdi,%rbx
	movq 24(%rbx),%r12
	movq $0,24(%rbx)
	leaq 8(%r12),%rdi
	call _type_clear
	leaq 8(%rbx),%rsi
	movq 8(%rbx),%rdi
	cmpq $0,%rdi
	jz L151
L153:
	movq 16(%r12),%rax
	movq %rdi,(%rax)
	movq 16(%rbx),%rdi
	movq %rdi,16(%r12)
	movq $0,8(%rbx)
	movq %rsi,16(%rbx)
L151:
	movq %rbx,%rdi
	call _tree_free
	movq %r12,%rax
L149:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L163:
_tree_fetch:
L164:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L186:
	movl %esi,%r13d
	movq %rdi,%rsi
	movl $1073741829,%edi
	call _tree_unary
	movq %rax,%r12
	movq %r12,%rbx
	movq 24(%r12),%rsi
	addq $8,%rsi
	leaq 8(%r12),%rdi
	call _type_deref
	movq 8(%r12),%rsi
	movq (%rsi),%rdi
	movq $-2147483649,%rax
	andq %rax,%rdi
	movq %rdi,(%rsi)
	testl $1,%r13d
	jnz L169
L167:
	movq 24(%r12),%rsi
	movl (%rsi),%edi
	cmpl $2147483649,%edi
	jnz L171
L177:
	cmpq $0,32(%rsi)
	jz L171
L173:
	movq 24(%rsi),%rsi
	cmpq $0,%rsi
	jnz L171
L170:
	movq %r12,%rdi
	call _tree_chop_unary
	movq %rax,%rbx
	movl $-2147483646,(%rax)
	jmp L169
L171:
	movq 24(%r12),%rsi
	movl (%rsi),%esi
	cmpl $1073741830,%esi
	jnz L169
L181:
	movq %r12,%rdi
	call _tree_chop_unary
	movq %rax,%rdi
	call _tree_chop_unary
	movq %rax,%rbx
L169:
	movq %rbx,%rax
L166:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L188:
_tree_addrof:
L189:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L204:
	movq %rdi,%rsi
	movl $1073741830,%edi
	call _tree_unary
	movq %rax,%r12
	movq %r12,%rbx
	movq 24(%r12),%rsi
	addq $8,%rsi
	leaq 8(%r12),%rdi
	call _type_ref
	movq 24(%r12),%rsi
	movl (%rsi),%edi
	cmpl $2147483650,%edi
	jnz L193
L195:
	movq 32(%rsi),%rsi
	movl 12(%rsi),%esi
	testl $48,%esi
	jz L193
L192:
	movq %r12,%rdi
	call _tree_chop_unary
	movq %rax,%rbx
	movl $-2147483647,(%rax)
	leaq 24(%rax),%rsi
	movq 8(%rax),%rdi
	movq (%rdi),%rdi
	andq $131071,%rdi
	call _con_normalize
	jmp L194
L193:
	movq 24(%r12),%rsi
	movl (%rsi),%esi
	cmpl $1073741829,%esi
	jnz L194
L199:
	movq %r12,%rdi
	call _tree_chop_unary
	movq %rax,%rdi
	call _tree_chop_unary
	movq %rax,%rbx
L194:
	movq %rbx,%rax
L191:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L206:
_tree_cast:
L207:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L212:
	movq %rsi,%r12
	movq %rdi,%rsi
	movl $1073741828,%edi
	call _tree_unary
	movq %rax,%rbx
	leaq 8(%rbx),%rdi
	movq %r12,%rsi
	call _type_copy
	movq %rbx,%rax
L209:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L214:
_tree_cast_bits:
L215:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L220:
	movq %rsi,%r12
	movq %rdi,%rsi
	movl $1073741828,%edi
	call _tree_unary
	movq %rax,%rbx
	leaq 8(%rbx),%rdi
	movq %r12,%rsi
	call _type_append_bits
	movq %rbx,%rax
L217:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L222:
.align 8
L242:
	.quad 0x0
_tree_nonzero:
L223:
	pushq %rbp
	movq %rsp,%rbp
L224:
	movl (%rdi),%esi
	cmpl $2147483649,%esi
	jnz L227
L229:
	cmpq $0,32(%rdi)
	jnz L227
L226:
	movq 8(%rdi),%rsi
	movq (%rsi),%rsi
	testq $7168,%rsi
	jz L234
L233:
	movsd 24(%rdi),%xmm0
	ucomisd L242(%rip),%xmm0
	setnz %sil
	movzbl %sil,%eax
	jmp L225
L234:
	movq 24(%rdi),%rsi
	cmpq $0,%rsi
	setnz %sil
	movzbl %sil,%eax
	jmp L225
L227:
	xorl %eax,%eax
L225:
	popq %rbp
	ret
L243:
_tree_zero:
L244:
	pushq %rbp
	movq %rsp,%rbp
L245:
	movl (%rdi),%esi
	cmpl $2147483649,%esi
	jnz L248
L250:
	cmpq $0,32(%rdi)
	jnz L248
L247:
	movq 8(%rdi),%rsi
	movq (%rsi),%rsi
	testq $7168,%rsi
	jz L255
L254:
	movsd 24(%rdi),%xmm0
	ucomisd L242(%rip),%xmm0
	setz %sil
	movzbl %sil,%eax
	jmp L246
L255:
	movq 24(%rdi),%rsi
	cmpq $0,%rsi
	setz %sil
	movzbl %sil,%eax
	jmp L246
L248:
	xorl %eax,%eax
L246:
	popq %rbp
	ret
L264:
_tree_normalize:
L265:
	pushq %rbp
	movq %rsp,%rbp
L266:
	movl (%rdi),%esi
	testl $536870912,%esi
	jz L267
L271:
	movq 24(%rdi),%rsi
	movl (%rsi),%eax
	cmpl $2147483649,%eax
	jnz L267
L275:
	cmpq $0,32(%rsi)
	jnz L267
L268:
	call _tree_commute
L267:
	popq %rbp
	ret
L282:
.align 8
L647:
	.quad 0xbff0000000000000
_simplify0:
L284:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
L645:
	movq %rdi,%rbx
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L288
L290:
	movl (%rbx),%eax
	testl $1073741824,%eax
	jz L288
L599:
	cmpl $1073741832,%eax
	jz L298
L296:
	movq 24(%rbx),%rsi
	movl (%rsi),%edi
	cmpl $2147483649,%edi
	jnz L289
L303:
	cmpq $0,32(%rsi)
	jnz L289
L601:
	cmpl $1073741828,%eax
	jz L330
L602:
	cmpl $1082130439,%eax
	jz L315
L603:
	cmpl $1082130441,%eax
	jnz L289
L326:
	movq 24(%rsi),%rdi
	notq %rdi
	movq %rdi,24(%rsi)
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_unary
	jmp L286
L315:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L319
L318:
	movsd 24(%rsi),%xmm0
	mulsd L647(%rip),%xmm0
	movsd %xmm0,24(%rsi)
	jmp L320
L319:
	movq 24(%rsi),%rdi
	negq %rdi
	movq %rdi,24(%rsi)
L320:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_unary
	jmp L286
L330:
	leaq 24(%rsi),%rcx
	movq 8(%rsi),%rsi
	movq (%rsi),%rdx
	movq 8(%rbx),%rsi
	movq (%rsi),%rdi
	movq %rcx,%rsi
	call _con_cast
	movq %rbx,%rdi
	call _tree_chop_unary
	jmp L286
L298:
	movq %rbx,%rdi
	call _tree_chop_unary
	jmp L286
L288:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L289
L339:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jnz L289
L332:
	movq %rbx,%rdi
	call _tree_normalize
	movq 24(%rbx),%rdi
	call _tree_nonzero
	cmpl $0,%eax
	jz L345
L343:
	movl (%rbx),%esi
	cmpl $39,%esi
	jz L350
L606:
	cmpl $184549409,%esi
	jz L352
L607:
	cmpl $184549413,%esi
	jnz L345
L354:
	movq %rbx,%rdi
	call _tree_free
	movl $64,%edi
	movl $1,%esi
	call _tree_i
	jmp L286
L352:
	movq %rbx,%rdi
	call _tree_commute
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L350:
	movq %rbx,%rdi
	call _tree_commute
	movq %rbx,%rdi
	call _tree_chop_binary
	movq %rax,%rdi
	call _tree_chop_binary
	jmp L286
L345:
	movq 24(%rbx),%rdi
	call _tree_zero
	cmpl $0,%eax
	jz L358
L356:
	movl (%rbx),%esi
	cmpl $39,%esi
	jz L363
L610:
	cmpl $184549409,%esi
	jz L365
L611:
	cmpl $184549413,%esi
	jnz L358
L367:
	movq %rbx,%rdi
	call _tree_commute
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L365:
	movq %rbx,%rdi
	call _tree_free
	movl $64,%edi
	xorl %esi,%esi
	call _tree_i
	jmp L286
L363:
	movq %rbx,%rdi
	call _tree_commute
	movq %rbx,%rdi
	call _tree_chop_binary
	movq %rax,%rbx
	movq %rbx,%rdi
	call _tree_commute
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L358:
	movl (%rbx),%esi
	cmpl $276825113,%esi
	jnz L371
L380:
	movq 24(%rbx),%rsi
	movl (%rsi),%edi
	cmpl $2147483649,%edi
	jnz L371
L376:
	movq 32(%rbx),%rdi
	movl (%rdi),%eax
	cmpl $2147483649,%eax
	jnz L371
L372:
	movq 32(%rdi),%rax
	cmpq 32(%rsi),%rax
	jnz L371
L369:
	movq $0,32(%rdi)
	movq 24(%rbx),%rsi
	movq $0,32(%rsi)
L371:
	movq 32(%rbx),%rsi
	movl (%rsi),%edi
	cmpl $2147483649,%edi
	jnz L289
L387:
	cmpq $0,32(%rsi)
	jnz L289
L384:
	movq 24(%rbx),%rdi
	movl (%rdi),%eax
	cmpl $2147483649,%eax
	jnz L393
L391:
	movl (%rbx),%eax
	cmpl $276825113,%eax
	jz L416
L614:
	cmpl $817890072,%eax
	jnz L393
L402:
	movq 8(%rbx),%rax
	movq (%rax),%rax
	testq $7168,%rax
	jz L406
L405:
	movsd 24(%rdi),%xmm1
	movsd 24(%rsi),%xmm0
	addsd %xmm0,%xmm1
	movsd %xmm1,24(%rdi)
	jmp L407
L406:
	testq $340,%rax
	jz L409
L408:
	movq 24(%rdi),%rax
	movq 24(%rsi),%rsi
	addq %rax,%rsi
	movq %rsi,24(%rdi)
	jmp L407
L409:
	movq 24(%rdi),%rax
	movq 24(%rsi),%rsi
	addq %rax,%rsi
	movq %rsi,24(%rdi)
L407:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L416:
	movq 8(%rbx),%rax
	movq (%rax),%rax
	testq $7168,%rax
	jz L420
L419:
	movsd 24(%rdi),%xmm1
	movsd 24(%rsi),%xmm0
	subsd %xmm0,%xmm1
	movsd %xmm1,24(%rdi)
	jmp L421
L420:
	testq $340,%rax
	jz L423
L422:
	movq 24(%rdi),%rax
	movq 24(%rsi),%rsi
	subq %rsi,%rax
	movq %rax,24(%rdi)
	jmp L421
L423:
	movq 24(%rdi),%rax
	subq 24(%rsi),%rax
	movq %rax,24(%rdi)
L421:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L393:
	movq 24(%rbx),%rsi
	movl (%rsi),%edi
	cmpl $2147483649,%edi
	jnz L289
L429:
	cmpq $0,32(%rsi)
	jnz L289
L426:
	movl (%rbx),%edi
	cmpl $134219294,%edi
	jz L486
	jb L617
L632:
	cmpl $549455908,%edi
	jz L519
	jb L633
L640:
	cmpl $637534243,%edi
	jz L546
	ja L289
L641:
	cmpl $637534242,%edi
	jnz L289
L538:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L542
L541:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	setz %sil
	movzbl %sil,%r12d
	jmp L543
L542:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setz %sil
	movzbl %sil,%r12d
L543:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L546:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L550
L549:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	setnz %sil
	movzbl %sil,%r12d
	jmp L551
L550:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setnz %sil
	movzbl %sil,%r12d
L551:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L633:
	cmpl $549454359,%edi
	jz L472
	jb L634
L637:
	cmpl $549455648,%edi
	jnz L289
L508:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $340,%rdi
	jz L512
L511:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rax
	andq %rax,%rdi
	movq %rdi,24(%rsi)
	jmp L513
L512:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	andq 24(%rax),%rdi
	movq %rdi,24(%rsi)
L513:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L634:
	cmpl $549453845,%edi
	jnz L289
L530:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $340,%rdi
	jz L534
L533:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rax
	xorq %rax,%rdi
	movq %rdi,24(%rsi)
	jmp L535
L534:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	xorq 24(%rax),%rdi
	movq %rdi,24(%rsi)
L535:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L472:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L476
L475:
	movsd 24(%rsi),%xmm1
	movq 32(%rbx),%rdi
	movsd 24(%rdi),%xmm0
	mulsd %xmm0,%xmm1
	movsd %xmm1,24(%rsi)
	jmp L477
L476:
	testq $340,%rdi
	jz L479
L478:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rax
	imulq %rax,%rdi
	movq %rdi,24(%rsi)
	jmp L477
L479:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	imulq 24(%rax),%rdi
	movq %rdi,24(%rsi)
L477:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L519:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $340,%rdi
	jz L523
L522:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rax
	orq %rax,%rdi
	movq %rdi,24(%rsi)
	jmp L524
L523:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	orq 24(%rax),%rdi
	movq %rdi,24(%rsi)
L524:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L617:
	cmpl $33554460,%edi
	jz L565
	jb L618
L625:
	cmpl $33554463,%edi
	jz L587
	jb L626
L629:
	cmpl $134219035,%edi
	jnz L289
L497:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $340,%rdi
	jz L501
L500:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rcx
	sarq %cl,%rdi
	movq %rdi,24(%rsi)
	jmp L502
L501:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rcx
	shrq %cl,%rdi
	movq %rdi,24(%rsi)
L502:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L626:
	cmpl $33554461,%edi
	jnz L289
L576:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L580
L579:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	setb %sil
	movzbl %sil,%r12d
	jmp L581
L580:
	testq $340,%rdi
	jz L583
L582:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setl %sil
	movzbl %sil,%r12d
	jmp L581
L583:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	cmpq 24(%rdi),%rsi
	setb %sil
	movzbl %sil,%r12d
L581:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L587:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L591
L590:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	setbe %sil
	movzbl %sil,%r12d
	jmp L592
L591:
	testq $340,%rdi
	jz L594
L593:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setle %sil
	movzbl %sil,%r12d
	jmp L592
L594:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	cmpq 24(%rdi),%rsi
	setbe %sil
	movzbl %sil,%r12d
L592:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L618:
	cmpl $2342,%edi
	jz L454
	jb L619
L622:
	cmpl $33554458,%edi
	jnz L289
L554:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L558
L557:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	seta %sil
	movzbl %sil,%r12d
	jmp L559
L558:
	testq $340,%rdi
	jz L561
L560:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setg %sil
	movzbl %sil,%r12d
	jmp L559
L561:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	cmpq 24(%rdi),%rsi
	seta %sil
	movzbl %sil,%r12d
L559:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L619:
	cmpl $278,%edi
	jnz L289
L437:
	movq 32(%rbx),%rdi
	call _tree_zero
	cmpl $0,%eax
	jnz L440
L444:
	movq 8(%rbx),%rsi
	movq (%rsi),%rsi
	testq $7168,%rsi
	jz L448
L447:
	movq 24(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	movq 32(%rbx),%rdi
	movsd 24(%rdi),%xmm0
	divsd %xmm0,%xmm1
	movsd %xmm1,24(%rsi)
	jmp L449
L448:
	testq $340,%rsi
	jz L451
L450:
	movq 24(%rbx),%rsi
	movq 24(%rsi),%rax
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cqto
	idivq %rdi
	movq %rax,24(%rsi)
	jmp L449
L451:
	movq 24(%rbx),%rsi
	movq 24(%rsi),%rax
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	xorl %edx,%edx
	divq %rdi
	movq %rax,24(%rsi)
L449:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	movq %rax,%rbx
L440:
	movq %rbx,%rax
	jmp L286
L454:
	movq 32(%rbx),%rdi
	call _tree_zero
	cmpl $0,%eax
	jnz L457
L461:
	movq 8(%rbx),%rsi
	movq (%rsi),%rsi
	testq $340,%rsi
	jz L465
L464:
	movq 24(%rbx),%rsi
	movq 24(%rsi),%rax
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cqto
	idivq %rdi
	movq %rdx,24(%rsi)
	jmp L466
L465:
	movq 24(%rbx),%rsi
	movq 24(%rsi),%rax
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	xorl %edx,%edx
	divq %rdi
	movq %rdx,24(%rsi)
L466:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	movq %rax,%rbx
L457:
	movq %rbx,%rax
	jmp L286
L565:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $7168,%rdi
	jz L569
L568:
	movsd 24(%rsi),%xmm0
	movq 32(%rbx),%rsi
	movsd 24(%rsi),%xmm1
	ucomisd %xmm1,%xmm0
	setae %sil
	movzbl %sil,%r12d
	jmp L570
L569:
	testq $340,%rdi
	jz L572
L571:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	movq 24(%rdi),%rdi
	cmpq %rdi,%rsi
	setge %sil
	movzbl %sil,%r12d
	jmp L570
L572:
	movq 24(%rsi),%rsi
	movq 32(%rbx),%rdi
	cmpq 24(%rdi),%rsi
	setae %sil
	movzbl %sil,%r12d
L570:
	movq %rbx,%rdi
	call _tree_free
	movslq %r12d,%rsi
	movl $64,%edi
	call _tree_i
	jmp L286
L486:
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	testq $340,%rdi
	jz L490
L489:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rcx
	shlq %cl,%rdi
	movq %rdi,24(%rsi)
	jmp L491
L490:
	movq 24(%rsi),%rdi
	movq 32(%rbx),%rax
	movq 24(%rax),%rcx
	shlq %cl,%rdi
	movq %rdi,24(%rsi)
L491:
	movq 24(%rbx),%rsi
	addq $24,%rsi
	movq 8(%rbx),%rdi
	movq (%rdi),%rdi
	call _con_normalize
	movq %rbx,%rdi
	call _tree_chop_binary
	jmp L286
L289:
	movq %rbx,%rax
L286:
	popq %r12
	popq %rbx
	popq %rbp
	ret
L648:
_tree_simplify:
L649:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
L696:
	movq %rdi,%rbx
	leaq -16(%rbp),%rsi
	xorps %xmm0,%xmm0
	movups %xmm0,-16(%rbp)
	movq %rsi,-8(%rbp)
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L656
L658:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jz L656
L655:
	movq 24(%rbx),%rdi
	call _tree_simplify
	movq %rax,24(%rbx)
L662:
	movq 32(%rbx),%rdi
	cmpq $0,%rdi
	jz L674
L665:
	movq 48(%rdi),%rsi
	cmpq $0,%rsi
	jz L669
L668:
	movq 56(%rdi),%rax
	movq %rax,56(%rsi)
	jmp L670
L669:
	movq 56(%rdi),%rsi
	movq %rsi,40(%rbx)
L670:
	movq 48(%rdi),%rsi
	movq 56(%rdi),%rax
	movq %rsi,(%rax)
	call _tree_simplify
	leaq 48(%rax),%rsi
	movq $0,48(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rax,(%rdi)
	movq %rsi,-8(%rbp)
	jmp L662
L674:
	leaq -16(%rbp),%rsi
	movq -16(%rbp),%rdi
	cmpq $0,%rdi
	jz L653
L677:
	movq 40(%rbx),%rax
	movq %rdi,(%rax)
	movq 40(%rbx),%rdi
	movq -16(%rbp),%rax
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,40(%rbx)
	movq $0,-16(%rbp)
	movq %rsi,-8(%rbp)
	jmp L653
L656:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L653
L690:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jnz L653
L683:
	movq 24(%rbx),%rdi
	call _tree_simplify
	movq %rax,24(%rbx)
	movq 32(%rbx),%rdi
	call _tree_simplify
	movq %rax,32(%rbx)
L653:
	movq %rbx,%rdi
	call _simplify0
L651:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L698:
_tree_rewrite_volatile:
L699:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
L758:
	movq %rdi,%rbx
	movl (%rbx),%esi
	cmpl $1073741830,%esi
	jnz L704
L702:
	movq %rbx,%rax
	jmp L701
L704:
	movl (%rbx),%esi
	cmpl $2147483650,%esi
	jnz L714
L709:
	movq 8(%rbx),%rsi
	movq (%rsi),%rsi
	testq $262144,%rsi
	jz L714
L706:
	movq %rbx,%rdi
	call _tree_addrof
	movq %rax,%rdi
	call _tree_simplify
	movq %rax,%rdi
	movl $1,%esi
	call _tree_fetch
	jmp L701
L714:
	leaq -16(%rbp),%rsi
	xorps %xmm0,%xmm0
	movups %xmm0,-16(%rbp)
	movq %rsi,-8(%rbp)
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L718
L720:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jz L718
L717:
	movq 24(%rbx),%rdi
	call _tree_rewrite_volatile
	movq %rax,24(%rbx)
L724:
	movq 32(%rbx),%rdi
	cmpq $0,%rdi
	jz L736
L727:
	movq 48(%rdi),%rsi
	cmpq $0,%rsi
	jz L731
L730:
	movq 56(%rdi),%rax
	movq %rax,56(%rsi)
	jmp L732
L731:
	movq 56(%rdi),%rsi
	movq %rsi,40(%rbx)
L732:
	movq 48(%rdi),%rsi
	movq 56(%rdi),%rax
	movq %rsi,(%rax)
	call _tree_rewrite_volatile
	leaq 48(%rax),%rsi
	movq $0,48(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rax,(%rdi)
	movq %rsi,-8(%rbp)
	jmp L724
L736:
	leaq -16(%rbp),%rsi
	movq -16(%rbp),%rdi
	cmpq $0,%rdi
	jz L715
L739:
	movq 40(%rbx),%rax
	movq %rdi,(%rax)
	movq 40(%rbx),%rdi
	movq -16(%rbp),%rax
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,40(%rbx)
	movq $0,-16(%rbp)
	movq %rsi,-8(%rbp)
	jmp L715
L718:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L715
L752:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jnz L715
L745:
	movq 24(%rbx),%rdi
	call _tree_rewrite_volatile
	movq %rax,24(%rbx)
	movq 32(%rbx),%rdi
	call _tree_rewrite_volatile
	movq %rax,32(%rbx)
L715:
	movq %rbx,%rax
L701:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L760:
_tree_opt:
L761:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
L811:
	movq %rdi,%rbx
	leaq -16(%rbp),%rsi
	xorps %xmm0,%xmm0
	movups %xmm0,-16(%rbp)
	movq %rsi,-8(%rbp)
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L768
L770:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jz L768
L767:
	movq 24(%rbx),%rdi
	call _tree_opt
	movq %rax,24(%rbx)
L774:
	movq 32(%rbx),%rdi
	cmpq $0,%rdi
	jz L786
L777:
	movq 48(%rdi),%rsi
	cmpq $0,%rsi
	jz L781
L780:
	movq 56(%rdi),%rax
	movq %rax,56(%rsi)
	jmp L782
L781:
	movq 56(%rdi),%rsi
	movq %rsi,40(%rbx)
L782:
	movq 48(%rdi),%rsi
	movq 56(%rdi),%rax
	movq %rsi,(%rax)
	call _tree_opt
	leaq 48(%rax),%rsi
	movq $0,48(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rax,(%rdi)
	movq %rsi,-8(%rbp)
	jmp L774
L786:
	leaq -16(%rbp),%rsi
	movq -16(%rbp),%rdi
	cmpq $0,%rdi
	jz L765
L789:
	movq 40(%rbx),%rax
	movq %rdi,(%rax)
	movq 40(%rbx),%rdi
	movq -16(%rbp),%rax
	movq %rdi,56(%rax)
	movq -8(%rbp),%rdi
	movq %rdi,40(%rbx)
	movq $0,-16(%rbp)
	movq %rsi,-8(%rbp)
	jmp L765
L768:
	movl (%rbx),%esi
	testl $2147483648,%esi
	jnz L765
L802:
	movl (%rbx),%esi
	testl $1073741824,%esi
	jnz L765
L795:
	movq 24(%rbx),%rdi
	call _tree_opt
	movq %rax,24(%rbx)
	movq 32(%rbx),%rdi
	call _tree_opt
	movq %rax,32(%rbx)
L765:
	movl (%rbx),%esi
	cmpl $1073741828,%esi
	jnz L808
L806:
	movq %rbx,%rdi
	call _cast_tree_opt
	movq %rax,%rbx
L808:
	movq %rbx,%rdi
	call _field_tree_opt
	movq %rax,%rdi
	call _sign_tree_opt
	movq %rax,%rdi
	call _algebra_tree_opt
L763:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L813:
.data
.align 8
_tree_op_text:
	.quad L815
	.quad L816
	.quad L817
	.quad L818
	.quad L819
	.quad L820
	.quad L821
	.quad L822
	.quad L823
	.quad L824
	.quad L825
	.quad L826
	.quad L827
	.quad L828
	.quad L829
	.quad L830
	.quad L831
	.quad L832
	.quad L833
	.quad L834
	.quad L835
	.quad L836
	.quad L837
	.quad L838
	.quad L839
	.quad L840
	.quad L841
	.quad L842
	.quad L843
	.quad L844
	.quad L845
	.quad L846
	.quad L847
	.quad L848
	.quad L849
	.quad L850
	.quad L851
	.quad L852
	.quad L853
	.quad L854
	.quad L855
	.quad L856
	.quad L857
	.quad L858
.text
_tree_debug:
L859:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L914:
	movl %esi,%ebx
	movq %rdi,%r13
	cmpl $0,%ebx
	jnz L866
L865:
	movq $L863,%rsi
	jmp L867
L866:
	movq $L864,%rsi
L867:
	pushq %rsi
	pushq $L862
	call _output
	addq $16,%rsp
	xorl %r12d,%r12d
L868:
	cmpl %ebx,%r12d
	jge L871
L869:
	pushq $L872
	call _output
	addq $8,%rsp
	addl $1,%r12d
	jmp L868
L871:
	movl (%r13),%edi
	leaq 8(%r13),%rsi
	andl $255,%edi
	movslq %edi,%rdi
	movq _tree_op_text(,%rdi,8),%rdi
	pushq %rsi
	pushq %rdi
	pushq $L873
	call _output
	addq $24,%rsp
	movl (%r13),%esi
	cmpl $-2147483647,%esi
	jz L878
L912:
	cmpl $-2147483646,%esi
	jz L880
L875:
	pushq $L863
	call _output
	addq $8,%rsp
	movl (%r13),%esi
	testl $2147483648,%esi
	jnz L889
L891:
	movl (%r13),%esi
	testl $1073741824,%esi
	jz L889
L888:
	leal 1(%rbx),%esi
	movq 24(%r13),%rdi
	call _tree_debug
	movq 32(%r13),%r12
L895:
	cmpq $0,%r12
	jz L861
L896:
	leal 2(%rbx),%esi
	movq %r12,%rdi
	call _tree_debug
	movq 48(%r12),%r12
	jmp L895
L889:
	movl (%r13),%esi
	testl $2147483648,%esi
	jnz L861
L906:
	movl (%r13),%esi
	testl $1073741824,%esi
	jnz L861
L899:
	addl $1,%ebx
	movq 24(%r13),%rdi
	movl %ebx,%esi
	call _tree_debug
	movq 32(%r13),%rdi
	movl %ebx,%esi
	call _tree_debug
	jmp L861
L878:
	movq 8(%r13),%rsi
	movq (%rsi),%rsi
	subq $8,%rsp
	movq 24(%r13),%rdi
	movq %rdi,(%rsp)
	pushq %rsi
	pushq $L879
	call _output
	addq $24,%rsp
L880:
	movq 32(%r13),%rsi
	cmpq $0,%rsi
	jz L883
L881:
	pushq %rsi
	pushq $L884
	call _output
	addq $16,%rsp
L883:
	pushq $L863
	call _output
	addq $8,%rsp
L861:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L916:
L873:
	.byte 37,115,32,60,37,84,62,32
	.byte 0
L872:
	.byte 32,32,32,32,0
L864:
	.byte 0
L850:
	.byte 78,69,81,0
L849:
	.byte 69,81,0
L846:
	.byte 76,84,69,81,0
L843:
	.byte 71,84,69,81,0
L852:
	.byte 76,79,82,0
L851:
	.byte 79,82,0
L836:
	.byte 88,79,82,0
L856:
	.byte 80,79,83,84,0
L854:
	.byte 81,85,69,83,84,0
L848:
	.byte 76,65,78,68,0
L847:
	.byte 65,78,68,0
L841:
	.byte 71,84,0
L839:
	.byte 65,68,68,0
L819:
	.byte 67,65,83,84,0
L815:
	.byte 78,79,78,69,0
L837:
	.byte 68,73,86,0
L863:
	.byte 10,0
L884:
	.byte 37,90,0
L845:
	.byte 83,72,76,0
L818:
	.byte 67,65,76,76,0
L879:
	.byte 37,67,32,0
L862:
	.byte 37,115,35,32,0
L857:
	.byte 67,79,77,77,65,0
L842:
	.byte 83,72,82,0
L840:
	.byte 83,85,66,0
L853:
	.byte 77,79,68,0
L844:
	.byte 76,84,0
L823:
	.byte 82,86,65,76,85,69,0
L821:
	.byte 65,68,68,82,79,70,0
L858:
	.byte 66,76,75,65,83,71,0
L835:
	.byte 88,79,82,65,83,71,0
L834:
	.byte 79,82,65,83,71,0
L833:
	.byte 65,78,68,65,83,71,0
L832:
	.byte 83,72,82,65,83,71,0
L831:
	.byte 83,72,76,65,83,71,0
L830:
	.byte 83,85,66,65,83,71,0
L829:
	.byte 65,68,68,65,83,71,0
L828:
	.byte 77,79,68,65,83,71,0
L827:
	.byte 68,73,86,65,83,71,0
L826:
	.byte 77,85,76,65,83,71,0
L825:
	.byte 65,83,71,0
L822:
	.byte 78,69,71,0
L820:
	.byte 70,69,84,67,72,0
L838:
	.byte 77,85,76,0
L824:
	.byte 67,79,77,0
L817:
	.byte 83,89,77,0
L855:
	.byte 67,79,76,79,78,0
L816:
	.byte 67,79,78,0
.globl _forest_clear
.globl _type_clear
.globl _tree_cast
.globl _output
.globl _con_cast
.globl _tree_rewrite_volatile
.globl _tree_normalize
.globl _tree_commute
.globl _con_normalize
.globl _tree_v
.globl _tree_nonzero
.globl _tree_zero
.globl _tree_cast_bits
.globl _type_append_bits
.globl _safe_malloc
.globl _tree_opt
.globl _algebra_tree_opt
.globl _sign_tree_opt
.globl _field_tree_opt
.globl _cast_tree_opt
.globl _tree_free
.globl _tree_rvalue
.globl _free
.globl _tree_addrof
.globl _tree_f
.globl _type_deref
.globl _type_ref
.globl _tree_debug
.globl _tree_fetch
.globl _tree_simplify
.globl _tree_chop_binary
.globl _tree_chop_unary
.globl _tree_binary
.globl _tree_unary
.globl _tree_i
.globl _type_copy
.globl _tree_sym
