.text
_compound_statement:
L7:
	pushq %rbp
	movq %rsp,%rbp
L14:
	movl $16,%edi
	call _lex_match
	call _local_declarations
L10:
	movl _token(%rip),%esi
	cmpl $17,%esi
	jz L12
L11:
	call _statement
	jmp L10
L12:
	movl $17,%edi
	call _lex_match
L9:
	popq %rbp
	ret
L16:
_condition:
L18:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L22:
	movq %rdi,%r13
	movq %rsi,%r12
	movl $262156,%edi
	call _lex_match
	call _expression
	movq %rax,%rdi
	movl $424673333,%esi
	call _test_expression
	movq %rax,%rdi
	movl $1,%esi
	call _gen
	movq %rax,%rbx
	movq %rbx,%rdi
	movq %r13,%rsi
	movq %r12,%rdx
	call _gen_branch
	movq %rbx,%rdi
	call _tree_free
	movl $13,%edi
	call _lex_match
L20:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L24:
_do_statement:
L26:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L27:
	movq _continue_block(%rip),%r12
	movq _break_block(%rip),%r13
	call _block_new
	movq %rax,%rbx
	call _block_new
	movq %rax,_break_block(%rip)
	call _block_new
	movq %rax,_continue_block(%rip)
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	movq %rbx,_current_block(%rip)
	call _lex
	call _statement
	movq _current_block(%rip),%rdi
	movq _continue_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
	movq _continue_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	movl $90,%edi
	call _lex_match
	movq _break_block(%rip),%rsi
	movq %rbx,%rdi
	call _condition
	movl $524311,%edi
	call _lex_match
	movq _break_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	movq %r12,_continue_block(%rip)
	movq %r13,_break_block(%rip)
L28:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L32:
_for_statement:
L34:
	pushq %rbp
	movq %rsp,%rbp
	subq $16,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L35:
	movq _continue_block(%rip),%r14
	movq _break_block(%rip),%r13
	call _block_new
	movq %rax,-8(%rbp)	 # spill
	call _block_new
	movq %rax,-16(%rbp)	 # spill
	xorl %ebx,%ebx
	xorl %r12d,%r12d
	xorl %r15d,%r15d
	call _block_new
	movq %rax,_continue_block(%rip)
	call _block_new
	movq %rax,_break_block(%rip)
	call _lex
	movl $262156,%edi
	call _lex_match
	movl _token(%rip),%esi
	cmpl $524311,%esi
	jz L39
L37:
	call _expression
	movq %rax,%rbx
L39:
	movl $524311,%edi
	call _lex_match
	movl _token(%rip),%esi
	cmpl $524311,%esi
	jz L42
L40:
	call _expression
	movq %rax,%rdi
	movl $424673333,%esi
	call _test_expression
	movq %rax,%r12
L42:
	movl $524311,%edi
	call _lex_match
	movl _token(%rip),%esi
	cmpl $13,%esi
	jz L45
L43:
	call _expression
	movq %rax,%r15
L45:
	movl $13,%edi
	call _lex_match
	cmpq $0,%rbx
	jz L48
L46:
	movq %rbx,%rdi
	movl $3,%esi
	call _gen
L48:
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq -8(%rbp),%rdx	 # spill
	call _block_add_successor
	movq -8(%rbp),%r10	 # spill
	movq %r10,_current_block(%rip)
	cmpq $0,%r12
	jz L50
L49:
	movq %r12,%rdi
	movl $1,%esi
	call _gen
	movq %rax,%rbx
	movq _break_block(%rip),%rdx
	movq %rbx,%rdi
	movq -16(%rbp),%rsi	 # spill
	call _gen_branch
	movq %rbx,%rdi
	call _tree_free
	jmp L51
L50:
	movq -8(%rbp),%rdi	 # spill
	movl $10,%esi
	movq -16(%rbp),%rdx	 # spill
	call _block_add_successor
L51:
	movq -16(%rbp),%r10	 # spill
	movq %r10,_current_block(%rip)
	call _statement
	movq _current_block(%rip),%rdi
	movq _continue_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
	movq _continue_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	cmpq $0,%r15
	jz L54
L52:
	movq %r15,%rdi
	movl $3,%esi
	call _gen
L54:
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq -8(%rbp),%rdx	 # spill
	call _block_add_successor
	movq _break_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	movq %r14,_continue_block(%rip)
	movq %r13,_break_block(%rip)
L36:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L58:
_if_statement:
L60:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L61:
	call _block_new
	movq %rax,%rbx
	call _block_new
	movq %rax,%r14
	movq %r14,%r12
	call _block_new
	movq %rax,%r13
	call _lex
	movq %rbx,%rdi
	movq %r14,%rsi
	call _condition
	movq %rbx,_current_block(%rip)
	call _statement
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %r13,%rdx
	call _block_add_successor
	movl _token(%rip),%esi
	cmpl $68,%esi
	jnz L65
L63:
	call _lex
	movq %r14,_current_block(%rip)
	call _statement
	movq _current_block(%rip),%r12
L65:
	movq %r12,%rdi
	movl $10,%esi
	movq %r13,%rdx
	call _block_add_successor
	movq %r13,_current_block(%rip)
L62:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L69:
_while_statement:
L71:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
L72:
	movq _break_block(%rip),%r14
	movq _continue_block(%rip),%r13
	call _block_new
	movq %rax,%rbx
	call _block_new
	movq %rax,%r12
	call _block_new
	movq %rax,_break_block(%rip)
	movq %rbx,_continue_block(%rip)
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	movq %rbx,_current_block(%rip)
	call _lex
	movq _break_block(%rip),%rsi
	movq %r12,%rdi
	call _condition
	movq %r12,_current_block(%rip)
	call _statement
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	movq _break_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	movq %r13,_continue_block(%rip)
	movq %r14,_break_block(%rip)
L73:
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L77:
_return_statement:
L79:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
	pushq %r12
	pushq %r13
L80:
	xorl %r13d,%r13d
	call _lex
	movl _token(%rip),%esi
	cmpl $524311,%esi
	jz L83
L82:
	call _expression
	movq $_func_ret_type,%rdi
	movq %rax,%rsi
	call _fake_assign
	movq %rax,%r13
	jmp L84
L83:
	movq _func_ret_type(%rip),%rsi
	movq (%rsi),%rsi
	testq $1,%rsi
	jnz L84
L85:
	pushq $L88
	pushq $0
	call _error
	addq $16,%rsp
L84:
	movq _func_ret_type(%rip),%rsi
	movq (%rsi),%rsi
	testq $65536,%rsi
	jz L90
L89:
	movq $_func_ret_type,%rdi
	xorl %esi,%esi
	call _type_sizeof
	movq %rax,%r12
	movq %r13,%rdi
	call _tree_addrof
	movq %rax,%rdi
	movl $1,%esi
	call _gen
	movq %rax,%rbx
	movq %rbx,%r13
	movq _target(%rip),%rsi
	movq 16(%rsi),%rdi
	movq %r12,%rsi
	xorl %edx,%edx
	call _operand_i
	movq %rax,%r12
	movq %rbx,%rdi
	call _operand_leaf
	movq %rax,%rbx
	movq _func_strun_ret(%rip),%rdi
	call _operand_sym
	pushq %r12
	pushq %rbx
	pushq %rax
	pushq $1900019716
	call _insn_new
	addq $32,%rsp
	movq _current_block(%rip),%rsi
	leaq 8(%rsi),%rdi
	movq %rax,%rsi
	call _insn_append
	jmp L91
L90:
	cmpq $0,%r13
	jz L94
L92:
	movq %r13,%rdi
	movl $1,%esi
	call _gen
	movq %rax,%r13
L94:
	cmpq $0,%r13
	jz L96
L95:
	movq %r13,%rdi
	call _operand_leaf
	jmp L97
L96:
	xorl %eax,%eax
L97:
	pushq %rax
	pushq $538968074
	call _insn_new
	addq $16,%rsp
	movq _current_block(%rip),%rsi
	leaq 8(%rsi),%rdi
	movq %rax,%rsi
	call _insn_append
L91:
	movq _current_block(%rip),%rdi
	movq _exit_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
	call _block_new
	movq %rax,_current_block(%rip)
	movl $524311,%edi
	call _lex_match
	movq %r13,%rdi
	call _tree_free
L81:
	popq %r13
	popq %r12
	popq %rbx
	popq %rbp
	ret
L101:
_loop_control:
L103:
	pushq %rbp
	movq %rsp,%rbp
	pushq %rbx
L111:
	movq %rdi,%rbx
	cmpq $0,%rbx
	jnz L108
L106:
	movl _token(%rip),%esi
	pushq %rsi
	pushq $L109
	pushq $1
	call _error
	addq $24,%rsp
L108:
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	call _block_new
	movq %rax,_current_block(%rip)
	call _lex
	movl $524311,%edi
	call _lex_match
L105:
	popq %rbx
	popq %rbp
	ret
L113:
_switch_statement:
L115:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
L116:
	movq _switch_block(%rip),%r10
	movq %r10,-8(%rbp)	 # spill
	movq _default_block(%rip),%r15
	movq _break_block(%rip),%r14
	movl _saw_default(%rip),%r13d
	call _block_new
	call _lex
	movl $262156,%edi
	call _lex_match
	call _expression
	movq %rax,%rbx
	movq %rbx,%r12
	movq 8(%rbx),%rsi
	movq (%rsi),%rsi
	testq $1022,%rsi
	jnz L120
L118:
	pushq $L121
	pushq $1
	call _error
	addq $16,%rsp
L120:
	movq 8(%rbx),%rsi
	movq (%rsi),%rsi
	testq $14,%rsi
	jnz L122
L125:
	testq $48,%rsi
	jz L124
L122:
	movq %rbx,%rdi
	movl $64,%esi
	call _tree_cast_bits
	movq %rax,%r12
L124:
	movl $13,%edi
	call _lex_match
	movq %r12,%rdi
	movl $1,%esi
	call _gen
	movq %rax,%rbx
	movq _current_block(%rip),%rsi
	movq %rsi,_switch_block(%rip)
	movl $0,_saw_default(%rip)
	call _block_new
	movq %rax,_default_block(%rip)
	call _block_new
	movq %rax,_break_block(%rip)
	call _block_new
	movq %rax,%r12
	movq _current_block(%rip),%rdi
	movq _default_block(%rip),%rdx
	movq %rbx,%rsi
	call _block_switch
	movq %rbx,%rdi
	call _tree_free
	movq %r12,_current_block(%rip)
	call _statement
	movq _current_block(%rip),%rdi
	movq _break_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
	movl _saw_default(%rip),%esi
	cmpl $0,%esi
	jnz L131
L129:
	movq _default_block(%rip),%rdi
	movq _break_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
L131:
	movq _switch_block(%rip),%rdi
	call _block_switch_done
	movq _break_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
	movq -8(%rbp),%r10	 # spill
	movq %r10,_switch_block(%rip)
	movq %r14,_break_block(%rip)
	movq %r15,_default_block(%rip)
	movl %r13d,_saw_default(%rip)
L117:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L135:
_in_switch:
L137:
	pushq %rbp
	movq %rsp,%rbp
L145:
	cmpq $0,_switch_block(%rip)
	jnz L139
L140:
	movl _token(%rip),%esi
	pushq %rsi
	pushq $L143
	pushq $1
	call _error
	addq $24,%rsp
L139:
	popq %rbp
	ret
L147:
_case_label:
L149:
	pushq %rbp
	movq %rsp,%rbp
	subq $8,%rsp
	pushq %rbx
L150:
	call _lex
	movq _switch_block(%rip),%rsi
	movq 352(%rsi),%rsi
	movq 8(%rsi),%rdi
	movl $1,%esi
	call _int_expression
	leaq -8(%rbp),%rsi
	movq %rax,-8(%rbp)
	movq _switch_block(%rip),%rdi
	movq 352(%rdi),%rdi
	movq 8(%rdi),%rdi
	call _con_normalize
	movl $486539286,%edi
	call _lex_match
	call _block_new
	movq %rax,%rbx
	movq -8(%rbp),%rsi
	movq _switch_block(%rip),%rdi
	movq %rbx,%rdx
	call _block_switch_case
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	movq %rbx,_current_block(%rip)
L151:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L155:
_default_case:
L157:
	pushq %rbp
	movq %rsp,%rbp
L158:
	movl _saw_default(%rip),%esi
	cmpl $0,%esi
	jz L162
L160:
	pushq $L163
	pushq $1
	call _error
	addq $16,%rsp
L162:
	call _lex
	movl $486539286,%edi
	call _lex_match
	movl $1,_saw_default(%rip)
	movq _current_block(%rip),%rdi
	movq _default_block(%rip),%rdx
	movl $10,%esi
	call _block_add_successor
	movq _default_block(%rip),%rsi
	movq %rsi,_current_block(%rip)
L159:
	popq %rbp
	ret
L167:
_goto_statement:
L169:
	pushq %rbp
	movq %rsp,%rbp
L170:
	call _lex
	movl $262145,%edi
	call _lex_expect
	movq _token+8(%rip),%rdi
	call _label_goto
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rax,%rdx
	call _block_add_successor
	call _block_new
	movq %rax,_current_block(%rip)
	call _lex
	movl $524311,%edi
	call _lex_match
L171:
	popq %rbp
	ret
L175:
_statement:
L176:
	pushq %rbp
	movq %rsp,%rbp
	subq $32,%rsp
	pushq %rbx
L179:
	movl _token(%rip),%esi
	cmpl $73,%esi
	jz L193
	jb L217
L232:
	cmpl $90,%esi
	jz L201
	jb L233
L240:
	cmpl $524311,%esi
	jz L191
	ja L181
L241:
	cmpl $262145,%esi
	jnz L181
L186:
	leaq -32(%rbp),%rdi
	call _lex_peek
	movups -32(%rbp),%xmm0
	movups %xmm0,-16(%rbp)
	movl -16(%rbp),%esi
	cmpl $486539286,%esi
	jnz L181
L187:
	movq _token+8(%rip),%rdi
	call _label_define
	movq %rax,%rbx
	call _lex
	call _lex
	movq _current_block(%rip),%rdi
	movl $10,%esi
	movq %rbx,%rdx
	call _block_add_successor
	movq %rbx,_current_block(%rip)
	jmp L179
L233:
	cmpl $78,%esi
	jz L207
	jb L234
L237:
	cmpl $84,%esi
	jnz L181
L209:
	call _switch_statement
	jmp L178
L234:
	cmpl $74,%esi
	jnz L181
L199:
	call _if_statement
	jmp L178
L207:
	call _return_statement
	jmp L178
L201:
	call _while_statement
	jmp L178
L217:
	cmpl $64,%esi
	jz L205
	jb L218
L225:
	cmpl $66,%esi
	jz L195
	jb L226
L229:
	cmpl $72,%esi
	jnz L181
L197:
	call _for_statement
	jmp L178
L226:
	cmpl $65,%esi
	jnz L181
L211:
	call _in_switch
	call _default_case
	jmp L179
L195:
	call _do_statement
	jmp L178
L218:
	cmpl $60,%esi
	jz L203
	jb L219
L222:
	cmpl $61,%esi
	jnz L181
L213:
	call _in_switch
	call _case_label
	jmp L179
L219:
	cmpl $16,%esi
	jz L184
L181:
	call _expression
	movq %rax,%rdi
	movl $3,%esi
	call _gen
L191:
	movl $524311,%edi
	call _lex_match
	jmp L178
L184:
	call _scope_enter
	call _compound_statement
	call _scope_exit
	jmp L178
L203:
	movq _break_block(%rip),%rdi
	call _loop_control
	jmp L178
L205:
	movq _continue_block(%rip),%rdi
	call _loop_control
	jmp L178
L193:
	call _goto_statement
L178:
	popq %rbx
	movq %rbp,%rsp
	popq %rbp
	ret
L247:
L143:
	.byte 109,105,115,112,108,97,99,101
	.byte 100,32,37,107,32,40,110,111
	.byte 116,32,105,110,32,115,119,105
	.byte 116,99,104,41,0
L109:
	.byte 109,105,115,112,108,97,99,101
	.byte 100,32,37,107,32,115,116,97
	.byte 116,101,109,101,110,116,0
L163:
	.byte 100,117,112,108,105,99,97,116
	.byte 101,32,100,101,102,97,117,108
	.byte 116,32,99,97,115,101,0
L121:
	.byte 115,119,105,116,99,104,32,101
	.byte 120,112,114,101,115,115,105,111
	.byte 110,32,109,117,115,116,32,98
	.byte 101,32,105,110,116,101,103,114
	.byte 97,108,0
L88:
	.byte 114,101,116,117,114,110,32,118
	.byte 97,108,117,101,32,109,105,115
	.byte 115,105,110,103,32,105,110,32
	.byte 110,111,110,45,118,111,105,100
	.byte 32,102,117,110,99,116,105,111
	.byte 110,0
.globl _block_add_successor
.globl _scope_enter
.globl _error
.globl _target
.globl _func_strun_ret
.globl _scope_exit
.globl _insn_append
.globl _lex_expect
.globl _con_normalize
.globl _func_ret_type
.globl _block_switch_done
.globl _label_define
.globl _block_new
.globl _insn_new
.globl _lex
.globl _label_goto
.globl _tree_cast_bits
.globl _local_declarations
.local _saw_default
.comm _saw_default, 4, 4
.globl _compound_statement
.globl _tree_free
.globl _block_switch_case
.globl _tree_addrof
.globl _operand_leaf
.globl _type_sizeof
.globl _block_switch
.globl _gen_branch
.globl _lex_match
.globl _operand_i
.local _default_block
.comm _default_block, 8, 8
.local _continue_block
.comm _continue_block, 8, 8
.local _break_block
.comm _break_block, 8, 8
.local _switch_block
.comm _switch_block, 8, 8
.globl _current_block
.globl _exit_block
.globl _lex_peek
.globl _operand_sym
.globl _gen
.globl _fake_assign
.globl _test_expression
.globl _int_expression
.globl _expression
.globl _token
