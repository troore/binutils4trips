/* BFD TRIPS CPU definition
   Copyright 1994, 1995, 1996, 2000, 2001, 2002
   Free Software Foundation, Inc.
   Contributed by Ian Lance Taylor, Cygnus Support.

This file is part of BFD, the Binary File Descriptor library.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"

/* The common TRIPS architecture offers both 32-bit and 64-bit formats  */

static const bfd_arch_info_type *trips_compatible
  PARAMS ((const bfd_arch_info_type *, const bfd_arch_info_type *));

static const bfd_arch_info_type *
trips_compatible (a,b)
     const bfd_arch_info_type *a;
     const bfd_arch_info_type *b;
{
  BFD_ASSERT (a->arch == bfd_arch_trips);
	if (b->arch == a->arch) {
    return a;
	}
	else {
		return NULL;
	}
}

const bfd_arch_info_type bfd_trips_archs[] =
{
#if BFD_DEFAULT_TARGET_SIZE == 64 
  {
    48,	/* 48 bits in an instruction word */
    64,	/* 64-bit address space */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    1,  /* mach identifier */
    "trips",
    "trips64:48x64", /* characterisitcs */
    3, /* aligment power--aligned on 64-bit boundaries */
    true, /* we are the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[1],
  },
  {
    64,	/* bits in a VLIW word */
    64,	/* 64-bit address space */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    2,  /* mach identifier */
    "trips64",
    "trips:2xN", /* VLIW of two words */
    3, /* aligment power */
    false, /* not the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[2],
  },
  {
    128, /* bits in a VLIW word */
    64,	/* 64-bit address space */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    4,  /* mach identifier */
    "trips",
    "trips64:4xN", /* VLIW of four words */
    3, /* aligment power */
    false, /* the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[3],
  },
  {
    256, /* bits in a VLIW word */
    64,	/* 64-bit address space */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    4,  /* mach identifier */
    "trips",
    "trips64:8xN", /* VLIW of eight words */
    3, /* aligment power */
    false, /* not the default configuration */
    trips_compatible,
    bfd_default_scan,
    0, /* end of the line */
  }
#else /* for the time being, we assume a 32-bit address space */
  {
    256, /* bits in a VLIW word */
    32,	/* 32 bits in an address */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    4,  /* mach identifier */
    "trips",
    "trips:8xN", /* VLIW of eight words */
    3, /* aligment power */
    true, /* the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[1], /* next field */
  },
  {
    64,	/* bits in a VLIW word */
    32,	/* 32 bits in an address */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    2,  /* mach identifier */
    "trips",
    "trips:2xN", /* VLIW of two words */
    3, /* aligment power */
    false, /* not the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[2], /* next field */
  },
  {
    128, /* bits in a VLIW word */
    32,	/* 32 bits in an address */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    3,  /* mach identifier */
    "trips",
    "trips:4xN", /* VLIW of four words */
    3, /* aligment power */
    false, /* not the default configuration */
    trips_compatible,
    bfd_default_scan,
    &bfd_trips_archs[3], /* next field */
  },
  {
    32,	/* just 32 bits in a VLIW word */
    32,	/* 32 bits in an address */
    8,	/* 8 bits in a byte */
    bfd_arch_trips,
    1,  /* mach identifier */
    "trips",
    "trips:1xN", /* VLIW of one word */
    3, /* aligment power */
    false, /* are we the default ? */
    trips_compatible,
    bfd_default_scan,
    0, /* end of the line */
  }
#endif
};
