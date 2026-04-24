.data
.align 8
_errors:
	.quad L2
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad L3
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad L4
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad 0
	.quad L5
	.quad L6
.text
_strerror:
L7:
	pushq %rbp
	movq %rsp,%rbp
L8:
	cmpl $0,%edi
	jl L10
L17:
	movslq %edi,%rsi
	cmpq $35,%rsi
	jae L10
L13:
	movslq %edi,%rsi
	movq _errors(,%rsi,8),%rax
	cmpq $0,%rax
	jnz L9
L10:
	movq $L21,%rax
L9:
	popq %rbp
	ret
L27:
L21:
	.byte 117,110,107,110,111,119,110,32
	.byte 101,114,114,111,114,0
L4:
	.byte 110,111,116,32,97,32,116,121
	.byte 112,101,119,114,105,116,101,114
	.byte 0
L2:
	.byte 110,111,32,101,114,114,111,114
	.byte 0
L5:
	.byte 109,97,116,104,32,97,114,103
	.byte 117,109,101,110,116,32,111,117
	.byte 116,32,111,102,32,100,111,109
	.byte 97,105,110,32,111,102,32,102
	.byte 117,110,99,0
L6:
	.byte 109,97,116,104,32,114,101,115
	.byte 117,108,116,32,110,111,116,32
	.byte 114,101,112,114,101,115,101,110
	.byte 116,97,98,108,101,0
L3:
	.byte 111,117,116,32,111,102,32,109
	.byte 101,109,111,114,121,0
.globl _strerror
