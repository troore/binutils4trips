/* tc-trips.h -- Header file for tc-trips.c.
   Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
   Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#define TC_TRIPS
#define TRIPS

#define OBJ_DEFAULT_OUTPUT_FILE_NAME "t.out"

#ifdef ANSI_PROTOTYPES
struct fix;
#endif

/* Set the endianness we are using.  Default to big endian.  */
#ifndef TARGET_BYTES_BIG_ENDIAN
#define TARGET_BYTES_BIG_ENDIAN 1
#endif

#ifndef BFD_ASSEMBLER
 #error TRIPS support requires BFD_ASSEMBLER
#endif

/* SYNTAX using by read.h/read.c/expr.c/etc */
/* FYI, from read.h
    1 - may continue a name 
    2 - may begin a name 
    4 - ends a name 
*/

/* Commas are permitted in node and GPR names.  */
/* The TRIPS assembler permits commas(,) inside label names (001b)
*/
/* NO LONGER 12/17/02: #define LEX_COMMA 1 */

/* Brace are permitted to start and end node and GPR names.  */
/* NO LONGER 12/17/02: #define LEX_BR 6 */

/* The TRIPS assembler permits brackets to start/end label names.  */
/* Left brace to begin a name, as for "[0,1,0]", and
   to continue a name, as for "R[0]":  2 | 1 = 3  */
/* NO LONGER 12/17/02: #define LEX_LBR 3 */

/* Left bracket to continue a name */
#define LEX_LBR 1

/* Right bracket to end a name */
#define LEX_RBR 4

/* We need to promote 'R' to begin a register name ("2") and 
   as well as continue a label name ("1")  1 | 2 = 3 */

#define LEX_R 3

/* 12/17/02: Same for 'N' for node names */
#define LEX_N 3

/* If OBJ_COFF is defined, and TE_PE is not defined, we are assembling
   XCOFF for AIX or TripsMac.  If TE_PE is defined, we are assembling
   COFF for Windows NT.  */

#ifdef OBJ_COFF
#ifndef TE_PE
#define OBJ_XCOFF
#endif
#endif

/* The target BFD architecture.  */
#define TARGET_ARCH (trips_arch ())
#define TARGET_MACH (trips_mach ())
extern enum bfd_architecture trips_arch PARAMS ((void));
extern unsigned long trips_mach PARAMS ((void));

/* Whether or not the target is big endian */
extern int target_big_endian;

/* The target BFD format.  */
#define TARGET_FORMAT (trips_target_format ())
extern char *trips_target_format PARAMS ((void));

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB 1

/* $ is used to refer to the current location.  */
#define DOLLAR_DOT

/* Strings do not use backslash escapes under COFF.  */
#ifdef OBJ_COFF
#define NO_STRING_ESCAPES
#endif

#ifdef OBJ_ELF
#define DIFF_EXPR_OK		/* foo-. gets turned into PC relative relocs */
#endif

#if TARGET_BYTES_BIG_ENDIAN
#define TRIPS_BIG_ENDIAN 1
#else
#define TRIPS_BIG_ENDIAN 0
#endif

/* We don't need to handle .word strangely.  */
#define WORKING_DOT_WORD

#define MAX_MEM_FOR_RS_ALIGN_CODE 4
#define HANDLE_ALIGN(FRAGP)						\
  if ((FRAGP)->fr_type == rs_align_code) 				\
    {									\
      valueT count = ((FRAGP)->fr_next->fr_address			\
		      - ((FRAGP)->fr_address + (FRAGP)->fr_fix));	\
      if (count != 0 && (count & 3) == 0)				\
	{								\
	  unsigned char *dest = (FRAGP)->fr_literal + (FRAGP)->fr_fix;	\
									\
	  (FRAGP)->fr_var = 4;						\
	  if (target_big_endian)					\
	    {								\
	      *dest++ = 0x60;						\
	      *dest++ = 0;						\
	      *dest++ = 0;						\
	      *dest++ = 0;						\
	    }								\
	  else								\
	    {								\
	      *dest++ = 0;						\
	      *dest++ = 0;						\
	      *dest++ = 0;						\
	      *dest++ = 0x60;						\
	    }								\
	}								\
    }



#ifdef OBJ_ELF

/* Branch prediction relocations must force relocation, as must
   the vtable description relocs.  */
#define TC_FORCE_RELOCATION(FIXP)					\
((FIXP)->fx_r_type == BFD_RELOC_TRIPS_B16_BRTAKEN				\
 || (FIXP)->fx_r_type == BFD_RELOC_TRIPS_B16_BRNTAKEN			\
 || (FIXP)->fx_r_type == BFD_RELOC_TRIPS_BA16_BRTAKEN			\
 || (FIXP)->fx_r_type == BFD_RELOC_TRIPS_BA16_BRNTAKEN			\
 || (BFD_DEFAULT_TARGET_SIZE == 64					\
     && (FIXP)->fx_r_type == BFD_RELOC_TRIPS64_TOC)			\
 || (FIXP)->fx_r_type == BFD_RELOC_VTABLE_INHERIT			\
 || (FIXP)->fx_r_type == BFD_RELOC_VTABLE_ENTRY)

#define TC_FORCE_RELOCATION_SECTION(FIXP,SEC)				\
(TC_FORCE_RELOCATION (FIXP)						\
 || ((FIXP)->fx_addsy && !(FIXP)->fx_subsy				\
     && S_GET_SEGMENT ((FIXP)->fx_addsy) != SEC))

/* Support for SHF_EXCLUDE and SHT_ORDERED */
extern int trips_section_letter PARAMS ((int, char **));
extern int trips_section_type PARAMS ((char *, size_t));
extern int trips_section_word PARAMS ((char *, size_t));
extern int trips_section_flags PARAMS ((int, int, int));

#define md_cleanup 	trips_cleanup
void trips_cleanup PARAMS ((void));

#define md_elf_section_letter(LETTER, PTR_MSG)	trips_section_letter (LETTER, PTR_MSG)
#define md_elf_section_type(STR, LEN)		trips_section_type (STR, LEN)
#define md_elf_section_word(STR, LEN)		trips_section_word (STR, LEN)
#define md_elf_section_flags(FLAGS, ATTR, TYPE)	trips_section_flags (FLAGS, ATTR, TYPE)

#if BFD_DEFAULT_TARGET_SIZE == 64
/* Extra sections for 64-bit ELF TRIPS. */
#define ELF_TC_SPECIAL_SECTIONS \
  { ".toc",		SHT_PROGBITS,	SHF_ALLOC + SHF_WRITE}, \
  { ".tocbss",		SHT_NOBITS,	SHF_ALLOC + SHF_WRITE},
#else
/* Add extra TRIPS sections -- Note, for now, make .sbss2 and .TRIPS.EMB.sbss0 a
   normal section, and not a bss section so that the linker doesn't crater
   when trying to make more than 2 sections.  */
#define ELF_TC_SPECIAL_SECTIONS \
  { ".tags",		SHT_ORDERED,	SHF_ALLOC }, \
  { ".sdata",		SHT_PROGBITS,	SHF_ALLOC + SHF_WRITE }, \
  { ".sbss",		SHT_NOBITS,	SHF_ALLOC + SHF_WRITE }, \
  { ".sdata2",		SHT_PROGBITS,	SHF_ALLOC }, \
  { ".sbss2",		SHT_PROGBITS,	SHF_ALLOC }, \
  { ".TRIPS.EMB.sdata0",	SHT_PROGBITS,	SHF_ALLOC }, \
  { ".TRIPS.EMB.sbss0",	SHT_PROGBITS,	SHF_ALLOC },
#endif

/* #define tc_comment_chars trips_comment_chars */
/* extern const char *comment_chars; */

/* Keep relocations relative to the GOT, or non-PC relative.  */
#define tc_fix_adjustable(fixp) trips_fix_adjustable (fixp)
extern int trips_fix_adjustable PARAMS ((struct fix *));

/* We must never ever try to resolve references to externally visible
   symbols in the assembler, because the .o file might go into a shared
   library, and some other shared library might override that symbol.  */
#define TC_RELOC_RTSYM_LOC_FIXUP(FIX)  \
  ((FIX)->fx_addsy == NULL \
   || (! S_IS_EXTERNAL ((FIX)->fx_addsy) \
       && ! S_IS_WEAK ((FIX)->fx_addsy) \
       && S_IS_DEFINED ((FIX)->fx_addsy) \
       && ! S_IS_COMMON ((FIX)->fx_addsy)))

#if BFD_DEFAULT_TARGET_SIZE == 64
/* Finish up the symbol.  */
#define tc_frob_symbol(sym, punt) punt = trips_elf_frob_symbol (sym)
extern int trips_elf_frob_symbol PARAMS ((symbolS *));
#endif

#define DWARF2_LINE_MIN_INSN_LENGTH 4
#endif /* OBJ_ELF */

/* call md_pcrel_from_section, not md_pcrel_from */
#define MD_PCREL_FROM_SECTION(FIXP, SEC) md_pcrel_from_section(FIXP, SEC)
extern long md_pcrel_from_section PARAMS ((struct fix *, segT));

#define md_parse_name(name, exp, c) trips_parse_name (name, exp)
extern int trips_parse_name PARAMS ((const char *, struct expressionS *));

#define md_operand(x)
#define md_after_parse_args()             trips_after_parse_args ()
void trips_after_parse_args PARAMS ((void));

#define LISTING_HEADER "TAS LISTING"

/* lifted from tc-sparc.c, thank you very much. */
typedef struct trips_relocator {
  char *error;
  unsigned long opcode;
  struct nlist *nlistp;
  expressionS exp;
  expressionS exp2;
  int pcrel;
  bfd_reloc_code_real_type reloc;
} trips_relocator_t;

/*
	SYSTEM PARAMS
*/
/* Moved to binutils/include/elf/trips.h 
   as for TOTAL_NUM_REGISTERS
*/
