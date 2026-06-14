/* Generated automatically by the program `genemit'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "expr.h"
#include "insn-config.h"

extern char *insn_operand_constraint[][MAX_RECOG_OPERANDS];

extern rtx recog_operand[];
#define operands recog_operand

#define FAIL do { emit_to_sequence--; return 0;} while (0)

#define DONE do { emit_to_sequence--; return gen_sequence ();} while (0)

rtx
gen_tstsi (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, operand0);
}

rtx
gen_tsthi (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, operand0);
}

rtx
gen_tstqi (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, operand0);
}

rtx
gen_tstsf (operand0)
     rtx operand0;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, cc0_rtx, operand0),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, HImode, 0))));
}

rtx
gen_tstdf (operand0)
     rtx operand0;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, cc0_rtx, operand0),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, HImode, 0))));
}

rtx
gen_cmpsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, gen_rtx (MINUS, VOIDmode, operand0, operand1));
}

rtx
gen_cmphi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, gen_rtx (MINUS, VOIDmode, operand0, operand1));
}

rtx
gen_cmpqi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, cc0_rtx, gen_rtx (MINUS, VOIDmode, operand0, operand1));
}

rtx
gen_cmpdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, cc0_rtx, gen_rtx (MINUS, VOIDmode, operand0, operand1)),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 0))));
}

rtx
gen_cmpsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, cc0_rtx, gen_rtx (MINUS, VOIDmode, operand0, operand1)),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 0))));
}

rtx
gen_movsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_movhi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_movqi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_movsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_movdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_movdi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, operand1);
}

rtx
gen_truncsiqi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (TRUNCATE, QImode, operand1));
}

rtx
gen_trunchiqi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (TRUNCATE, QImode, operand1));
}

rtx
gen_truncsihi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (TRUNCATE, HImode, operand1));
}

rtx
gen_zero_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ZERO_EXTEND, SImode, operand1));
}

rtx
gen_zero_extendqihi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ZERO_EXTEND, HImode, operand1));
}

rtx
gen_zero_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ZERO_EXTEND, SImode, operand1));
}

rtx
gen_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (SIGN_EXTEND, SImode, operand1));
}

rtx
gen_extendqihi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (SIGN_EXTEND, HImode, operand1));
}

rtx
gen_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (SIGN_EXTEND, SImode, operand1));
}

rtx
gen_extendsfdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (FLOAT_EXTEND, DFmode, operand1));
}

rtx
gen_truncdfsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (FLOAT_TRUNCATE, SFmode, operand1));
}

rtx
gen_floatsisf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (FLOAT, SFmode, operand1));
}

rtx
gen_floatsidf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (FLOAT, DFmode, operand1));
}

rtx
gen_addsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (PLUS, SImode, operand1, operand2));
}

rtx
gen_addhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (PLUS, HImode, operand1, operand2));
}

rtx
gen_addqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (PLUS, QImode, operand1, operand2));
}

rtx
gen_adddf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (PLUS, DFmode, operand1, operand2));
}

rtx
gen_addsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (PLUS, SFmode, operand1, operand2));
}

rtx
gen_subsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MINUS, SImode, operand1, operand2));
}

rtx
gen_subhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MINUS, HImode, operand1, operand2));
}

rtx
gen_subqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MINUS, QImode, operand1, operand2));
}

rtx
gen_subdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MINUS, DFmode, operand1, operand2));
}

rtx
gen_subsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MINUS, SFmode, operand1, operand2));
}

rtx
gen_mulhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MULT, SImode, operand1, operand2));
}

rtx
gen_mulsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MULT, SImode, operand1, operand2));
}

rtx
gen_umulqihi3 (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (UMULT, HImode, operand0, operand0));
}

rtx
gen_umulhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, operand0, gen_rtx (UMULT, SImode, operand1, operand2)),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, HImode, 1))));
}

rtx
gen_umulsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, operand0, gen_rtx (UMULT, SImode, operand1, operand2)),
		gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 1))));
}

rtx
gen_muldf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MULT, DFmode, operand1, operand2));
}

rtx
gen_mulsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (MULT, SFmode, operand1, operand2));
}

rtx
gen_divdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (DIV, DFmode, operand1, operand2));
}

rtx
gen_divsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (DIV, SFmode, operand1, operand2));
}

rtx
gen_divmodsi4 (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, operand0, gen_rtx (DIV, SImode, operand1, operand2)),
		gen_rtx (SET, VOIDmode, operand3, gen_rtx (MOD, SImode, operand1, operand2))));
}

rtx
gen_udivmodsi4 (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, operand0, gen_rtx (UDIV, SImode, operand1, operand2)),
		gen_rtx (SET, VOIDmode, operand3, gen_rtx (UMOD, SImode, operand1, operand2))));
}

rtx
gen_andsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (AND, SImode, operand1, operand2));
}

rtx
gen_andhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (AND, HImode, operand1, operand2));
}

rtx
gen_andqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (AND, QImode, operand1, operand2));
}

rtx
gen_iorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (IOR, SImode, operand1, operand2));
}

rtx
gen_iorhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (IOR, HImode, operand1, operand2));
}

rtx
gen_iorqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (IOR, QImode, operand1, operand2));
}

rtx
gen_xorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (XOR, SImode, operand1, operand2));
}

rtx
gen_xorhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (XOR, HImode, operand1, operand2));
}

rtx
gen_xorqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (XOR, QImode, operand1, operand2));
}

rtx
gen_negsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NEG, SImode, operand1));
}

rtx
gen_neghi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NEG, HImode, operand1));
}

rtx
gen_negqi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NEG, QImode, operand1));
}

rtx
gen_negsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NEG, SFmode, operand1));
}

rtx
gen_negdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NEG, DFmode, operand1));
}

rtx
gen_abssf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ABS, SFmode, operand1));
}

rtx
gen_absdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ABS, DFmode, operand1));
}

rtx
gen_one_cmplsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NOT, SImode, operand1));
}

rtx
gen_one_cmplhi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NOT, HImode, operand1));
}

rtx
gen_one_cmplqi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (NOT, QImode, operand1));
}

rtx
gen_ashlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFT, SImode, operand1, operand2));
}

rtx
gen_ashlhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFT, HImode, operand1, operand2));
}

rtx
gen_ashlqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFT, QImode, operand1, operand2));
}

rtx
gen_ashrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFTRT, SImode, operand1, operand2));
}

rtx
gen_ashrhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFTRT, HImode, operand1, operand2));
}

rtx
gen_ashrqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ASHIFTRT, QImode, operand1, operand2));
}

rtx
gen_lshlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFT, SImode, operand1, operand2));
}

rtx
gen_lshlhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFT, HImode, operand1, operand2));
}

rtx
gen_lshlqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFT, QImode, operand1, operand2));
}

rtx
gen_lshrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFTRT, SImode, operand1, operand2));
}

rtx
gen_lshrhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFTRT, HImode, operand1, operand2));
}

rtx
gen_lshrqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (LSHIFTRT, QImode, operand1, operand2));
}

rtx
gen_rotlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATE, SImode, operand1, operand2));
}

rtx
gen_rotlhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATE, HImode, operand1, operand2));
}

rtx
gen_rotlqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATE, QImode, operand1, operand2));
}

rtx
gen_rotrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATERT, SImode, operand1, operand2));
}

rtx
gen_rotrhi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATERT, HImode, operand1, operand2));
}

rtx
gen_rotrqi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (ROTATERT, QImode, operand1, operand2));
}

rtx
gen_beq (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (EQ, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bne (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (NE, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bgt (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (GT, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bgtu (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (GTU, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_blt (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (LT, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bltu (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (LTU, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bge (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (GE, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bgeu (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (GEU, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_ble (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (LE, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_bleu (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (IF_THEN_ELSE, VOIDmode, gen_rtx (LEU, VOIDmode, cc0_rtx, const0_rtx), gen_rtx (LABEL_REF, VOIDmode, operand0), pc_rtx));
}

rtx
gen_jump (operand0)
     rtx operand0;
{
  return gen_rtx (SET, VOIDmode, pc_rtx, gen_rtx (LABEL_REF, VOIDmode, operand0));
}

rtx
gen_tablejump (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
		gen_rtx (SET, VOIDmode, pc_rtx, operand0),
		gen_rtx (USE, VOIDmode, gen_rtx (LABEL_REF, VOIDmode, operand1))));
}

rtx
gen_call (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx (CALL, VOIDmode, operand0, operand1);
}

rtx
gen_call_value (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx (SET, VOIDmode, operand0, gen_rtx (CALL, VOIDmode, operand1, operand2));
}

rtx
gen_return ()
{
  return gen_rtx (RETURN, VOIDmode);
}

