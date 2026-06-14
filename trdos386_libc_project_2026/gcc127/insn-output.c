/* Generated automatically by the program `genoutput'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "regs.h"
#include "conditions.h"
#include "insn-flags.h"
#include "insn-config.h"

#include "output.h"
#include "aux-output.c"


char *
output_0 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  return output_move_double (operands);
}
}

char *
output_1 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  operands[1] = const0_rtx;
  return AS2 (cmp%L,%1,%0);
}
}

char *
output_2 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  operands[1] = const0_rtx;
  return AS2 (cmp%W,%1,%0);
}
}

char *
output_3 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  operands[1] = const0_rtx;
  return AS2 (cmp%B,%1,%0);
}
}

char *
output_4 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  rtx xops[1];
  if (!FP_REG_P (operands[0]))
    fp_push_sf (operands[0]);
/*  fp_pop_level--; */
  xops[0] = FP_TOP;
  cc_status.flags |= CC_IN_80387;
  output_asm_insn ("ftst\n\tfstp %0(0)\n\tfstsw %Rax\n\tsahf", xops);
  RETCOM (testsf);
}
}

char *
output_5 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  rtx xops[1];
  if (!FP_REG_P (operands[0]))
    fp_push_df (operands[0]);
/*  fp_pop_level--; */
  xops[0] = FP_TOP;
  cc_status.flags |= CC_IN_80387;
  output_asm_insn ("ftst\n\tfstp %0(0)\n\tfstsw %Rax\n\tsahf", xops);
  RETCOM (testdf);
}
}

char *
output_6 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[1])
      || (!REG_P (operands[0]) && GET_CODE (operands[0]) != MEM))
    {
      cc_status.flags |= CC_REVERSED;
      return AS2 (cmp%L,%0,%1);
    }
  return AS2 (cmp%L,%1,%0);
}
}

char *
output_7 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[1])
      || (!REG_P (operands[0]) && GET_CODE (operands[0]) != MEM))
    {
      cc_status.flags |= CC_REVERSED;
      return AS2 (cmp%W,%0,%1);
    }
  return AS2 (cmp%W,%1,%0);
}
}

char *
output_8 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[1])
      || (!REG_P (operands[0]) && GET_CODE (operands[0]) != MEM))
    {
      cc_status.flags |= CC_REVERSED;
      return AS2 (cmp%B,%0,%1);
    }
  return AS2 (cmp%B,%1,%0);
}
}

char *
output_9 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[0]))
    {
      rtx tem = operands[1];
      operands[1] = operands[0];
      operands[0] = tem;
      cc_status.flags |= CC_REVERSED;
    }
  if (! FP_REG_P (operands[1]))
    output_movdf (FP_TOP, operands[1]);
  output_movdf (FP_TOP, operands[0]);
/*  fp_pop_level--;
  fp_pop_level--; */
  cc_status.flags |= CC_IN_80387;
  return "fcompp\n\tfstsw %Rax\n\tsahf";
}
}

char *
output_10 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[0]))
    {
      rtx tem = operands[1];
      operands[1] = operands[0];
      operands[0] = tem;
      cc_status.flags |= CC_REVERSED;
    }
  if (! FP_REG_P (operands[1]))
    output_movsf (FP_TOP, operands[1]);
  output_movsf (FP_TOP, operands[0]);
/*  fp_pop_level--;
  fp_pop_level--; */
  cc_status.flags |= CC_IN_80387;
  return "fcompp\n\tfstsw %Rax\n\tsahf";
}
}

char *
output_11 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (GET_CODE (operands[1]) == CONST_INT || GET_CODE (operands[0]) == MEM)
    return AS2 (test%L,%1,%0);
  return AS2 (test%L,%0,%1);
}
}

char *
output_12 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (GET_CODE (operands[1]) == CONST_INT || GET_CODE (operands[0]) == MEM)
    return AS2 (test%W,%1,%0);
  return AS2 (test%W,%0,%1);
}
}

char *
output_13 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (GET_CODE (operands[1]) == CONST_INT || GET_CODE (operands[0]) == MEM)
    return AS2 (test%B,%1,%0);
  return AS2 (test%B,%0,%1);
}
}

char *
output_15 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (operands[1] == const0_rtx && REG_P (operands[0]))
    return "xor%L %0,%0";
  if (operands[1] == const1_rtx
      && REG_NOTES (insn)
      && GET_MODE (REG_NOTES (insn)) == (enum machine_mode) REG_WAS_0
      /* Make sure the insn that stored the 0 is still present.  */
      && ! XEXP (REG_NOTES (insn), 0)->volatil
      && GET_CODE (XEXP (REG_NOTES (insn), 0)) != NOTE
      /* Make sure cross jumping didn't happen here.  */
      && no_labels_between_p (XEXP (REG_NOTES (insn), 0), insn))
    /* Fastest way to change a 0 to a 1.  */
    return "inc%L %0";
  return "mov%L %1,%0";
}
}

char *
output_17 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (operands[1] == const0_rtx && REG_P (operands[0]))
    return "xor%W %0,%0";
  if (operands[1] == const1_rtx
      && REG_NOTES (insn)
      && GET_MODE (REG_NOTES (insn)) == (enum machine_mode) REG_WAS_0
      /* Make sure the insn that stored the 0 is still present.  */
      && ! XEXP (REG_NOTES (insn), 0)->volatil
      && GET_CODE (XEXP (REG_NOTES (insn), 0)) != NOTE
      /* Make sure cross jumping didn't happen here.  */
      && no_labels_between_p (XEXP (REG_NOTES (insn), 0), insn))
    /* Fastest way to change a 0 to a 1.  */
    return "inc%W %0";
  return "mov%W %1,%0";
}
}

char *
output_18 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (operands[1] == const0_rtx && REG_P (operands[0]))
    return "xor%B %0,%0";
  if (operands[1] == const1_rtx
      && REG_NOTES (insn)
      && GET_MODE (REG_NOTES (insn)) == (enum machine_mode) REG_WAS_0
      /* Make sure the insn that stored the 0 is still present.  */
      && ! XEXP (REG_NOTES (insn), 0)->volatil
      && GET_CODE (XEXP (REG_NOTES (insn), 0)) != NOTE
      /* Make sure cross jumping didn't happen here.  */
      && no_labels_between_p (XEXP (REG_NOTES (insn), 0), insn))
    /* Fastest way to change a 0 to a 1.  */
    return "inc%B %0";
  /* If mov%B isn't allowed for one of these regs, use mov%W.  */
  if (NON_QI_REG_P (operands[0]) || NON_QI_REG_P (operands[1]))
    return (AS2 (mov%W,%w1,%w0));
  return (AS2 (mov%B,%1,%0));
}
}

char *
output_19 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[1]))
    {
      rtx xops[3];
      xops[0] = AT_SP (SFmode);
      xops[1] = gen_rtx (CONST_INT, VOIDmode, 4);
      xops[2] = stack_pointer_rtx;
/*      fp_pop_level--; */
      output_asm_insn (AS2 (sub%L,%1,%2), xops);
      output_asm_insn ("fstp%S %0", xops);
      RET;
    }
  return "push%L %1";
}
}

char *
output_20 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[1])
      && !FP_REG_P (operands[0])
      && !top_dead_p (insn))
    fp_store_sf (operands[0]);
  else
    output_movsf (operands[0], operands[1]);
  RETCOM (movsf);
}
}

char *
output_21 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[1]))
    {
      rtx xops[3];
      xops[0] = AT_SP (DFmode);
      xops[1] = gen_rtx (CONST_INT, VOIDmode, 8);
      xops[2] = stack_pointer_rtx;
/*      fp_pop_level--; */
      output_asm_insn (AS2 (sub%L,%1,%2), xops);
      output_asm_insn ("fstp%Q %0", xops);
      RETCOM (pushdf);
    }
  else
    return output_move_double (operands);
}
}

char *
output_22 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[1])
      && ! FP_REG_P (operands[0])
      && ! top_dead_p (insn))
    fp_store_df (operands[0]);
  else
    output_movdf (operands[0], operands[1]);
  RETCOM (movdf);
}
}

char *
output_23 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
   return output_move_double (operands);
}
}

char *
output_24 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  rtx xops[4];
  xops[0] = operands[0];
  xops[1] = operands[1];
  xops[2] = operands[2];
  xops[3] = gen_rtx (MEM, SImode, stack_pointer_rtx);
  output_asm_insn ("push%z1 %1", xops);
  output_asm_insn (AS2 (add%z3,%2,%3), xops);
  RET;
}
}

char *
output_38 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (FP_REG_P (operands[0]))
    {
      output_movsf (operands[0], operands[1]);
      RET;
    }
  if (FP_REG_P (operands[1]))
    {
      if (top_dead_p (insn))
        fp_pop_df (operands[0]);
      else
        fp_store_df (operands[0]);
      RET;
    }
  output_movsf (FP_TOP, operands[1]);
  fp_pop_df (operands[0]);
  RETCOM (extendsfdf2);
}
}

char *
output_39 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  fp_pop_sf (operands[0]);
  RETCOM (truncdfsf2);
}
}

char *
output_40 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
/*  fp_pop_level++; */

  if (GET_CODE (operands[1]) != MEM)
    {
      rtx xops[2];
      output_asm_insn ("push%L %1", operands);
      operands[1] = AT_SP (SImode);
      output_asm_insn ("fild%L %1", operands);
      xops[0] = stack_pointer_rtx;
      xops[1] = gen_rtx (CONST_INT, VOIDmode, 4);
      output_asm_insn (AS2 (add%L,%1,%0), xops);
    }
  else
    output_asm_insn ("fild%L %1", operands);

  if (! FP_REG_P (operands[0]))
    {
/*      fp_pop_level--; */
      return "fstp%S %0";
    }
  RET;
}
}

char *
output_41 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
/*  fp_pop_level++; */
  if (GET_CODE (operands[1]) != MEM)
    {
      rtx xops[2];
      output_asm_insn ("push%L %1", operands);
      operands[1] = AT_SP (SImode);
      output_asm_insn ("fild%L %1", operands);
      xops[0] = stack_pointer_rtx;
      xops[1] = gen_rtx (CONST_INT, VOIDmode, 4);
      output_asm_insn (AS2 (add%L,%1,%0), xops);
    }
  else
    output_asm_insn ("fild%L %1", operands);
  if (! FP_REG_P (operands[0]))
    {
/*      fp_pop_level--; */
      return "fstp%Q %0";
    }
  RET;
}
}

char *
output_47 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fadd%z0 %0", "fadd%z0 %0", 2)
}

char *
output_48 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fadd%z0 %0", "fadd%z0 %0", 2)
}

char *
output_54 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fsub%z0 %0", "fsubr%z0 %0", 2)
}

char *
output_55 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fsub%z0 %0", "fsubr%z0 %0", 2)
}

char *
output_57 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (rtx_equal_p (operands[0], operands[1])
      && (GET_CODE (operands[2]) == MEM
	  || GET_CODE (operands[2]) == REG))
    /* Assembler has weird restrictions.  */
    return AS2 (imul%L,%2,%0);
  return AS3 (imul%L,%2,%1,%0);
}
}

char *
output_61 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fmul%z0 %0", "fmul%z0 %0", 2)

}

char *
output_62 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fmul%z0 %0", "fmul%z0 %0", 2)

}

char *
output_63 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fdiv%z0 %0", "fdivr%z0 %0", 2)

}

char *
output_64 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL ("fdiv%z0 %0", "fdivr%z0 %0", 2)

}

char *
output_79 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL1 ("fchs")
}

char *
output_80 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL1 ("fchs")
}

char *
output_81 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL1 ("fabs")
}

char *
output_82 (operands, insn)
     rtx *operands;
     rtx insn;
{
FP_CALL1 ("fabs")
}

char *
output_86 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sal%L,%Rcl,%0);
  else
    return AS2 (sal%L,%2,%1);
}
}

char *
output_87 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sal%W,%Rcl,%0);
  else
    return AS2 (sal%W,%2,%1);
}
}

char *
output_88 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sal%B,%Rcl,%0);
  else
    return AS2 (sal%B,%2,%1);
}
}

char *
output_89 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sar%L,%Rcl,%0);
  else
    return AS2 (sar%L,%2,%0);
}
}

char *
output_90 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sar%W,%Rcl,%0);
  else
    return AS2 (sar%W,%2,%0);
}
}

char *
output_91 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (sar%B,%Rcl,%0);
  return
    AS2 (sar%B,%2,%1);
}
}

char *
output_92 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shl%L,%Rcl,%0);
  else
    return AS2 (shl%L,%2,%1);
}
}

char *
output_93 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shl%W,%Rcl,%0);
  else
    return AS2 (shl%W,%2,%1);
}
}

char *
output_94 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shl%B,%Rcl,%0);
  else
    return AS2 (shl%B,%2,%1);
}
}

char *
output_95 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shr%L,%Rcl,%0);
  else
    return AS2 (shr%L,%2,%1);
}
}

char *
output_96 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shr%W,%%cl,%0);
  else
    return AS2 (shr%W,%2,%1);
}
}

char *
output_97 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (shr%B,%%cl,%0);
  else
    return AS2 (shr%B,%2,%1);
}
}

char *
output_98 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (rol%L,%%cl,%0);
  else
    return AS2 (rol%L,%2,%1);
}
}

char *
output_99 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (rol%W,%%cl,%0);
  else
    return AS2 (rol%W,%2,%1);
}
}

char *
output_100 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (rol%B,%%cl,%0);
  else
    return AS2 (rol%B,%2,%1);
}
}

char *
output_101 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (ror%L,%%cl,%0);
  else
    return AS2 (ror%L,%2,%1);
}
}

char *
output_102 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (ror%W,%%cl,%0);
  else
    return AS2 (ror%W,%2,%1);
}
}

char *
output_103 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (REG_P (operands[2]))
    return AS2 (ror%B,%%cl,%0);
  else
    return AS2 (ror%B,%2,%1);
}
}

char *
output_106 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jg %l0", "ja %l0", "jg %l0")
}

char *
output_108 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jl %l0", "jb %l0", "jl %l0")
}

char *
output_110 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jge %l0", "jae %l0", "jge %l0")
}

char *
output_112 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jle %l0", "jbe %l0", "jle %l0") 
}

char *
output_116 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jle %l0", "jbe %l0", "jle %l0") 
}

char *
output_118 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jge %l0", "jae %l0", "jge %l0")

}

char *
output_120 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jl %l0", "jb %l0", "jl %l0")
}

char *
output_122 (operands, insn)
     rtx *operands;
     rtx insn;
{
OUTPUT_JUMP ("jg %l0", "ja %l0", "jg %l0")
}

char *
output_125 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  CC_STATUS_INIT;

  return "jmp %*%0";
}
}

char *
output_126 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (GET_CODE (operands[0]) == MEM
      && ! CONSTANT_ADDRESS_P (XEXP (operands[0], 0)))
    {
      operands[0] = XEXP (operands[0], 0);
      return "call %*%0";
    }
  else
    return "call %0";
}
}

char *
output_127 (operands, insn)
     rtx *operands;
     rtx insn;
{

{
  if (GET_CODE (operands[1]) == MEM
      && ! CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))
    {
      operands[1] = XEXP (operands[1], 0);
      return "call %*%1";
    }
  else
    output_asm_insn ("call %1", operands);

  if (GET_MODE (operands[0]) == DFmode
      || GET_MODE (operands[0]) == SFmode)
    {
/*      fp_pop_level++; */
      /* pop if reg dead */
      if (!FP_REG_P (operands[0]))
	abort ();
      if (top_dead_p (insn))
	{
	  POP_ONE_FP;
	}
    }
  RET;
}
}

char *insn_template[] =
  {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "push%L %1",
    0,
    "push%W %1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "inc%L %0",
    "dec%L %0",
    "dec%L %0",
    "lea%L %a1,%0",
    "mov%B %1,%0",
    "mov%B %1,%0",
    "mov%W %1,%0",
    "movz%W%L %1,%0",
    "movz%B%W %1,%0",
    "movz%B%L %1,%0",
    "movs%W%L %1,%0",
    "movs%B%W %1,%0",
    "movs%B%L %1,%0",
    0,
    0,
    0,
    0,
    "add%L %2,%0",
    "inc%W %0",
    "add%W %2,%0",
    "inc%B %0",
    "add%B %2,%0",
    0,
    0,
    "sub%L %2,%0",
    "dec%W %0",
    "sub%W %2,%0",
    "dec%B %0",
    "sub%B %2,%0",
    0,
    0,
    "imul%W %2,%0",
    0,
    "mul%B %2,%0",
    "mul%W %2,%0",
    "mul%L %2,%0",
    0,
    0,
    0,
    0,
    "cltd\n\tidiv%L %2",
    "xor%L %3,%3\n\tdiv%L %2",
    "and%L %2,%0",
    "and%W %2,%0",
    "and%B %2,%0",
    "or%L %2,%0",
    "or%W %2,%0",
    "or%B %2,%0",
    "xor%L %2,%0",
    "xor%W %2,%0",
    "xor%B %2,%0",
    "neg%L %0",
    "neg%W %0",
    "neg%B %0",
    0,
    0,
    0,
    0,
    "not%L %0",
    "not%W %0",
    "not%B %0",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "je %l0",
    "jne %l0",
    0,
    "ja %l0",
    0,
    "jb %l0",
    0,
    "jae %l0",
    0,
    "jbe %l0",
    "jne %l0",
    "je %l0",
    0,
    "jbe %l0",
    0,
    "jae %l0",
    0,
    "jb %l0",
    0,
    "ja %l0",
    "jmp %l0",
    0,
    0,
    0,
    "ret",
  };

char *(*insn_outfun[])() =
  {
    output_0,
    output_1,
    output_2,
    output_3,
    output_4,
    output_5,
    output_6,
    output_7,
    output_8,
    output_9,
    output_10,
    output_11,
    output_12,
    output_13,
    0,
    output_15,
    0,
    output_17,
    output_18,
    output_19,
    output_20,
    output_21,
    output_22,
    output_23,
    output_24,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_38,
    output_39,
    output_40,
    output_41,
    0,
    0,
    0,
    0,
    0,
    output_47,
    output_48,
    0,
    0,
    0,
    0,
    0,
    output_54,
    output_55,
    0,
    output_57,
    0,
    0,
    0,
    output_61,
    output_62,
    output_63,
    output_64,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_79,
    output_80,
    output_81,
    output_82,
    0,
    0,
    0,
    output_86,
    output_87,
    output_88,
    output_89,
    output_90,
    output_91,
    output_92,
    output_93,
    output_94,
    output_95,
    output_96,
    output_97,
    output_98,
    output_99,
    output_100,
    output_101,
    output_102,
    output_103,
    0,
    0,
    output_106,
    0,
    output_108,
    0,
    output_110,
    0,
    output_112,
    0,
    0,
    0,
    output_116,
    0,
    output_118,
    0,
    output_120,
    0,
    output_122,
    0,
    0,
    output_125,
    output_126,
    output_127,
    0,
  };

rtx (*insn_gen_function[]) () =
  {
    0,
    gen_tstsi,
    gen_tsthi,
    gen_tstqi,
    gen_tstsf,
    gen_tstdf,
    gen_cmpsi,
    gen_cmphi,
    gen_cmpqi,
    gen_cmpdf,
    gen_cmpsf,
    0,
    0,
    0,
    0,
    gen_movsi,
    0,
    gen_movhi,
    gen_movqi,
    0,
    gen_movsf,
    0,
    gen_movdf,
    gen_movdi,
    0,
    0,
    0,
    0,
    0,
    gen_truncsiqi2,
    gen_trunchiqi2,
    gen_truncsihi2,
    gen_zero_extendhisi2,
    gen_zero_extendqihi2,
    gen_zero_extendqisi2,
    gen_extendhisi2,
    gen_extendqihi2,
    gen_extendqisi2,
    gen_extendsfdf2,
    gen_truncdfsf2,
    gen_floatsisf2,
    gen_floatsidf2,
    gen_addsi3,
    0,
    gen_addhi3,
    0,
    gen_addqi3,
    gen_adddf3,
    gen_addsf3,
    gen_subsi3,
    0,
    gen_subhi3,
    0,
    gen_subqi3,
    gen_subdf3,
    gen_subsf3,
    gen_mulhi3,
    gen_mulsi3,
    gen_umulqihi3,
    gen_umulhi3,
    gen_umulsi3,
    gen_muldf3,
    gen_mulsf3,
    gen_divdf3,
    gen_divsf3,
    gen_divmodsi4,
    gen_udivmodsi4,
    gen_andsi3,
    gen_andhi3,
    gen_andqi3,
    gen_iorsi3,
    gen_iorhi3,
    gen_iorqi3,
    gen_xorsi3,
    gen_xorhi3,
    gen_xorqi3,
    gen_negsi2,
    gen_neghi2,
    gen_negqi2,
    gen_negsf2,
    gen_negdf2,
    gen_abssf2,
    gen_absdf2,
    gen_one_cmplsi2,
    gen_one_cmplhi2,
    gen_one_cmplqi2,
    gen_ashlsi3,
    gen_ashlhi3,
    gen_ashlqi3,
    gen_ashrsi3,
    gen_ashrhi3,
    gen_ashrqi3,
    gen_lshlsi3,
    gen_lshlhi3,
    gen_lshlqi3,
    gen_lshrsi3,
    gen_lshrhi3,
    gen_lshrqi3,
    gen_rotlsi3,
    gen_rotlhi3,
    gen_rotlqi3,
    gen_rotrsi3,
    gen_rotrhi3,
    gen_rotrqi3,
    gen_beq,
    gen_bne,
    gen_bgt,
    gen_bgtu,
    gen_blt,
    gen_bltu,
    gen_bge,
    gen_bgeu,
    gen_ble,
    gen_bleu,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_jump,
    gen_tablejump,
    gen_call,
    gen_call_value,
    gen_return,
  };

int insn_n_operands[] =
  {
    2,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    2,
    3,
    2,
    3,
    3,
    3,
    3,
    2,
    3,
    2,
    3,
    3,
    3,
    3,
    3,
    1,
    3,
    3,
    3,
    3,
    3,
    3,
    4,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    2,
    3,
    0,
  };

int insn_n_dups[] =
  {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  };

char *insn_operand_constraint[][MAX_RECOG_OPERANDS] =
  {
    { "=m", "r", },
    { "rm", },
    { "rm", },
    { "rm", },
    { "rm,f", },
    { "rm,f", },
    { "mr,ri", "ri,mr", },
    { "mr,ri", "ri,mr", },
    { "qn,mq", "qm,nq", },
    { "m,f*r,m,f,r,!*r", "m,m,f*r,r,f,*r", },
    { "m,f*r,m,f,r,!*r", "m,m,f*r,r,f,*r", },
    { "rm,ri", "ri,rm", },
    { "rm,ri", "ri,rm", },
    { "qm,qi", "qi,qm", },
    { "=m", "g", },
    { "=g,r", "ri,m", },
    { "=m", "g", },
    { "=g,r", "ri,m", },
    { "=q,*r,m", "*g,q,qi", },
    { "=m,m", "gF,f", },
    { "=mf,rmf,!rm", "m,rf,F", },
    { "=m,m", "gF,f", },
    { "=&rmf,rmf,!rm", "m,fr,F", },
    { "=&r,rm", "m,ri", },
    { "=m", "%r", "ri", },
    { "=g", "0", },
    { "=g", "0", },
    { "=g", "0", },
    { "=r", "p", },
    { "=rm,r", "ri,m", },
    { "=rm,r", "ri,m", },
    { "=rm,r", "ri,m", },
    { "=r", "rm", },
    { "=r", "qm", },
    { "=r", "qm", },
    { "=r", "rm", },
    { "=r", "qm", },
    { "=r", "qm", },
    { "=fm,fm,fm", "m,f,!*r", },
    { "=m,!*r", "f,f", },
    { "=fm,fm", "m,!*r", },
    { "=fm,fm", "m,!*r", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=g", "0", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=qm", "0", },
    { "=m,q", "%0,0", "qn,qmn", },
    { "=f,m,f", "%0,0,0)", "m,!f,!*r", },
    { "=f,m,f", "%0,0,0", "m,!f,!*r", },
    { "=rm,r", "0,0", "ri,rm", },
    { "=g", "0", },
    { "=rm,r", "0,0", "ri,rm", },
    { "=qm", "0", },
    { "=m,q", "0,0", "qn,qmn", },
    { "=f,m,f,f", "0,0,0,m", "m,!f,!*r,*0", },
    { "=f,m,f,f", "0,0,0,m", "m,!f,!*r,*0", },
    { "=r", "%0", "g", },
    { "=r,r", "%0,rm", "g,i", },
    { "rm", },
    { "=a", "%a", "rm", },
    { "=a", "%a", "rm", },
    { "=f,m,f", "%0,0,0", "m,!f,!*r", },
    { "=f,m,f", "%0,0,0", "m,!f,!*r", },
    { "=f,m,f,f", "0,0,0,m", "m,!f,!*r,*0", },
    { "=f,m,f,f", "0,0,0,m", "m,!f,!*r,*0", },
    { "=a", "0", "rm", "=&d", },
    { "=a", "0", "rm", "=&d", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=m,q", "%0,0", "qn,qmn", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=m,q", "%0,0", "qn,qmn", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=rm,r", "%0,0", "ri,rm", },
    { "=qm", "%0", "qn", },
    { "=rm", "0", },
    { "=rm", "0", },
    { "=qm", "0", },
    { "=f,!m", "0,0", },
    { "=f,!m", "0,0", },
    { "=f,!m", "0,0", },
    { "=f,!m", "0,0", },
    { "=rm", "0", },
    { "=rm", "0", },
    { "=qm", "0", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=rm", "0", "cI", },
    { "=qm", "0", "cI", },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { "rm", },
    { "m", "g", },
    { "rf", "m", "g", },
    { 0 },
  };

enum machine_mode insn_operand_mode[][MAX_RECOG_OPERANDS] =
  {
    { DImode, DImode, },
    { SImode, },
    { HImode, },
    { QImode, },
    { SFmode, },
    { DFmode, },
    { SImode, SImode, },
    { HImode, HImode, },
    { QImode, QImode, },
    { DFmode, DFmode, },
    { SFmode, SFmode, },
    { SImode, SImode, },
    { HImode, HImode, },
    { QImode, QImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { HImode, HImode, },
    { HImode, HImode, },
    { QImode, QImode, },
    { SFmode, SFmode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { DFmode, DFmode, },
    { DImode, DImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, QImode, },
    { QImode, SImode, },
    { QImode, HImode, },
    { HImode, SImode, },
    { SImode, HImode, },
    { HImode, QImode, },
    { SImode, QImode, },
    { SImode, HImode, },
    { HImode, QImode, },
    { SImode, QImode, },
    { DFmode, SFmode, },
    { SFmode, DFmode, },
    { SFmode, SImode, },
    { DFmode, SImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, },
    { QImode, QImode, QImode, },
    { DFmode, DFmode, DFmode, },
    { SFmode, SFmode, SFmode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, },
    { QImode, QImode, QImode, },
    { DFmode, DFmode, DFmode, },
    { SFmode, SFmode, SFmode, },
    { HImode, HImode, HImode, },
    { SImode, SImode, SImode, },
    { QImode, },
    { HImode, HImode, HImode, },
    { SImode, SImode, SImode, },
    { DFmode, DFmode, DFmode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { SFmode, SFmode, SFmode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, },
    { HImode, HImode, },
    { QImode, QImode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { SImode, SImode, },
    { HImode, HImode, },
    { QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { SImode, SImode, SImode, },
    { HImode, HImode, HImode, },
    { QImode, QImode, QImode, },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { SImode, },
    { QImode, SImode, },
    { VOIDmode, QImode, SImode, },
    { VOIDmode },
  };

char insn_operand_strict_low[][MAX_RECOG_OPERANDS] =
  {
    { 0, 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0 },
  };

int (*insn_operand_predicate[][MAX_RECOG_OPERANDS])() =
  {
    { push_operand, general_operand, },
    { general_operand, },
    { general_operand, },
    { general_operand, },
    { general_operand, },
    { general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { push_operand, general_operand, },
    { general_operand, general_operand, },
    { push_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { push_operand, general_operand, },
    { general_operand, general_operand, },
    { push_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { push_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { register_operand, address_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { general_operand, general_operand, general_operand, },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { general_operand, },
    { indirect_operand, general_operand, },
    { 0, indirect_operand, general_operand, },
    { 0 },
  };
