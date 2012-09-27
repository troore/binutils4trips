/* trips-dis.c -- Disassemble TRIPS instructions */

/* This file provides several disassembler functions, all of which use
   the disassembler interface defined in dis-asm.h.  Several functions
   are provided because this file handles disassembly for the TRIPS
   in both big and little endian mode.
*/

#include <stdio.h>
#include <string.h>
#include "sysdep.h"
#include "dis-asm.h"
#include "libiberty.h" /* for xmalloc() decl's */

#include "elf/internal.h"  /* to get the ELF header struct */
#include "elf/trips.h"  /* to get the TRIPS processor struct */
#include "../bfd/elf-bfd.h"  /* to get the ELF header struct */

/* Where GET_TRIPS_OP() GET_TRIPS_FORM() etc. are defined */
#include "opcode/trips.h"

/* Get the hw config params from the tcconfig.h file */
#include "opcode/tcconfig.h"

#include <fcntl.h>
/* for file ops */
#include <unistd.h>
/* for the file close() in _trips_free_fd_table() */

/* 
   Extra info from the calling routine (as in objdump) to this
   disassembler address printing function.

   It is bogus that we have to inline this structure in the C
   sources--possible versionitis problems.  This structure definition
   should be place in a header, probably binutils/include/dis-asm.h.
 */
struct objdump_disasm_info {
  bfd *abfd;
  asection *sec;
  boolean require_sec;
};


/* FUNCTIONS -- internal */
static int print_insn_trips PARAMS ((bfd_vma, struct disassemble_info *,
                                       int bigendian));

/* FUNCTIONS -- exportable -- extern-ed in include/opcode/trips.h */
int get_bits PARAMS ((int));

unsigned long long bfd_getb48 PARAMS ((bfd_byte []));

enum ok_type _trips_check_compatibility PARAMS ((struct trips_processor_obj *));
void _trips_free_instruction_specifier PARAMS ((struct trips_instruction_specifier *, struct trips_processor_obj *));

void _trips_free_node_specifier PARAMS ((struct trips_node_specifier *, 
   struct trips_processor_obj *));

void _trips_free_processor_specifier PARAMS ((trips_processor_obj_t *));

void _trips_free_stats PARAMS ((trips_aframe_stats_t *, trips_processor_obj_t *));

void _trips_make_sequence_number_table PARAMS((struct trips_processor_obj *));
trips_instruction_specifier_t * _trips_decode_word PARAMS ((trips_word,  trips_processor_obj_t * ));

void _trips_make_grid PARAMS ((struct trips_processor_obj *));

void _trips_make_operand_ports PARAMS ((struct trips_processor_obj *));

trips_aframe_stats_t * _trips_make_statistics PARAMS ((void));

trips_node_specifier_t * _trips_make_node_specifier PARAMS ((trips_processor_obj_t *));

struct trips_instruction_specifier * _trips_make_instruction_specifier PARAMS ((struct trips_processor_obj *));

struct trips_processor_obj * _trips_make_processor_specifier PARAMS((void));

void _trips_make_registers PARAMS ((struct trips_processor_obj *));

trips_processor_obj_t * init_as_necessary PARAMS ((bfd *, int));
/* END FUNCTIONS -- exportable */


#ifdef FAT_BITS 
#define BUFFER_SIZE 8
#define INSN_FORMAT_SPEC ".long 0x%.8x"
#else /* FAT_BITS */
#define BUFFER_SIZE 6
#define INSN_FORMAT_SPEC ".long 0x%.4x"
#endif /* FAT_BITS */

void
_trips_make_sequence_number_table (p)
struct trips_processor_obj * p;
{
   /* Note: we can't simply zero the entries because "0" is
      a valid pointer into the grid--namely, it points to
      node [0,0,0] as an ALU source node */
   /* 
   int i ;
   for (i=1; i < (MAX_NUM_ROWS_IN_GRID * MAX_NUM_COLS_IN_GRID * MAX_NUM_PFRAMES_IN_GRID); i++) {
     if (p) {
       p->sequence_number_table[i]  = UNUSED_SEQUENCE_NUMBER;
     }
     else {
       printf("_trips_make_sequence_number_table--trips-dis.c-->fix\n");
     }
   }
   */
   memset (p->sequence_number_table, (char)UNUSED_SEQUENCE_NUMBER, 
          sizeof(p->sequence_number_table));
   p->sequence_number_table[0] = 0;
}

void _trips_make_grid (p)
   struct trips_processor_obj * p;
{
   /* int r, c, f;
   for (r=0; r < MAX_NUM_ROWS_IN_GRID; r++) {
     for (c=0; c < MAX_NUM_COLS_IN_GRID; c++) {
       for (f=0; f < MAX_NUM_PFRAMES_IN_GRID; f++) {
         if (p) {
             p->grid[r][c][f] = 0;
         }
         else {
             printf("_trips_make_sequence_grid--trips-dis.c-->fix\n");
         }
       }
     }
   }
   */
   memset (p->grid, '\0', sizeof(p->grid));
}

trips_node_specifier_t *
_trips_make_node_specifier (pp)
  trips_processor_obj_t *pp;
{
    struct trips_node_specifier * p; 
    if (! pp) {
        return NULL;
    }
    p = xmalloc (sizeof (struct trips_node_specifier)); 
    /* 
    printf("mns: size: %d; ", sizeof (struct trips_node_specifier));
    printf("addr: 0x%x.\n", p); 
    */
    p->flat_name  = -1;
	p->slot = -1;
    p->ntype = ANY_TYPE;
    return p;
}

void _trips_make_registers (p)
   struct trips_processor_obj * p;
{
	int i;

	for (i = 0; i < HEADER_CHUNK_SIZE; ++i)
	{
		p->block_header_words[i] = 0;
		p->block_header[i].header_nibble = 0;
		p->block_header[i].read_insn = 0;
		p->block_header[i].write_insn = 0;
	}
}

trips_aframe_stats_t * 
_trips_make_statistics()
{
  trips_aframe_stats_t * pstat;
  pstat = (trips_aframe_stats_t *)(xmalloc (sizeof (trips_aframe_stats_t)));
  /* 
  printf("ts: size: %d; ", sizeof (trips_aframe_stats_t));
  printf("addr: 0x%x.\n", pstat);
  */
  pstat->store_flag = (unsigned long long)0;

  return pstat;
}

struct trips_processor_obj * 
_trips_make_processor_specifier ()
{
   struct trips_processor_obj * pp;
   
   pp = (struct trips_processor_obj *)(xmalloc (sizeof (struct trips_processor_obj)));
   /* load_hw_config_file(); */ /* defined in trips-config.c */
   pp->pframes_per_aframe = /* trips_config_table[INDEX_PFRAMES_PER_AFRAME].value */ 8;
   pp->x_config           = /* trips_config_table[INDEX_GRID_HEIGHT].value */ 4;
   pp->y_config           = /* trips_config_table[INDEX_GRID_WIDTH].value */ 4;
   pp->sequence_numbers  = 1; /* we expect sequence numbers */

   _trips_make_grid(pp); /* These update the global processor object. */
   _trips_make_sequence_number_table(pp);
   pp->aframe_stats = _trips_make_statistics();
   return pp;
}

void 
_trips_free_stats (pstats, pp) 
  trips_aframe_stats_t * pstats;
  trips_processor_obj_t * pp;
{
  /* Since there is only one set of statistics
     block per processor or aframe object, we don't need to pass in the
     thread object itself, but what the heck */
  if (! pp) {
    return;
  }
  if (pstats) {
    xfree(pstats);
    pstats = NULL;
  }
  pp->aframe_stats = NULL;
}

void 
_trips_free_processor_specifier (pp) 
  trips_processor_obj_t * pp;
{
  if (! pp) {
    return;
  }
  xfree(pp);
  pp = NULL;
}

void 
_trips_free_node_specifier (ns, pp) 
  struct trips_node_specifier * ns;
  struct trips_processor_obj * pp;
{
  if (! pp) {
    return;
  }
  if (ns) {
    xfree(ns);
    ns = NULL;
  }
}

void 
_trips_free_instruction_specifier (is, pp) 
  struct trips_instruction_specifier * is;
  struct trips_processor_obj * pp;
{
  if (is) {
    if (is->source_node) {
      _trips_free_node_specifier (is->source_node, pp);
    }
    if (is->target_one) {
      _trips_free_node_specifier (is->target_one, pp);
    }
    if (is->target_two) {
      _trips_free_node_specifier (is->target_two, pp);
    }
    if (is->target_three) {
      _trips_free_node_specifier (is->target_three, pp);
    }
    xfree(is);
    is = NULL;
  }
}

struct trips_instruction_specifier *
_trips_make_instruction_specifier (struct trips_processor_obj *pp)
{
	struct trips_instruction_specifier * is; 

	if (! pp) 
	{
		return NULL;
	}
	is = xmalloc (sizeof (struct trips_instruction_specifier)); 

	is->target_one		= (struct trips_node_specifier *)NULL;
	is->target_two		= (struct trips_node_specifier *)NULL;
	is->target_three	= (struct trips_node_specifier *)NULL;
	is->target_four		= (struct trips_node_specifier *)NULL;
	is->instruction_format =	UNKNOWN_FORM;
	is->insn_fields = xmalloc (sizeof (struct trips_instruction_fields));
	memset(is->insn_fields, 0, sizeof (struct trips_instruction_fields)); 
	/* is->insn_fields->CON = (trips_word)0;
	is->insn_fields->EXIT = 0;
	is->insn_fields->GR = 0;
	is->insn_fields->IMM = 0;
	is->insn_fields->LSID = 0;
	is->insn_fields->M3TX = 0;
	is->insn_fields->M4TX = 0;
	is->insn_fields->OFFSET = 0;
	is->insn_fields->PR = 0;
	is->insn_fields->V = 0;
	is->insn_fields->XOP = 0; */

	is->the_word = 0;
	is->sequence_number = BLANK_SEQUENCE_NUMBER;

	return is;
}

int
get_bits (dim)
  int dim;
{
  switch (dim) {
    case 128:
      return 7;
      break;
    case 64:
      return 6;
      break;
    case 32:
      return 5;
      break;
    case 16:
      return 4;
      break;
    case 8:
      return 3;
      break;
    case 4:
      return 2;
      break;
    case 2:
      return 1;
      break;
    case 1:
      return 0;
      break;
    default:
      printf("tas: bad bit specification: %d.", dim);
      return -1;
      break;
  }
}
