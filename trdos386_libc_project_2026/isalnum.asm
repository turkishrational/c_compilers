;nasm

global _isalnum
global _isalpha
global _iscntrl
global _isdigit
global _isgraph
global _islower
global _isprint
global _isspace
global _isupper
global _isxdigit

__is:				; int _is[128] =
 DD 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
 DD 004h, 104h, 104h, 104h, 104h, 104h, 004h, 004h
 DD 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
 DD 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
 DD 140h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h
 DD 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h
 DD 459h, 459h, 459h, 459h, 459h, 459h, 459h, 459h
 DD 459h, 459h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h
 DD 0D0h, 653h, 653h, 653h, 653h, 653h, 653h, 253h
 DD 253h, 253h, 253h, 253h, 253h, 253h, 253h, 253h
 DD 253h, 253h, 253h, 253h, 253h, 253h, 253h, 253h
 DD 253h, 253h, 253h, 0D0h, 0D0h, 0D0h, 0D0h, 0D0h
 DD 0D0h, 473h, 473h, 473h, 473h, 473h, 473h, 073h
 DD 073h, 073h, 073h, 073h, 073h, 073h, 073h, 073h
 DD 073h, 073h, 073h, 073h, 073h, 073h, 073h, 073h
 DD 073h, 073h, 073h, 0D0h, 0D0h, 0D0h, 0D0h, 004h

; isalnum (c) int c; {return (_is[c] & ALNUM);}
; /* 'a'-'z', 'A'-'Z', '0'-'9' */

_isalnum:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; ALNUM
	AND	EAX,1		; (bit 0)
	RETN			; return (_is[c] & ALNUM);

;isalpha (c) int c; {return (_is[c] & ALPHA);}
; /* 'a'-'z', 'A'-'Z' */

_isalpha:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; ALPHA
	AND	EAX,2		; (bit 1)
	RETN			; return (_is[c] & ALPHA);

; iscntrl (c) int c; {return (_is[c] & CNTRL);}
; /* 0-31, 127 */

_iscntrl:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; CNTRL
	AND	EAX,4		; (bit 2)
	RETN			; return (_is[c] & CNTRL);

;isdigit (c) int c; {return (_is[c] & DIGIT );}
; /* '0'-'9' */

_isdigit:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; DIGIT
	AND	EAX,8		; (bit 3)
	RETN			; return (_is[c] & DIGIT);

;isgraph (c) int c; {return (_is[c] & GRAPH);}
; /* '!'-'~' */

_isgraph:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; GRAPH
	AND	EAX,16		; (bit 4)
	RETN			; return (_is[c] & GRAPH);

;islower (c) int c; {return (_is[c] & LOWER);}
; /* 'a'-'z' */

_islower:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX, __is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; LOWER
	AND	EAX,32		; (bit 5)
	RETN			; return (_is[c] & LOWER);

;isprint (c) int c; {return (_is[c] & PRINT);}
; /* ' '-'~' */

_isprint:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX, __is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; PRINT
	AND	EAX,64		; (bit 6)
	RETN			; return (_is[c] & PRINT);

;ispunct (c) int c; {return (_is[c] & PUNCT);}
; /* !alnum && !cntrl && !space */

_ispunct:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX, __is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; PUNCT
	AND	EAX,128		; (bit 7)
	RETN			; return (_is[c] & PUNCT);

;isspace (c) int c; {return (_is[c] & BLANK);}
; /* HT, LF, VT, FF, CR, ' ' */

_isspace:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX, __is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; BLANK
	AND	EAX,256		; (bit 8)
	RETN			; return (_is[c] & BLANK);

;isupper (c) int c; {return (_is[c] & UPPER);}
; /* 'A'-'Z' */

_isupper:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX, __is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; UPPER
	AND	EAX,512		; (bit 9)
	RETN			; return (_is[c] & UPPER);

;isxdigit(c) int c; {return (_is[c] & XDIGIT);}
; /* '0'-'9', 'a'-'f', 'A'-'F' */

_isxdigit:
	MOV	EAX,[ESP+4]	; int c;
	MOV	EDX,__is
	SHL	EAX,1
	SHL	EAX,1
	ADD	EDX,EAX
	MOV	EAX,[EDX]	; XDIGIT
	AND	EAX,1024	; (bit 10)
	RETN			; return (_is[c] & XDIGIT);
