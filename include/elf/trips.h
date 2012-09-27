/* TRIPS ELF support for BFD.
Copyright 1995, 1996, 1998, 2000, 2001 Free Software Foundation, Inc.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This file holds definitions specific to the TRIPS ELF ABI. 

--> I am "binutils/include/elf/trips.h"  <---

Most of the meat in this file has migrated to:
--> I am binutils/include/opcode/trips.h <--

Fri Sep 13 08:48:38 CDT 2002

*/

#ifndef _ELF_TRIPS_H
#define _ELF_TRIPS_H

#include "elf/reloc-macros.h"
#include "opcode/trips.h"

/* Relocations.  */
START_RELOC_NUMBERS (elf_trips_reloc_type)
	/* For TRIPS! */
  RELOC_NUMBER (R_TRIPS_NONE,      0)
  RELOC_NUMBER (R_TRIPS_HI_13,     1)
//  RELOC_NUMBER (R_TRIPS_MIDHI_13,  2)
  RELOC_NUMBER (R_TRIPS_MID_13,    3)
  RELOC_NUMBER (R_TRIPS_LO_13,     4)
  RELOC_NUMBER (R_TRIPS_BOTTOM_13, 5)
  RELOC_NUMBER (R_TRIPS_ADDR_64,   6)
  RELOC_NUMBER (R_TRIPS_ADDR_32,   7)
  RELOC_NUMBER (R_TRIPS_ADDR_26,   8)
  RELOC_NUMBER (R_TRIPS_ADDR_24,   9)
  RELOC_NUMBER (R_TRIPS_ADDR_16,  10)
  RELOC_NUMBER (R_TRIPS_ADDR_14,  11)
  RELOC_NUMBER (R_TRIPS_ADDR_8,   12)
	/* Following are NOT USED for TRIPS.... */
  RELOC_NUMBER (R_TRIPS_REL14_BRNTAKEN, 13)
  RELOC_NUMBER (R_TRIPS_GOT16,    14)
  RELOC_NUMBER (R_TRIPS_GOT16_LO, 15)
  RELOC_NUMBER (R_TRIPS_GOT16_HI, 16)
  RELOC_NUMBER (R_TRIPS_GOT16_HA, 17)
  RELOC_NUMBER (R_TRIPS_PLTREL24, 18)
  RELOC_NUMBER (R_TRIPS_COPY, 19)
  RELOC_NUMBER (R_TRIPS_GLOB_DAT, 20)
  RELOC_NUMBER (R_TRIPS_JMP_SLOT, 21)
  RELOC_NUMBER (R_TRIPS_RELATIVE, 22)
  RELOC_NUMBER (R_TRIPS_LOCAL24PC, 23)
  RELOC_NUMBER (R_TRIPS_UADDR32, 24)
  RELOC_NUMBER (R_TRIPS_UADDR16, 25)
  RELOC_NUMBER (R_TRIPS_REL32, 26)
  RELOC_NUMBER (R_TRIPS_PLT32, 27)
  RELOC_NUMBER (R_TRIPS_PLTREL32, 28)
  RELOC_NUMBER (R_TRIPS_PLT16_LO, 29)
  RELOC_NUMBER (R_TRIPS_PLT16_HI, 30)
  RELOC_NUMBER (R_TRIPS_PLT16_HA, 31)
  RELOC_NUMBER (R_TRIPS_SDAREL16, 32)
  RELOC_NUMBER (R_TRIPS_SECTOFF, 33)
  RELOC_NUMBER (R_TRIPS_SECTOFF_LO, 34)
  RELOC_NUMBER (R_TRIPS_SECTOFF_HI, 35)
  RELOC_NUMBER (R_TRIPS_SECTOFF_HA, 36)
  RELOC_NUMBER (R_TRIPS_ADDR30, 37)

/* The following relocs are from the 64-bit PowerPC ELF ABI. */
  RELOC_NUMBER (R_TRIPS64_ADDR64,		 38)
  RELOC_NUMBER (R_TRIPS64_ADDR16_HIGHER,	 39)
  RELOC_NUMBER (R_TRIPS64_ADDR16_HIGHERA,	 40)
  RELOC_NUMBER (R_TRIPS64_ADDR16_HIGHEST,	 41)
  RELOC_NUMBER (R_TRIPS64_ADDR16_HIGHESTA, 42)
  RELOC_NUMBER (R_TRIPS64_UADDR64,	 43)
  RELOC_NUMBER (R_TRIPS64_REL64,		 44)
  RELOC_NUMBER (R_TRIPS64_PLT64,		 45)
  RELOC_NUMBER (R_TRIPS64_PLTREL64,	 46)
  RELOC_NUMBER (R_TRIPS64_TOC16,		 47)
  RELOC_NUMBER (R_TRIPS64_TOC16_LO,	 48)
  RELOC_NUMBER (R_TRIPS64_TOC16_HI,	 49)
  RELOC_NUMBER (R_TRIPS64_TOC16_HA,	 50)
  RELOC_NUMBER (R_TRIPS64_TOC,		 51)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16,	 52)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16_LO,	 53)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16_HI,	 54)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16_HA,	 55)

/* The following relocs were added in the 64-bit PowerPC ELF ABI revision 1.2. */
  RELOC_NUMBER (R_TRIPS64_ADDR16_DS,       56)
  RELOC_NUMBER (R_TRIPS64_ADDR16_LO_DS,    57)
  RELOC_NUMBER (R_TRIPS64_GOT16_DS,        58)
  RELOC_NUMBER (R_TRIPS64_GOT16_LO_DS,     59)
  RELOC_NUMBER (R_TRIPS64_PLT16_LO_DS,     60)
  RELOC_NUMBER (R_TRIPS64_SECTOFF_DS,      61)
  RELOC_NUMBER (R_TRIPS64_SECTOFF_LO_DS,   62)
  RELOC_NUMBER (R_TRIPS64_TOC16_DS,        63)
  RELOC_NUMBER (R_TRIPS64_TOC16_LO_DS,     64)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16_DS,     65)
  RELOC_NUMBER (R_TRIPS64_PLTGOT16_LO_DS,  66)

/* The remaining relocs are from the Embedded ELF ABI, and are not
   in the SVR4 ELF ABI.  */
  RELOC_NUMBER (R_TRIPS_EMB_NADDR32, 101)
  RELOC_NUMBER (R_TRIPS_EMB_NADDR16, 102)
  RELOC_NUMBER (R_TRIPS_EMB_NADDR16_LO, 103)
  RELOC_NUMBER (R_TRIPS_EMB_NADDR16_HI, 104)
  RELOC_NUMBER (R_TRIPS_EMB_NADDR16_HA, 105)
  RELOC_NUMBER (R_TRIPS_EMB_SDAI16, 106)
  RELOC_NUMBER (R_TRIPS_EMB_SDA2I16, 107)
  RELOC_NUMBER (R_TRIPS_EMB_SDA2REL, 108)
  RELOC_NUMBER (R_TRIPS_EMB_SDA21, 109)
  RELOC_NUMBER (R_TRIPS_EMB_MRKREF, 110)
  RELOC_NUMBER (R_TRIPS_EMB_RELSEC16, 111)
  RELOC_NUMBER (R_TRIPS_EMB_RELST_LO, 112)
  RELOC_NUMBER (R_TRIPS_EMB_RELST_HI, 113)
  RELOC_NUMBER (R_TRIPS_EMB_RELST_HA, 114)
  RELOC_NUMBER (R_TRIPS_EMB_BIT_FLD, 115)
  RELOC_NUMBER (R_TRIPS_EMB_RELSDA, 116)

  RELOC_NUMBER (R_TRIPS_ADDR32, 117)
  RELOC_NUMBER (R_TRIPS_ADDR24, 118)
  RELOC_NUMBER (R_TRIPS_ADDR16, 119)
  RELOC_NUMBER (R_TRIPS_ADDR16_LO, 120)
  RELOC_NUMBER (R_TRIPS_ADDR16_HI, 121)

  /* These are GNU extensions to enable C++ vtable garbage collection.  */
  RELOC_NUMBER (R_TRIPS_GNU_VTINHERIT, 253)
  RELOC_NUMBER (R_TRIPS_GNU_VTENTRY, 254)

/* This is a phony reloc to handle any old fashioned TOC16 references
   that may still be in object files.  */
  RELOC_NUMBER (R_TRIPS_TOC16, 255)

END_RELOC_NUMBERS (R_TRIPS_max)

/* Aliases for R_TRIPS64-relocs. */
#define R_TRIPS64_NONE              R_TRIPS_NONE
#define R_TRIPS64_ADDR32            R_TRIPS_ADDR32
#define R_TRIPS64_ADDR24            R_TRIPS_ADDR24
#define R_TRIPS64_ADDR16            R_TRIPS_ADDR16
#define R_TRIPS64_ADDR16_LO         R_TRIPS_ADDR16_LO
#define R_TRIPS64_ADDR16_HI         R_TRIPS_ADDR16_HI
#define R_TRIPS64_ADDR16_HA         R_TRIPS_ADDR16_HA
#define R_TRIPS64_ADDR14            R_TRIPS_ADDR14
#define R_TRIPS64_ADDR14_BRTAKEN    R_TRIPS_ADDR14_BRTAKEN
#define R_TRIPS64_ADDR14_BRNTAKEN   R_TRIPS_ADDR14_BRNTAKEN
#define R_TRIPS64_REL24             R_TRIPS_REL24
#define R_TRIPS64_REL14             R_TRIPS_REL14
#define R_TRIPS64_REL14_BRTAKEN     R_TRIPS_REL14_BRTAKEN
#define R_TRIPS64_REL14_BRNTAKEN    R_TRIPS_REL14_BRNTAKEN
#define R_TRIPS64_GOT16             R_TRIPS_GOT16
#define R_TRIPS64_GOT16_LO          R_TRIPS_GOT16_LO
#define R_TRIPS64_GOT16_HI          R_TRIPS_GOT16_HI
#define R_TRIPS64_GOT16_HA          R_TRIPS_GOT16_HA
#define R_TRIPS64_COPY              R_TRIPS_COPY
#define R_TRIPS64_GLOB_DAT          R_TRIPS_GLOB_DAT
#define R_TRIPS64_JMP_SLOT          R_TRIPS_JMP_SLOT
#define R_TRIPS64_RELATIVE          R_TRIPS_RELATIVE
#define R_TRIPS64_UADDR32           R_TRIPS_UADDR32
#define R_TRIPS64_UADDR16           R_TRIPS_UADDR16
#define R_TRIPS64_REL32             R_TRIPS_REL32
#define R_TRIPS64_PLT32             R_TRIPS_PLT32
#define R_TRIPS64_PLTREL32          R_TRIPS_PLTREL32
#define R_TRIPS64_PLT16_LO          R_TRIPS_PLT16_LO
#define R_TRIPS64_PLT16_HI          R_TRIPS_PLT16_HI
#define R_TRIPS64_PLT16_HA          R_TRIPS_PLT16_HA
#define R_TRIPS64_SECTOFF           R_TRIPS_SECTOFF
#define R_TRIPS64_SECTOFF_LO        R_TRIPS_SECTOFF_LO
#define R_TRIPS64_SECTOFF_HI        R_TRIPS_SECTOFF_HI
#define R_TRIPS64_SECTOFF_HA        R_TRIPS_SECTOFF_HA
#define R_TRIPS64_ADDR30            R_TRIPS_ADDR30
#define R_TRIPS64_GNU_VTINHERIT	  R_TRIPS_GNU_VTINHERIT
#define R_TRIPS64_GNU_VTENTRY	  R_TRIPS_GNU_VTENTRY

/* Specify the start of the .glink section.  */
#define DT_TRIPS64_GLINK		DT_LOPROC

/* Specify the start and size of the .opd section.  */
#define DT_TRIPS64_OPD		(DT_LOPROC + 1)
#define DT_TRIPS64_OPDSZ		(DT_LOPROC + 2)

/* Processor specific flags for the ELF header e_flags field.  */

#define	EF_TRIPS_EMB		0x80000000	/* PowerPC embedded flag.  */

#define	EF_TRIPS_RELOCATABLE	0x00010000	/* PowerPC -mrelocatable flag.  */
#define	EF_TRIPS_RELOCATABLE_LIB	0x00008000	/* PowerPC -mrelocatable-lib flag.  */

/* Processor specific section headers, sh_type field.  */

#define SHT_ORDERED		SHT_HIPROC	/* Link editor is to sort the \
						   entries in this section \
						   based on the address \
						   specified in the associated \
						   symbol table entry.  */

/* Processor specific section flags, sh_flags field.  */

#define SHF_EXCLUDE		0x80000000	/* Link editor is to exclude \
						   this section from executable \
						   and shared objects that it \
						   builds when those objects \
						   are not to be furhter \
						   relocated.  */

/* 
   Macros for decomposing 64-bit relocatable addresses into 
   13-bit chunks for the TRIPS REPEAT instruction 
 */
#define BOTTOM_SHIFT(X) ((X) >> 0)
#define LO_SHIFT(X)     ((X) >> 13)
#define MID_SHIFT(X)    ((X) >> 26)
#define MIDHI_SHIFT(X)  ((X) >> 39)
#define HI_SHIFT(X)     ((X) >> 52)

/* for forming 32-bit addresses using a %hi/%bottom combo */
/* only use the upper 19 bits for the %hi portion */
#define X0_FORM_SHIFT(X) ((X) >> 13)
#define MASK_19(X) (X) & 0x7FFFF

#define MASK_13(X) (X) & 0x1FFF

#endif /* _ELF_TRIPS_H */
