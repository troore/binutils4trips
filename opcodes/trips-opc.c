/* trips-opc.c -- Trips opcode list */


#include <stdio.h>
#include "sysdep.h"
#include "opcode/trips.h"
#include "opintl.h"

#define OP_MATCH(x) ((((unsigned long)(x)) & 0x7f) << 25)
#define OP_MASK OP_MATCH(0x7f)

#define XOP_MATCH(x) ((((unsigned long)(x)) & 0x1f) << 18)
#define XOP_MASK XOP_MATCH(0x1f)

#define T0_MATCH(x) ((((unsigned long)(x)) & 0x1ff) << 0)
#define T0_MASK T0_MATCH(0x1ff)

#define T1_MATCH(x) ((((unsigned long)(x)) & 0x1ff) << 9)
#define T1_MASK T1_MATCH(0x1ff)

#define OFFSET_MATCH(x) ((((unsigned long)(x)) & 0xfffff) << 0)
#define OFFSET_MASK OFFSET_MATCH(0xfffff)

#define CONST_MATCH(x) ((((unsigned long)(x)) & 0xffff) << 9)
#define CONST_MASK OFFSET_MATCH(0xffff)

/* The opcode table.

   The format of the opcode table is:

   NAME	FORMAT	MASK	MATCH

   NAME is the name of the instruction.
   FORMAT is the instruction format.
   MASK is the instruction mask.
   MATCH is the value for known bits of the instruction.

   */

/** 00000000000000000000000000000000 **/
const struct trips_opcode trips_opcodes[] = {
{ "add",	G_FORM, OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x0)},
{ "and",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x8)},
{ "divs",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x3)},
{ "divu",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x4)},
{ "extsb",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x1)},
{ "extsh",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x2)},
{ "extsw",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x3)},
{ "extub",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x4)},
{ "extuh",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x5)},
{ "extuw",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x6)},
{ "fadd",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x0)},
{ "fdiv",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x3)},
{ "fdtio",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x12) | XOP_MATCH(0x1)},
{ "fdtos",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x12) | XOP_MATCH(0x3)},
{ "feq",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x4)},
{ "fge",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x9)},
{ "fgt",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x8)},
{ "fitod",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x12) | XOP_MATCH(0x0)},
{ "fle",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x5)},
{ "flt",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x6)},
{ "fmul",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x2)},
{ "fne",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x7)},
{ "fstod",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x12) | XOP_MATCH(0x2)},
{ "fsub",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x16) | XOP_MATCH(0x1)},
{ "mov",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x13) | XOP_MATCH(0x0)},
{ "mul",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x2)},
{ "null",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x10) | XOP_MATCH(0x0)},
{ "or",		G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x9)},
{ "sll",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0xc)},
{ "sra",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0xe)},
{ "srl",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0xd)},
{ "sub",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x1)},
{ "teq",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x10)},
{ "tge",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x17)},
{ "tgeu",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x19)},
{ "tgt",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x16)},
{ "tgtu",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x18)},
{ "tle",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x11)},
{ "tleu",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x13)},
{ "tlt",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x12)},
{ "tltu",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x14)},
{ "tne",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0x15)},
{ "xor",	G_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x17) | XOP_MATCH(0xa)},
{ "mov3",	M3_FORM,	OP_MASK,	OP_MATCH(0x06)},
{ "mov4",	M4_FORM,	OP_MASK | (1 << 24),	OP_MATCH(0x07) | (0 << 24)},
{ "addi",	I_FORM, OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x0)},
{ "movi",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x18) | XOP_MATCH(0X0)},
{ "divsi",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x3)},
{ "divui",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x4)},
{ "mfpc",	I_FORM,	OP_MASK | XOP_MASK | T1_MASK,	OP_MATCH(0x18) | XOP_MATCH(0x1) | T1_MATCH(0x0)},
{ "muli",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x2)},
{ "ori",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x9)},
{ "slli",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0xc)},
{ "srai",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0xe)},
{ "srli",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0xd)},
{ "subi",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x1)},
{ "teqi",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x10)},
{ "tgei",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x17)},
{ "tgeui",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x19)},
{ "tgti",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x16)},
{ "tgtui",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x18)},
{ "tlei",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x11)},
{ "tleui",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x13)},
{ "tlti",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x12)},
{ "tltui",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x14)},
{ "tnei",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0x15)},
{ "xori",	I_FORM,	OP_MASK | XOP_MASK,	OP_MATCH(0x1f) | XOP_MATCH(0xa)},
{ "br",		B_FORM,	OP_MASK | OFFSET_MASK,	OP_MATCH(0x0c) | OFFSET_MATCH(0x0)},
{ "bro",	B_FORM,	OP_MASK,	OP_MATCH(0x08)},
{ "call",	B_FORM,	OP_MASK | OFFSET_MASK,	OP_MATCH(0x0d) | OFFSET_MATCH(0x0)},
{ "callo",	B_FORM,	OP_MASK,	OP_MATCH(0x09)},
{ "ret",	B_FORM,	OP_MASK | OFFSET_MASK,	OP_MATCH(0x0e) | OFFSET_MATCH(0x0)},
{ "scall",	B_FORM,	OP_MASK | OFFSET_MASK,	OP_MATCH(0x0a) | OFFSET_MATCH(0X0)},
{ "app",	C_FORM,	OP_MASK,	OP_MATCH(0x04)},
{ "gens",	C_FORM,	OP_MASK,	OP_MATCH(0x01)},
{ "genu",	C_FORM,	OP_MASK,	OP_MATCH(0x02)},
{ "nop",	C_FORM,	OP_MASK | CONST_MASK | T0_MASK,	OP_MATCH(0x0) | CONST_MATCH(0x0) | T0_MATCH(0x0)},
{ "lb",		L_FORM,	OP_MASK,	OP_MATCH(0x20)},
{ "lbs",	L_FORM,	OP_MASK,	OP_MATCH(0x24)},
{ "ld",		L_FORM,	OP_MASK,	OP_MATCH(0x23)},
{ "lh",		L_FORM,	OP_MASK,	OP_MATCH(0x21)},
{ "lhs",	L_FORM,	OP_MASK,	OP_MATCH(0x25)},
{ "lock",	L_FORM,	OP_MASK,	OP_MATCH(0x27)},
{ "lw",		L_FORM,	OP_MASK,	OP_MATCH(0x22)},
{ "lws",	L_FORM,	OP_MASK,	OP_MATCH(0x26)},
{ "sb",		S_FORM,	OP_MASK | T0_MASK,	OP_MATCH(0x28) | T0_MATCH(0x0)},
{ "sd",		S_FORM,	OP_MASK | T0_MASK,	OP_MATCH(0x2b) | T0_MATCH(0x0)},
{ "sh",		S_FORM,	OP_MASK | T0_MASK,	OP_MATCH(0x29) | T0_MATCH(0x0)},
{ "sw",		S_FORM,	OP_MASK | T0_MASK,	OP_MATCH(0x2a) | T0_MATCH(0x0)},
{ "read",	R_FORM,	0xffc00000,	0x00000000},
{ "write",	W_FORM,	0xffffffc0,	0x00000000},
};
/** 00000000000000000000000000000000 **/

const int trips_num_opcodes = sizeof (trips_opcodes) / sizeof (trips_opcodes[0]);
