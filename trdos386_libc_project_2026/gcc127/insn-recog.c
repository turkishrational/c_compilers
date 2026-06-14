/* Generated automatically by the program `genrecog'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"

/* `recog' contains a decision tree
   that recognizes whether the rtx X0 is a valid instruction.

   recog returns -1 if the rtx is not valid.
   If the rtx is valid, recog returns a nonnegative number
   which is the insn code number for the pattern that matched.
   This is the same as the order in the machine description of
   the entry that matched.  This number can be used as an index into
   insn_templates and insn_n_operands (found in insn-output.c)
   or as an argument to output_insn_hairy (also in insn-output.c).  */

rtx recog_operand[MAX_RECOG_OPERANDS];

rtx *recog_operand_loc[MAX_RECOG_OPERANDS];

rtx *recog_dup_loc[MAX_DUP_OPERANDS];

char recog_dup_num[MAX_DUP_OPERANDS];

extern rtx recog_addr_dummy;

#define operands recog_operand

int
recog_1 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L496:
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
 switch (GET_CODE (x2))
  {
  case EQ:
  if (1)
    goto L497;
  break;
  case NE:
  if (1)
    goto L506;
  break;
  case GT:
  if (1)
    goto L515;
  break;
  case GTU:
  if (1)
    goto L524;
  break;
  case LT:
  if (1)
    goto L533;
  break;
  case LTU:
  if (1)
    goto L542;
  break;
  case GE:
  if (1)
    goto L551;
  break;
  case GEU:
  if (1)
    goto L560;
  break;
  case LE:
  if (1)
    goto L569;
  break;
  case LEU:
  if (1)
    goto L578;
  break;
  }
  goto ret0;
 L497:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L498;
  goto ret0;
 L498:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L499;
  goto ret0;
 L499:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L500;
  if (x2 == pc_rtx && 1)
    goto L590;
  goto ret0;
 L500:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L501; }
  goto ret0;
 L501:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 104;
  goto ret0;
 L590:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L591;
  goto ret0;
 L591:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 114; }
  goto ret0;
 L506:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L507;
  goto ret0;
 L507:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L508;
  goto ret0;
 L508:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L509;
  if (x2 == pc_rtx && 1)
    goto L599;
  goto ret0;
 L509:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L510; }
  goto ret0;
 L510:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 105;
  goto ret0;
 L599:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L600;
  goto ret0;
 L600:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 115; }
  goto ret0;
 L515:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L516;
  goto ret0;
 L516:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L517;
  goto ret0;
 L517:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L518;
  if (x2 == pc_rtx && 1)
    goto L608;
  goto ret0;
 L518:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L519; }
  goto ret0;
 L519:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 106;
  goto ret0;
 L608:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L609;
  goto ret0;
 L609:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 116; }
  goto ret0;
 L524:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L525;
  goto ret0;
 L525:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L526;
  goto ret0;
 L526:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L527;
  if (x2 == pc_rtx && 1)
    goto L617;
  goto ret0;
 L527:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L528; }
  goto ret0;
 L528:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 107;
  goto ret0;
 L617:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L618;
  goto ret0;
 L618:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 117; }
  goto ret0;
 L533:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L534;
  goto ret0;
 L534:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L535;
  goto ret0;
 L535:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L536;
  if (x2 == pc_rtx && 1)
    goto L626;
  goto ret0;
 L536:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L537; }
  goto ret0;
 L537:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 108;
  goto ret0;
 L626:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L627;
  goto ret0;
 L627:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 118; }
  goto ret0;
 L542:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L543;
  goto ret0;
 L543:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L544;
  goto ret0;
 L544:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L545;
  if (x2 == pc_rtx && 1)
    goto L635;
  goto ret0;
 L545:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L546; }
  goto ret0;
 L546:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 109;
  goto ret0;
 L635:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L636;
  goto ret0;
 L636:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 119; }
  goto ret0;
 L551:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L552;
  goto ret0;
 L552:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L553;
  goto ret0;
 L553:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L554;
  if (x2 == pc_rtx && 1)
    goto L644;
  goto ret0;
 L554:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L555; }
  goto ret0;
 L555:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 110;
  goto ret0;
 L644:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L645;
  goto ret0;
 L645:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 120; }
  goto ret0;
 L560:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L561;
  goto ret0;
 L561:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L562;
  goto ret0;
 L562:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L563;
  if (x2 == pc_rtx && 1)
    goto L653;
  goto ret0;
 L563:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L564; }
  goto ret0;
 L564:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 111;
  goto ret0;
 L653:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L654;
  goto ret0;
 L654:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 121; }
  goto ret0;
 L569:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L570;
  goto ret0;
 L570:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L571;
  goto ret0;
 L571:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L572;
  if (x2 == pc_rtx && 1)
    goto L662;
  goto ret0;
 L572:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L573; }
  goto ret0;
 L573:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 112;
  goto ret0;
 L662:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L663;
  goto ret0;
 L663:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 122; }
  goto ret0;
 L578:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L579;
  goto ret0;
 L579:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L580;
  goto ret0;
 L580:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L581;
  if (x2 == pc_rtx && 1)
    goto L671;
  goto ret0;
 L581:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L582; }
  goto ret0;
 L582:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 113;
  goto ret0;
 L671:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L672;
  goto ret0;
 L672:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 123; }
  goto ret0;
 ret0: return -1;
}

int
recog_2 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L133:
  x1 = XEXP (x0, 1);
 switch (GET_CODE (x1))
  {
  case TRUNCATE:
  if (GET_MODE (x1) == HImode && 1)
    goto L134;
  break;
  case ZERO_EXTEND:
  if (GET_MODE (x1) == HImode && 1)
    goto L142;
  break;
  case SIGN_EXTEND:
  if (GET_MODE (x1) == HImode && 1)
    goto L154;
  break;
  case PLUS:
  if (GET_MODE (x1) == HImode && 1)
    goto L183;
  break;
  case MINUS:
  if (GET_MODE (x1) == HImode && 1)
    goto L218;
  break;
  case MULT:
  if (GET_MODE (x1) == SImode && 1)
    goto L248;
  break;
  case UMULT:
  if (GET_MODE (x1) == HImode && 1)
    goto L258;
  break;
  case AND:
  if (GET_MODE (x1) == HImode && 1)
    goto L326;
  break;
  case IOR:
  if (GET_MODE (x1) == HImode && 1)
    goto L341;
  break;
  case XOR:
  if (GET_MODE (x1) == HImode && 1)
    goto L356;
  break;
  case NEG:
  if (GET_MODE (x1) == HImode && 1)
    goto L370;
  break;
  case NOT:
  if (GET_MODE (x1) == HImode && 1)
    goto L398;
  break;
  case ASHIFT:
  if (GET_MODE (x1) == HImode && 1)
    goto L411;
  break;
  case ASHIFTRT:
  if (GET_MODE (x1) == HImode && 1)
    goto L426;
  break;
  case LSHIFT:
  if (GET_MODE (x1) == HImode && 1)
    goto L441;
  break;
  case LSHIFTRT:
  if (GET_MODE (x1) == HImode && 1)
    goto L456;
  break;
  case ROTATE:
  if (GET_MODE (x1) == HImode && 1)
    goto L471;
  break;
  case ROTATERT:
  if (GET_MODE (x1) == HImode && 1)
    goto L486;
  break;
  }
 L81:
  if (general_operand (x1, HImode))
    { recog_operand[1] = x1; return 17; }
  goto ret0;
 L134:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 31; }
  goto L81;
 L142:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 33; }
  goto L81;
 L154:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 36; }
  goto L81;
 L183:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L184; }
  goto L81;
 L184:
  x2 = XEXP (x1, 1);
  if (x2 == const1_rtx && 1)
    return 43;
 L189:
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 44; }
  goto L81;
 L218:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L219; }
  goto L81;
 L219:
  x2 = XEXP (x1, 1);
  if (x2 == const1_rtx && 1)
    return 50;
 L224:
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 51; }
  goto L81;
 L248:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L249; }
  goto L81;
 L249:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 56; }
  goto L81;
 L258:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[0] = x2; goto L259; }
  goto L81;
 L259:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[0] = x2; return 58; }
  goto L81;
 L326:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L327; }
  goto L81;
 L327:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 68; }
  goto L81;
 L341:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L342; }
  goto L81;
 L342:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 71; }
  goto L81;
 L356:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L357; }
  goto L81;
 L357:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 74; }
  goto L81;
 L370:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 77; }
  goto L81;
 L398:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 84; }
  goto L81;
 L411:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L412; }
  goto L81;
 L412:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 87; }
  goto L81;
 L426:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L427; }
  goto L81;
 L427:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 90; }
  goto L81;
 L441:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L442; }
  goto L81;
 L442:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 93; }
  goto L81;
 L456:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L457; }
  goto L81;
 L457:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 96; }
  goto L81;
 L471:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L472; }
  goto L81;
 L472:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 99; }
  goto L81;
 L486:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L487; }
  goto L81;
 L487:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 102; }
  goto L81;
 ret0: return -1;
}

int
recog_3 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L4:
  x1 = XEXP (x0, 0);
  if (x1 == cc0_rtx && 1)
    goto L26;
  if (x1 == pc_rtx && 1)
    goto L495;
 L688:
  if (1)
    { recog_operand[0] = x1; goto L689; }
 L83:
 switch (GET_MODE (x1))
  {
  case QImode:
  if (general_operand (x1, QImode))
    { recog_operand[0] = x1; goto L125; }
  break;
 L77:
  case HImode:
  if (push_operand (x1, HImode))
    { recog_operand[0] = x1; goto L78; }
 L80:
  if (general_operand (x1, HImode))
    { recog_operand[0] = x1; goto L133; }
  break;
  case SImode:
  if (push_operand (x1, SImode))
    { recog_operand[0] = x1; goto L72; }
 L74:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; goto L75; }
 L101:
  if (push_operand (x1, SImode))
    { recog_operand[0] = x1; goto L102; }
 L106:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; goto L107; }
 L121:
  if (register_operand (x1, SImode))
    { recog_operand[0] = x1; goto L122; }
 L136:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; goto L137; }
  break;
  case DImode:
  if (push_operand (x1, DImode))
    { recog_operand[0] = x1; goto L2; }
 L98:
  if (general_operand (x1, DImode))
    { recog_operand[0] = x1; goto L99; }
  break;
  case SFmode:
  if (push_operand (x1, SFmode))
    { recog_operand[0] = x1; goto L87; }
 L89:
  if (general_operand (x1, SFmode))
    { recog_operand[0] = x1; goto L165; }
  break;
  case DFmode:
  if (push_operand (x1, DFmode))
    { recog_operand[0] = x1; goto L93; }
 L95:
  if (general_operand (x1, DFmode))
    { recog_operand[0] = x1; goto L161; }
  break;
  }
  goto ret0;
 L26:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == MINUS && 1)
    goto L27;
  if (GET_CODE (x1) == AND && GET_MODE (x1) == SImode && 1)
    goto L58;
  if (GET_CODE (x1) == AND && GET_MODE (x1) == HImode && 1)
    goto L63;
  if (GET_CODE (x1) == AND && GET_MODE (x1) == QImode && 1)
    goto L68;
 L5:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; return 1; }
 L8:
  if (general_operand (x1, HImode))
    { recog_operand[0] = x1; return 2; }
 L11:
  if (general_operand (x1, QImode))
    { recog_operand[0] = x1; return 3; }
  x1 = XEXP (x0, 0);
  goto L688;
 L27:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L28; }
 L32:
  if (general_operand (x2, HImode))
    { recog_operand[0] = x2; goto L33; }
 L37:
  if (general_operand (x2, QImode))
    { recog_operand[0] = x2; goto L38; }
  goto L5;
 L28:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 6; }
  x2 = XEXP (x1, 0);
  goto L32;
 L33:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 7; }
  x2 = XEXP (x1, 0);
  goto L37;
 L38:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 8; }
  goto L5;
 L58:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L59; }
  goto L5;
 L59:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 11; }
  goto L5;
 L63:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[0] = x2; goto L64; }
  goto L5;
 L64:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 12; }
  goto L5;
 L68:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[0] = x2; goto L69; }
  goto L5;
 L69:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 13; }
  goto L5;
 L495:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L496;
  if (GET_CODE (x1) == LABEL_REF && 1)
    goto L676;
  x1 = XEXP (x0, 0);
  goto L688;
 L496:
  tem = recog_1 (x0, insn);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L688;
 L676:
  x2 = XEXP (x1, 0);
  if (1)
    { recog_operand[0] = x2; return 124; }
  x1 = XEXP (x0, 0);
  goto L688;
 L689:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == CALL && 1)
    goto L690;
  x1 = XEXP (x0, 0);
  goto L83;
 L690:
  x2 = XEXP (x1, 0);
  if (indirect_operand (x2, QImode))
    { recog_operand[1] = x2; goto L691; }
  x1 = XEXP (x0, 0);
  goto L83;
 L691:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 127; }
  x1 = XEXP (x0, 0);
  goto L83;
 L125:
  x1 = XEXP (x0, 1);
 switch (GET_CODE (x1))
  {
  case TRUNCATE:
  if (GET_MODE (x1) == QImode && 1)
    goto L126;
  break;
  case PLUS:
  if (GET_MODE (x1) == QImode && 1)
    goto L193;
  break;
  case MINUS:
  if (GET_MODE (x1) == QImode && 1)
    goto L228;
  break;
  case AND:
  if (GET_MODE (x1) == QImode && 1)
    goto L331;
  break;
  case IOR:
  if (GET_MODE (x1) == QImode && 1)
    goto L346;
  break;
  case XOR:
  if (GET_MODE (x1) == QImode && 1)
    goto L361;
  break;
  case NEG:
  if (GET_MODE (x1) == QImode && 1)
    goto L374;
  break;
  case NOT:
  if (GET_MODE (x1) == QImode && 1)
    goto L402;
  break;
  case ASHIFT:
  if (GET_MODE (x1) == QImode && 1)
    goto L416;
  break;
  case ASHIFTRT:
  if (GET_MODE (x1) == QImode && 1)
    goto L431;
  break;
  case LSHIFT:
  if (GET_MODE (x1) == QImode && 1)
    goto L446;
  break;
  case LSHIFTRT:
  if (GET_MODE (x1) == QImode && 1)
    goto L461;
  break;
  case ROTATE:
  if (GET_MODE (x1) == QImode && 1)
    goto L476;
  break;
  case ROTATERT:
  if (GET_MODE (x1) == QImode && 1)
    goto L491;
  break;
  }
 L84:
  if (general_operand (x1, QImode))
    { recog_operand[1] = x1; return 18; }
  x1 = XEXP (x0, 0);
  goto L77;
 L126:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 29; }
 L130:
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 30; }
  goto L84;
 L193:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L194; }
  goto L84;
 L194:
  x2 = XEXP (x1, 1);
  if (x2 == const1_rtx && 1)
    return 45;
 L199:
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 46; }
  goto L84;
 L228:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L229; }
  goto L84;
 L229:
  x2 = XEXP (x1, 1);
  if (x2 == const1_rtx && 1)
    return 52;
 L234:
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 53; }
  goto L84;
 L331:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L332; }
  goto L84;
 L332:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 69; }
  goto L84;
 L346:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L347; }
  goto L84;
 L347:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 72; }
  goto L84;
 L361:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L362; }
  goto L84;
 L362:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 75; }
  goto L84;
 L374:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 78; }
  goto L84;
 L402:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 85; }
  goto L84;
 L416:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L417; }
  goto L84;
 L417:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 88; }
  goto L84;
 L431:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L432; }
  goto L84;
 L432:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 91; }
  goto L84;
 L446:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L447; }
  goto L84;
 L447:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 94; }
  goto L84;
 L461:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L462; }
  goto L84;
 L462:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 97; }
  goto L84;
 L476:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L477; }
  goto L84;
 L477:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 100; }
  goto L84;
 L491:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L492; }
  goto L84;
 L492:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 103; }
  goto L84;
 L78:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    { recog_operand[1] = x1; return 16; }
  x1 = XEXP (x0, 0);
  goto L80;
 L133:
  return recog_2 (x0, insn);
 L72:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    { recog_operand[1] = x1; return 14; }
  x1 = XEXP (x0, 0);
  goto L74;
 L75:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    { recog_operand[1] = x1; return 15; }
  x1 = XEXP (x0, 0);
  goto L101;
 L102:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == PLUS && GET_MODE (x1) == SImode && 1)
    goto L103;
  x1 = XEXP (x0, 0);
  goto L106;
 L103:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L104; }
  x1 = XEXP (x0, 0);
  goto L106;
 L104:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 24; }
  x1 = XEXP (x0, 0);
  goto L106;
 L107:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L121;
    }
  if (GET_CODE (x1) == PLUS && 1)
    goto L108;
  if (GET_CODE (x1) == MINUS && 1)
    goto L118;
  x1 = XEXP (x0, 0);
  goto L121;
 L108:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L109; }
  x1 = XEXP (x0, 0);
  goto L121;
 L109:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) != CONST_INT)
    {  x1 = XEXP (x0, 0);
    goto L121; }
  if (x2 == const1_rtx && 1)
    return 25;
 L114:
  if (XINT (x2, 0) == -1 && 1)
    return 26;
  x1 = XEXP (x0, 0);
  goto L121;
 L118:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L119; }
  x1 = XEXP (x0, 0);
  goto L121;
 L119:
  x2 = XEXP (x1, 1);
  if (x2 == const1_rtx && 1)
    return 27;
  x1 = XEXP (x0, 0);
  goto L121;
 L122:
  x1 = XEXP (x0, 1);
  if (address_operand (x1, QImode))
    { recog_operand[1] = x1; return 28; }
  x1 = XEXP (x0, 0);
  goto L136;
 L137:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case ZERO_EXTEND:
  if (1)
    goto L138;
  break;
  case SIGN_EXTEND:
  if (1)
    goto L150;
  break;
  case PLUS:
  if (1)
    goto L178;
  break;
  case MINUS:
  if (1)
    goto L213;
  break;
  case MULT:
  if (1)
    goto L253;
  break;
  case AND:
  if (1)
    goto L321;
  break;
  case IOR:
  if (1)
    goto L336;
  break;
  case XOR:
  if (1)
    goto L351;
  break;
  case NEG:
  if (1)
    goto L366;
  break;
  case NOT:
  if (1)
    goto L394;
  break;
  case ASHIFT:
  if (1)
    goto L406;
  break;
  case ASHIFTRT:
  if (1)
    goto L421;
  break;
  case LSHIFT:
  if (1)
    goto L436;
  break;
  case LSHIFTRT:
  if (1)
    goto L451;
  break;
  case ROTATE:
  if (1)
    goto L466;
  break;
  case ROTATERT:
  if (1)
    goto L481;
  break;
  }
  goto ret0;
 L138:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 32; }
 L146:
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 34; }
  goto ret0;
 L150:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 35; }
 L158:
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 37; }
  goto ret0;
 L178:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L179; }
  goto ret0;
 L179:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 42; }
  goto ret0;
 L213:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L214; }
  goto ret0;
 L214:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 49; }
  goto ret0;
 L253:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L254; }
  goto ret0;
 L254:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 57; }
  goto ret0;
 L321:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L322; }
  goto ret0;
 L322:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 67; }
  goto ret0;
 L336:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L337; }
  goto ret0;
 L337:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 70; }
  goto ret0;
 L351:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L352; }
  goto ret0;
 L352:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 73; }
  goto ret0;
 L366:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 76; }
  goto ret0;
 L394:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 83; }
  goto ret0;
 L406:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L407; }
  goto ret0;
 L407:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 86; }
  goto ret0;
 L421:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L422; }
  goto ret0;
 L422:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 89; }
  goto ret0;
 L436:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L437; }
  goto ret0;
 L437:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 92; }
  goto ret0;
 L451:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L452; }
  goto ret0;
 L452:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 95; }
  goto ret0;
 L466:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L467; }
  goto ret0;
 L467:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 98; }
  goto ret0;
 L481:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L482; }
  goto ret0;
 L482:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 101; }
  goto ret0;
 L2:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DImode))
    { recog_operand[1] = x1; return 0; }
  x1 = XEXP (x0, 0);
  goto L98;
 L99:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DImode))
    { recog_operand[1] = x1; return 23; }
  goto ret0;
 L87:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SFmode))
    { recog_operand[1] = x1; return 19; }
  x1 = XEXP (x0, 0);
  goto L89;
 L165:
  x1 = XEXP (x0, 1);
 switch (GET_CODE (x1))
  {
  case FLOAT_TRUNCATE:
  if (GET_MODE (x1) == SFmode && 1)
    goto L166;
  break;
  case FLOAT:
  if (GET_MODE (x1) == SFmode && 1)
    goto L170;
  break;
  case PLUS:
  if (GET_MODE (x1) == SFmode && 1)
    goto L208;
  break;
  case MINUS:
  if (GET_MODE (x1) == SFmode && 1)
    goto L243;
  break;
  case MULT:
  if (GET_MODE (x1) == SFmode && 1)
    goto L284;
  break;
  case DIV:
  if (GET_MODE (x1) == SFmode && 1)
    goto L294;
  break;
  case NEG:
  if (GET_MODE (x1) == SFmode && 1)
    goto L378;
  break;
  case ABS:
  if (GET_MODE (x1) == SFmode && 1)
    goto L386;
  break;
  }
 L90:
  if (general_operand (x1, SFmode))
    { recog_operand[1] = x1; return 20; }
  goto ret0;
 L166:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 39; }
  goto L90;
 L170:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; if (TARGET_80387) return 40; }
  goto L90;
 L208:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L209; }
  goto L90;
 L209:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 48; }
  goto L90;
 L243:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L244; }
  goto L90;
 L244:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 55; }
  goto L90;
 L284:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L285; }
  goto L90;
 L285:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 62; }
  goto L90;
 L294:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L295; }
  goto L90;
 L295:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 64; }
  goto L90;
 L378:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 79; }
  goto L90;
 L386:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 81; }
  goto L90;
 L93:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DFmode))
    { recog_operand[1] = x1; return 21; }
  x1 = XEXP (x0, 0);
  goto L95;
 L161:
  x1 = XEXP (x0, 1);
 switch (GET_CODE (x1))
  {
  case FLOAT_EXTEND:
  if (GET_MODE (x1) == DFmode && 1)
    goto L162;
  break;
  case FLOAT:
  if (GET_MODE (x1) == DFmode && 1)
    goto L174;
  break;
  case PLUS:
  if (GET_MODE (x1) == DFmode && 1)
    goto L203;
  break;
  case MINUS:
  if (GET_MODE (x1) == DFmode && 1)
    goto L238;
  break;
  case MULT:
  if (GET_MODE (x1) == DFmode && 1)
    goto L279;
  break;
  case DIV:
  if (GET_MODE (x1) == DFmode && 1)
    goto L289;
  break;
  case NEG:
  if (GET_MODE (x1) == DFmode && 1)
    goto L382;
  break;
  case ABS:
  if (GET_MODE (x1) == DFmode && 1)
    goto L390;
  break;
  }
 L96:
  if (general_operand (x1, DFmode))
    { recog_operand[1] = x1; return 22; }
  goto ret0;
 L162:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 38; }
  goto L96;
 L174:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; if (TARGET_80387) return 41; }
  goto L96;
 L203:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L204; }
  goto L96;
 L204:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 47; }
  goto L96;
 L238:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L239; }
  goto L96;
 L239:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 54; }
  goto L96;
 L279:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L280; }
  goto L96;
 L280:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 61; }
  goto L96;
 L289:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L290; }
  goto L96;
 L290:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; if (TARGET_80387) return 63; }
  goto L96;
 L382:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 80; }
  goto L96;
 L390:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; if (TARGET_80387) return 82; }
  goto L96;
 ret0: return -1;
}

int
recog_4 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L0:
 switch (GET_CODE (x0))
  {
  case SET:
  if (1)
    goto L4;
  break;
  case PARALLEL:
  if (XVECLEN (x0, 0) == 2 && 1)
    goto L13;
  break;
  case CALL:
  if (1)
    goto L685;
  break;
  case RETURN:
  if (1)
    if (0) return 128;
  break;
  }
  goto ret0;
 L4:
  return recog_3 (x0, insn);
 L13:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L14;
  goto ret0;
 L14:
  x2 = XEXP (x1, 0);
  if (x2 == cc0_rtx && 1)
    goto L42;
  if (x2 == pc_rtx && 1)
    goto L680;
 L262:
  if (general_operand (x2, HImode))
    { recog_operand[0] = x2; goto L263; }
 L270:
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L271; }
  goto ret0;
 L42:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == MINUS && 1)
    goto L43;
 L15:
  if (general_operand (x2, SFmode))
    { recog_operand[0] = x2; goto L16; }
 L21:
  if (general_operand (x2, DFmode))
    { recog_operand[0] = x2; goto L22; }
  x2 = XEXP (x1, 0);
  goto L262;
 L43:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[0] = x3; goto L44; }
 L51:
  if (general_operand (x3, SFmode))
    { recog_operand[0] = x3; goto L52; }
  goto L15;
 L44:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; goto L45; }
  x3 = XEXP (x2, 0);
  goto L51;
 L45:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L46;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L51;
 L46:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 9;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L51;
 L52:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; goto L53; }
  goto L15;
 L53:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L54;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L15;
 L54:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 10;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L15;
 L16:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L17;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L21;
 L17:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 4;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L21;
 L22:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L23;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L23:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 5;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L680:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L681; }
  x2 = XEXP (x1, 0);
  goto L262;
 L681:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L682;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L682:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L683;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L683:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[1] = x3; return 125; }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L263:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == UMULT && GET_MODE (x2) == SImode && 1)
    goto L264;
  x2 = XEXP (x1, 0);
  goto L270;
 L264:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, HImode))
    { recog_operand[1] = x3; goto L265; }
  x2 = XEXP (x1, 0);
  goto L270;
 L265:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, HImode))
    { recog_operand[2] = x3; goto L266; }
  x2 = XEXP (x1, 0);
  goto L270;
 L266:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L267;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L270;
 L267:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 1 && 1)
    return 59;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L270;
 L271:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  if (GET_CODE (x2) == UMULT && 1)
    goto L272;
  if (GET_CODE (x2) == DIV && 1)
    goto L300;
  if (GET_CODE (x2) == UDIV && 1)
    goto L311;
  goto ret0;
 L272:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L273; }
  goto ret0;
 L273:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L274; }
  goto ret0;
 L274:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L275;
  goto ret0;
 L275:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 1 && 1)
    return 60;
  goto ret0;
 L300:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L301; }
  goto ret0;
 L301:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L302; }
  goto ret0;
 L302:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L303;
  goto ret0;
 L303:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[3] = x2; goto L304; }
  goto ret0;
 L304:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == MOD && GET_MODE (x2) == SImode && 1)
    goto L305;
  goto ret0;
 L305:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, recog_operand[1]) && 1)
    goto L306;
  goto ret0;
 L306:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, recog_operand[2]) && 1)
    return 65;
  goto ret0;
 L311:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L312; }
  goto ret0;
 L312:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L313; }
  goto ret0;
 L313:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L314;
  goto ret0;
 L314:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[3] = x2; goto L315; }
  goto ret0;
 L315:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == UMOD && GET_MODE (x2) == SImode && 1)
    goto L316;
  goto ret0;
 L316:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, recog_operand[1]) && 1)
    goto L317;
  goto ret0;
 L317:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, recog_operand[2]) && 1)
    return 66;
  goto ret0;
 L685:
  x1 = XEXP (x0, 0);
  if (indirect_operand (x1, QImode))
    { recog_operand[0] = x1; goto L686; }
  goto ret0;
 L686:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    { recog_operand[1] = x1; return 126; }
  goto ret0;
 ret0: return -1;
}

int
recog (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L0:
 switch (GET_CODE (x0))
  {
  case SET:
  if (1)
    goto L4;
  break;
  case PARALLEL:
  if (XVECLEN (x0, 0) == 2 && 1)
    goto L13;
  break;
  case CALL:
  if (1)
    goto L685;
  break;
  case RETURN:
  if (1)
    if (0) return 128;
  break;
  }
  goto ret0;
 L4:
  return recog_3 (x0, insn);
 L13:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L14;
  goto ret0;
 L14:
  x2 = XEXP (x1, 0);
  if (x2 == cc0_rtx && 1)
    goto L42;
  if (x2 == pc_rtx && 1)
    goto L680;
 L262:
  if (general_operand (x2, HImode))
    { recog_operand[0] = x2; goto L263; }
 L270:
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L271; }
  goto ret0;
 L42:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == MINUS && 1)
    goto L43;
 L15:
  if (general_operand (x2, SFmode))
    { recog_operand[0] = x2; goto L16; }
 L21:
  if (general_operand (x2, DFmode))
    { recog_operand[0] = x2; goto L22; }
  x2 = XEXP (x1, 0);
  goto L262;
 L43:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[0] = x3; goto L44; }
 L51:
  if (general_operand (x3, SFmode))
    { recog_operand[0] = x3; goto L52; }
  goto L15;
 L44:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; goto L45; }
  x3 = XEXP (x2, 0);
  goto L51;
 L45:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L46;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L51;
 L46:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 9;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L51;
 L52:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; goto L53; }
  goto L15;
 L53:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L54;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L15;
 L54:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 10;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L15;
 L16:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L17;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L21;
 L17:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 4;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L21;
 L22:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L23;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L23:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 0 && 1)
    if (TARGET_80387) return 5;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L680:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L681; }
  x2 = XEXP (x1, 0);
  goto L262;
 L681:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L682;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L682:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L683;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L683:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[1] = x3; return 125; }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L262;
 L263:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == UMULT && GET_MODE (x2) == SImode && 1)
    goto L264;
  x2 = XEXP (x1, 0);
  goto L270;
 L264:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, HImode))
    { recog_operand[1] = x3; goto L265; }
  x2 = XEXP (x1, 0);
  goto L270;
 L265:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, HImode))
    { recog_operand[2] = x3; goto L266; }
  x2 = XEXP (x1, 0);
  goto L270;
 L266:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L267;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L270;
 L267:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == HImode && XINT (x2, 0) == 1 && 1)
    return 59;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L270;
 L271:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == UMULT && 1)
    goto L272;
  if (GET_CODE (x2) == DIV && 1)
    goto L300;
  if (GET_CODE (x2) == UDIV && 1)
    goto L311;
  goto ret0;
 L272:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L273; }
  goto ret0;
 L273:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L274; }
  goto ret0;
 L274:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L275;
  goto ret0;
 L275:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && GET_MODE (x2) == SImode && XINT (x2, 0) == 1 && 1)
    return 60;
  goto ret0;
 L300:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L301; }
  goto ret0;
 L301:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L302; }
  goto ret0;
 L302:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L303;
  goto ret0;
 L303:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[3] = x2; goto L304; }
  goto ret0;
 L304:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == MOD && GET_MODE (x2) == SImode && 1)
    goto L305;
  goto ret0;
 L305:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, recog_operand[1]) && 1)
    goto L306;
  goto ret0;
 L306:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, recog_operand[2]) && 1)
    return 65;
  goto ret0;
 L311:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SImode))
    { recog_operand[1] = x3; goto L312; }
  goto ret0;
 L312:
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    { recog_operand[2] = x3; goto L313; }
  goto ret0;
 L313:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L314;
  goto ret0;
 L314:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[3] = x2; goto L315; }
  goto ret0;
 L315:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == UMOD && GET_MODE (x2) == SImode && 1)
    goto L316;
  goto ret0;
 L316:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, recog_operand[1]) && 1)
    goto L317;
  goto ret0;
 L317:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, recog_operand[2]) && 1)
    return 66;
  goto ret0;
 L685:
  x1 = XEXP (x0, 0);
  if (indirect_operand (x1, QImode))
    { recog_operand[0] = x1; goto L686; }
  goto ret0;
 L686:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    { recog_operand[1] = x1; return 126; }
  goto ret0;
 ret0: return -1;
}
