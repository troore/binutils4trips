/* tc-trips.c -- Assembler for the TRIPS Processor

   I AM THE ASSEMBLER FOR THE TRIPS PROCESSOR, or at least much
   of it.  I depend hugely for help on the frontend gas sources, on
   the BFD library, and on the opcode library.  
   
   My starting point was the tc-ppc.c module for the PowerPC
   assembler, for which I am very grateful.
   
*/

#include <stdio.h>
#include "safe-ctype.h"
#include "opcode/trips.h"
#include "elf/trips.h"
#include "dwarf2dbg.h"
#include "ansidecl.h" /* for PARAMS definitions in ../include/opcode/tcconfig.h */
#include "opcode/tcconfig.h"

#include "bfd.h"
#include "struc-symbol.h"

char TRIPS_VERSION_STRING[] = "$Revision: 1.208 $";

#define MAGIC_RELOC_VALUE 0x1dad

#define THREE_D_NODE_NAMES 0
#define FLAT_NODE_NAME 999
#define UNUSED_SEQUENCE_NUMBER_ENTRY ((UNUSED_SEQUENCE_NUMBER << 16) \
  | UNUSED_SEQUENCE_NUMBER)

/* Tell the main code what the endianness is.  */
extern int target_big_endian;

/* FUNCTIONS - external */
/* Refer to include/opcode/trips.h for a listing */

/* should go in the block structure */
static int visited_register_block  = 0;

static int R_BITS = 2;
static int C_BITS = 2;
static int F_BITS = 3;
static int R_MASK = 0x3;
static int C_MASK = 0x3;
static int F_MASK = 0x7;

static trips_instruction_specifier_t * fixup_grid[MAX_NUM_ROWS_IN_GRID][MAX_NUM_COLS_IN_GRID][MAX_NUM_PFRAMES_IN_GRID];

/* Whether or not we've set target_big_endian.  */
static int set_target_endian = 0;

/* For communication between sparc get_expression() and
trips_parse_field_operator() or _trips_parse_boffset_field() */
static char *expr_end;

/* Let us keep around this TRIPS object. */        
/* It is defined in include/elf/trips.h. */
static struct trips_processor_obj * tp = NULL;      

struct trips_block_specifier {
   char *name;
   unsigned int size;
   symbolS *sym;
   boolean valid_bend;
   boolean frame_initialized;
   int max_alu_flat_name;	/* The max index of all alu type instructions in a block */
   int frames_in_need;	/* frames_in_need = ((max_alu_flat_name % 32 == 0) ? 
						   (max_alu_flat_name / 32) : (max_alu_flat_name / 32 + 1)) * 2*/
};

static struct trips_block_specifier *current_block_specifier = NULL;
static struct trips_node_specifier  *last_completed_node     = NULL;

#define UNSPECIFIED_BLOCK_NAME_STRING "_unspecified_"

/*** parse_status codes ***********/
#define PARSED_OK                         0x00
#define ROW_TOO_BIG                       0x11
#define ROW_NEGATIVE                      0x12
#define ROW_NOT_A_DIGIT                   0x13
#define COLUMN_TOO_BIG                    0x14
#define COLUMN_NEGATIVE                   0x15
#define COLUMN_NOT_A_DIGIT                0x16
#define FRAME_TOO_BIG                     0x17
#define FRAME_NEGATIVE                    0x18
#define FRAME_NOT_A_DIGIT                 0x19
#define BANK_TOO_BIG                      0x1A
#define BANK_NOT_A_DIGIT                  0x1B
#define BAD_NODE_CODE                     0xFFF
#define RELATIVE_SPEC_FOR_SOURCE_NODE     0x1C
#define BAD_NODE_START                    0x1D
#define UNDEFINED                         0x1E
#define GPR_BANK_TOO_BIG                  0x1F
#define GPR_ENTRY_TOO_BIG                 0x20
#define GPR_BANK_NOT_A_DIGIT              0x21
#define GPR_ENTRY_NOT_A_DIGIT             0x22
#define COLUMN_TOO_BIG_RELATIVE           0x34
#define ROW_TOO_BIG_X_BITS                0x35

#define UNPARSED 0x01
#define INVALID_SLOT_NUMBER 0x36
#define NODE_DELIMETER_LOST 0x37

#define MISSING_COLUMN_SPECIFIER          0x36
#define MISSING_FRAME_SPECIFIER           0x37
#define MISSING_BANK_SPECIFIER            0x38
#define GPR_SPECIFIER_INVALID             0x39
#define NO_OFFSETS_FOR_ABSOLUTE           0x40
#define ALU_NAME_NOT_A_DIGIT              0x41
#define ALU_NAME_TOO_BIG                  0x42
#define ALPHA_NODE_INCORRECT              0x43
#define SOURCE_NODE_DISALLOWS_RS_SPEC     0x44


/* Macros for calculating LO, HI, HA, HIGHER, HIGHERA, HIGHEST,
   HIGHESTA.  */

/* #lo(value) denotes the least significant 16 bits of the indicated.  */
#define TRIPS_LO(v) ((v) & 0xffff)

/* #hi(value) denotes bits 16 through 31 of the indicated value.  */
#define TRIPS_HI(v) (((v) >> 16) & 0xffff)

/* #ha(value) denotes the high adjusted value: bits 16 through 31 of
  the indicated value, compensating for #lo() being treated as a
  signed number.  */
#define TRIPS_HA(v) TRIPS_HI ((v) + 0x8000)

/* #higher(value) denotes bits 32 through 47 of the indicated value.  */
#define TRIPS_HIGHER(v) (((v) >> 32) & 0xffff)

/* #highera(value) denotes bits 32 through 47 of the indicated value,
   compensating for #lo() being treated as a signed number.  */
#define TRIPS_HIGHERA(v) TRIPS_HIGHER ((v) + 0x8000)

/* #highest(value) denotes bits 48 through 63 of the indicated value.  */
#define TRIPS_HIGHEST(v) (((v) >> 48) & 0xffff)

/* #highesta(value) denotes bits 48 through 63 of the indicated value,
   compensating for #lo being treated as a signed number.  */
#define TRIPS_HIGHESTA(v) TRIPS_HIGHEST ((v) + 0x8000)

#define SEX16(val) ((((val) & 0xffff) ^ 0x8000) - 0x8000)

static int get_expression PARAMS ((char *, trips_relocator_t *));
static int get_expression2 PARAMS ((char *, trips_relocator_t *));

static int trips_parse_field_operator PARAMS ((char *, 
  trips_instruction_specifier_t * ));

void trips_after_parse_args PARAMS ((void));

static struct trips_input_block_header_type _trips_make_input_block_header PARAMS((void));

// static enum ok_type _trips_check_sequence_number_table PARAMS((struct trips_processor_obj *));

static struct trips_block_specifier *  _trips_make_block_specifier PARAMS ((char *, symbolS *));

static trips_word _trips_build_the_instruction  PARAMS ((trips_instruction_specifier_t *, trips_processor_obj_t *));

static trips_word _trips_get_normal_target_value PARAMS ((struct trips_node_specifier *));

static void _trips_parse_immediate_field PARAMS ((struct trips_instruction_specifier *, char **));

static void _trips_parse_boffset_field PARAMS ((struct trips_instruction_specifier *, char **));

static void  _trips_place_read_or_write_instruction_in_block_header  PARAMS ((trips_instruction_specifier_t *, trips_processor_obj_t *));

static void _trips_place_normal_instruction_in_grid  PARAMS ((trips_instruction_specifier_t *, trips_processor_obj_t *));

static void _trips_close_block_specifier PARAMS ((struct trips_block_specifier * ));

void trips_cleanup  PARAMS ((void));

static void _trips_make_fixup_grid PARAMS ((void));

static void _trips_flush_registers PARAMS ((trips_processor_obj_t *));

static void _trips_flush_grid PARAMS ((trips_processor_obj_t *));

static void _trips_pseudo_op_block_begin PARAMS ((int));
static void _trips_pseudo_op_block_end PARAMS ((int));
static void _trips_pseudo_op_extract PARAMS ((int));
static void _trips_pseudo_op_sequence PARAMS ((int));

/* static void trips_macro PARAMS ((char *str, const struct trips_macro *macro)); */
static void trips_byte PARAMS ((int));
static int _trips_capture_specific_alu_node PARAMS ((char *, struct trips_node_specifier *));

static void trips_set_fix PARAMS ((trips_instruction_specifier_t *, int ));

trips_relocator_t the_insn; /* stop the global-ness... */

#ifdef OBJ_ELF
static bfd_reloc_code_real_type trips_elf_suffix PARAMS ((char **, expressionS *));
static void trips_elf_cons PARAMS ((int));
static void trips_elf_rdata PARAMS ((int));
static void trips_elf_lcomm PARAMS ((int));
#endif

struct trips_node_specifier * trips_find_node PARAMS ((char *, int *));
static int _trips_capture_any_node PARAMS ((char *, struct trips_node_specifier *));

static void _trips_deal_with_invalid_node PARAMS ((char *, struct trips_node_specifier *));

static enum ok_type _trips_check_operand_range PARAMS ((trips_instruction_specifier_t *, trips_processor_obj_t *));

static void _trips_deal_with_invalid_instruction PARAMS ((char *, struct trips_instruction_specifier *));

/* Generic assembler global variables which must be defined by all
   targets.  */

/* used by app.c */
const char comment_chars[] = ";";

/* Characters which start a comment at the beginning of a line.  */
const char line_comment_chars[] = ";*#";

/* Characters which may be used to separate multiple commands on a single
   line. The semicolon is such a character by default and should not be
   explicitly listed.  */
const char line_separator_chars[] = "";

/* Characters which are used to indicate an exponent in a floating
   point number.  */
const char EXP_CHARS[] = "eE";

/* Characters which mean that a number is a floating point constant,
   as in 0d1.0.  */
const char FLT_CHARS[] = "dD";

/* The target specific pseudo-ops which we support.  */

const pseudo_typeS md_pseudo_table[] =
{
  /* Pseudo-ops for TRIPS.  */
  /* Pseudo-ops from read.c which are overridden.  */
  {"byte",        trips_byte,        0 },
  {"align",       s_align_bytes,     0}, 
  /* Defined like the SPARC (tc-sparc.c) and UNLIKE the PPC (tc-ppc.c) */
  /* TRIPS-specific pseudo-ops                        */
  {"bbegin",            _trips_pseudo_op_block_begin, 0},
  {"bend",              _trips_pseudo_op_block_end,   0},
  {"extract",           _trips_pseudo_op_extract,     0},
  {"sequence_numbers",  _trips_pseudo_op_sequence,    0},
  { "llong",  cons,                  8 },
  { "quad",   cons,                  8 },
  { "long",   cons,                  4 }, /* NOT 8 bytes - 06/19/03 - dcboyed */
  { "word",   cons,                  2 }, /* NOT 8 bytes - 06/19/03 - dcboyed */
  { "short",  cons,                  2 },
  { "rdata",  trips_elf_rdata,       0 },
  { "rodata", trips_elf_rdata,       0 },
  { "lcomm",  trips_elf_lcomm,       0 },
  { "file",   dwarf2_directive_file, 0 },
  { "loc",    dwarf2_directive_loc,  0 },
  { NULL,        NULL,                0 }
};



/* Local variables.  */

/* The type of processor we are assembling for.  This is one or more
   of the TRIPS_OPCODE flags defined in opcode/trips.h.  */
static int trips_cpu = 0;

/* The size of the processor we are assembling for.  This is either
   TRIPS_OPCODE_32 or TRIPS_OPCODE_64.  */
static unsigned long trips_size = (BFD_DEFAULT_TARGET_SIZE == 64
                                 ? TRIPS_OPCODE_64
                                 : TRIPS_OPCODE_32);

/* Whether to target xcoff64.  */
static int trips_xcoff64 = 0;

/* Opcode hash table.  */
static struct hash_control *trips_hash;

/* Macro hash table.  */
/* static struct hash_control *trips_macro_hash; */

#ifdef OBJ_ELF
/* What type of shared library support to use.  */
static enum { SHLIB_NONE, SHLIB_PIC, SHLIB_MRELOCATABLE } shlib = SHLIB_NONE;

symbolS *GOT_symbol;                /* Pre-defined "_GLOBAL_OFFSET_TABLE" */
#endif /* OBJ_ELF */
/* Flags to set in the elf header.  */
static flagword trips_flags = 0;

#ifdef OBJ_ELF
const char *const md_shortopts = "b:p:w:l:ceusm:K:VQ:";
#else
const char *const md_shortopts = "um:";
#endif
const struct option md_longopts[] = {
  {NULL, no_argument, NULL, 0}
};
const size_t md_longopts_size = sizeof (md_longopts);

int
md_parse_option (c, arg)
     int c;
     char *arg;
{
  char *s = arg;
  int i,x,y;

  if (! tp) {
      /* ugh--have to place these in two separate locations */
      tp = _trips_make_processor_specifier();           
      tp->initialized = true;
      visited_register_block  = 0;
  }

  switch (c)
  {
  }

  return 1;
}

void
md_show_usage (stream)
     FILE *stream;
{
  fprintf (stream, _("\
TRIPS-specific options:\n\
-b<n>                   Assemble for <n> bits per x,y,z specifier; default is 3.\n\
-c                      Cause the linker to compress the binary code.\n\
-E                      Produce extended debugging information.\n\
-e                      Emit ASCII-readable binary code for portability.\n\
-mNxM                   Assemble for an NxM grid, where N and M\n\
                        are powers of 2: 4,8,16,32. (Default is -m8x8.)\n"));
  fprintf (stream, _("\
-p<n>                   Assemble for <n> p-frames per a-frame; default is 128.\n\
-s                      Turn off sequence numbers processing; default is 'on'.\n\
-V                      Print assembler version number.\n\
-w32                    Generate code for 32-bit word-size (default).\n\
-w64                    Generate code for 64-bit word-size.\n"));
}


/* State the BFD architecture to use.  */

enum bfd_architecture
trips_arch ()
{
  return bfd_arch_trips;
}

unsigned long
trips_mach ()
{
  trips_cpu = TRIPS_OPCODE_TRIPS;
  if (! tp) {
      /* ugh--have to place these in two separate locations */
      tp = _trips_make_processor_specifier();           
      visited_register_block  = 0;
      tp->initialized = true;

  }
  bfd_usrdata(stdoutput) = tp;
  /* bfd_set_private_flags(stdoutput, 0xdeadbeef); */
  return trips_size == TRIPS_OPCODE_64 ? 620 : 0;
}
extern char *
trips_target_format ()
{
#ifdef OBJ_ELF
  boolean is64 = BFD_DEFAULT_TARGET_SIZE == 64 && trips_size == TRIPS_OPCODE_64;

  return (target_big_endian
          ? (is64 ? "elf64-trips" : "elf32-trips")
          : (is64 ? "elf64-tripsle" : "elf32-tripsle"));
#endif
}

/* This function is called when the assembler starts up.  It is called
   after the options have been parsed and the output file has been
   opened.  */

void
md_begin ()
{
  register const struct trips_opcode *op;
  const struct trips_opcode *op_end;
 /*  const struct trips_macro *macro; */
 /*  const struct trips_macro *macro_end; */
  boolean dup_insn = false;

  /* Insert the opcodes into a hash table.  */
  trips_hash = hash_new ();

  op_end = trips_opcodes + trips_num_opcodes;
  for (op = trips_opcodes; op < op_end; op++) {
    char *retval;
    retval = (char *)hash_insert (trips_hash, op->name, (PTR) op);
    if (retval != (char *) NULL) {
      as_bad (_("tas: problem assimilating instruction %s."), op->name);
      dup_insn = true;
    }
  }
  /* Insert the macros into a hash table.  */
  /* trips_macro_hash = hash_new (); */

  /* macro_end = trips_macros + trips_num_macros; 
  for (macro = trips_macros; macro < macro_end; macro++) {
    const char *retval;
    retval = hash_insert (trips_macro_hash, macro->name, (PTR) macro);
    if (retval != (const char *) NULL) {
      as_bad (_("Internal assembler error for macro %s"), macro->name);
      dup_insn = true;
    }
  } */
  if (dup_insn)
    abort ();

  /* set up bit fields */
  if  (tp) {
    R_BITS = get_bits (tp->x_config);
    C_BITS = get_bits (tp->y_config);
    F_BITS = get_bits (tp->pframes_per_aframe);
    R_MASK = tp->x_config - 1;
    C_MASK = tp->y_config - 1;
    F_MASK = tp->pframes_per_aframe - 1;
  }

  /* Tell the main code what the endianness is if it is not overidden
     by the user.  */
  if (!set_target_endian)
    {
      set_target_endian = 1;
      target_big_endian = TRIPS_BIG_ENDIAN;
    }


}

#ifdef OBJ_ELF
/* Parse @got, etc. and return the desired relocation.  */
static bfd_reloc_code_real_type
trips_elf_suffix (str_p, exp_p)
     char **str_p;
     expressionS *exp_p;
{
  struct map_bfd {
    char *string;
    int length;
    int reloc;
  };

  char ident[20];
  char *str = *str_p;
  char *str2;
  int ch;
  int len;
  const struct map_bfd *ptr;

#define MAP(str,reloc) { str, sizeof (str)-1, reloc }

  static const struct map_bfd mapping[] = {
    MAP ("l",                (int) BFD_RELOC_LO16),
    MAP ("h",                (int) BFD_RELOC_HI16),
    MAP ("ha",                (int) BFD_RELOC_HI16_S),
    MAP ("brtaken",        (int) BFD_RELOC_TRIPS_B16_BRTAKEN),
    MAP ("brntaken",        (int) BFD_RELOC_TRIPS_B16_BRNTAKEN),
    MAP ("got",                (int) BFD_RELOC_16_GOTOFF),
    MAP ("got@l",        (int) BFD_RELOC_LO16_GOTOFF),
    MAP ("got@h",        (int) BFD_RELOC_HI16_GOTOFF),
    MAP ("got@ha",        (int) BFD_RELOC_HI16_S_GOTOFF),
    MAP ("fixup",        (int) BFD_RELOC_CTOR), /* warning with -mrelocatable */
    MAP ("plt",                (int) BFD_RELOC_24_PLT_PCREL),
    MAP ("pltrel24",        (int) BFD_RELOC_24_PLT_PCREL),
    MAP ("copy",        (int) BFD_RELOC_TRIPS_COPY),
    MAP ("globdat",        (int) BFD_RELOC_TRIPS_GLOB_DAT),
    MAP ("local24pc",        (int) BFD_RELOC_TRIPS_LOCAL24PC),
    MAP ("local",        (int) BFD_RELOC_TRIPS_LOCAL24PC),
    MAP ("pltrel",        (int) BFD_RELOC_32_PLT_PCREL),
    MAP ("plt@l",        (int) BFD_RELOC_LO16_PLTOFF),
    MAP ("plt@h",        (int) BFD_RELOC_HI16_PLTOFF),
    MAP ("plt@ha",        (int) BFD_RELOC_HI16_S_PLTOFF),
    MAP ("sdarel",        (int) BFD_RELOC_GPREL16),
    MAP ("sectoff",        (int) BFD_RELOC_32_BASEREL),
    MAP ("sectoff@l",        (int) BFD_RELOC_LO16_BASEREL),
    MAP ("sectoff@h",        (int) BFD_RELOC_HI16_BASEREL),
    MAP ("sectoff@ha",        (int) BFD_RELOC_HI16_S_BASEREL),
    MAP ("naddr",        (int) BFD_RELOC_TRIPS_EMB_NADDR32),
    MAP ("naddr16",        (int) BFD_RELOC_TRIPS_EMB_NADDR16),
    MAP ("naddr@l",        (int) BFD_RELOC_TRIPS_EMB_NADDR16_LO),
    MAP ("naddr@h",        (int) BFD_RELOC_TRIPS_EMB_NADDR16_HI),
    MAP ("naddr@ha",        (int) BFD_RELOC_TRIPS_EMB_NADDR16_HA),
    MAP ("sdai16",        (int) BFD_RELOC_TRIPS_EMB_SDAI16),
    MAP ("sda2rel",        (int) BFD_RELOC_TRIPS_EMB_SDA2REL),
    MAP ("sda2i16",        (int) BFD_RELOC_TRIPS_EMB_SDA2I16),
    MAP ("sda21",        (int) BFD_RELOC_TRIPS_EMB_SDA21),
    MAP ("mrkref",        (int) BFD_RELOC_TRIPS_EMB_MRKREF),
    MAP ("relsect",        (int) BFD_RELOC_TRIPS_EMB_RELSEC16),
    MAP ("relsect@l",        (int) BFD_RELOC_TRIPS_EMB_RELST_LO),
    MAP ("relsect@h",        (int) BFD_RELOC_TRIPS_EMB_RELST_HI),
    MAP ("relsect@ha",        (int) BFD_RELOC_TRIPS_EMB_RELST_HA),
    MAP ("bitfld",        (int) BFD_RELOC_TRIPS_EMB_BIT_FLD),
    MAP ("relsda",        (int) BFD_RELOC_TRIPS_EMB_RELSDA),
    MAP ("xgot",        (int) BFD_RELOC_TRIPS_TOC16),
#if BFD_DEFAULT_TARGET_SIZE == 64
    MAP ("higher",        - (int) BFD_RELOC_TRIPS64_HIGHER),
    MAP ("highera",        - (int) BFD_RELOC_TRIPS64_HIGHER_S),
    MAP ("highest",        - (int) BFD_RELOC_TRIPS64_HIGHEST),
    MAP ("highesta",        - (int) BFD_RELOC_TRIPS64_HIGHEST_S),
    MAP ("tocbase",        - (int) BFD_RELOC_TRIPS64_TOC),
    MAP ("toc",                - (int) BFD_RELOC_TRIPS_TOC16),
    MAP ("toc@l",        - (int) BFD_RELOC_TRIPS64_TOC16_LO),
    MAP ("toc@h",        - (int) BFD_RELOC_TRIPS64_TOC16_HI),
    MAP ("toc@ha",        - (int) BFD_RELOC_TRIPS64_TOC16_HA),
#endif
    { (char *) 0, 0,        (int) BFD_RELOC_UNUSED }
  };

  if (*str++ != '@')
    return BFD_RELOC_UNUSED;

  for (ch = *str, str2 = ident;
       (str2 < ident + sizeof (ident) - 1
        && (ISALNUM (ch) || ch == '@'));
       ch = *++str)
    {
      *str2++ = TOLOWER (ch);
    }

  *str2 = '\0';
  len = str2 - ident;

  ch = ident[0];
  for (ptr = &mapping[0]; ptr->length > 0; ptr++)
    if (ch == ptr->string[0]
        && len == ptr->length
        && memcmp (ident, ptr->string, ptr->length) == 0)
      {
        int reloc = ptr->reloc;

        if (BFD_DEFAULT_TARGET_SIZE == 64 && reloc < 0)
          {
            if (trips_size != TRIPS_OPCODE_64)
              return BFD_RELOC_UNUSED;
            reloc = -reloc;
          }

        if (exp_p->X_add_number != 0
            && (reloc == (int) BFD_RELOC_16_GOTOFF
                || reloc == (int) BFD_RELOC_LO16_GOTOFF
                || reloc == (int) BFD_RELOC_HI16_GOTOFF
                || reloc == (int) BFD_RELOC_HI16_S_GOTOFF))
          as_warn (_("identifier+constant@got means identifier@got+constant"));

        /* Now check for identifier@suffix+constant.  */
        if (*str == '-' || *str == '+')
          {
            char *orig_line = input_line_pointer;
            expressionS new_exp;

            input_line_pointer = str;
            expression (&new_exp);
            if (new_exp.X_op == O_constant)
              {
                exp_p->X_add_number += new_exp.X_add_number;
                str = input_line_pointer;
              }

            if (&input_line_pointer != str_p)
              input_line_pointer = orig_line;
          }
        *str_p = str;

        if (BFD_DEFAULT_TARGET_SIZE == 64
            && reloc == (int) BFD_RELOC_TRIPS64_TOC
            && exp_p->X_op == O_symbol)
          {
            /* This reloc type ignores the symbol.  Change the symbol
               so that the dummy .TOC. symbol can be omitted from the
               object file.  */
            exp_p->X_add_symbol = &abs_symbol;
          }

        return (bfd_reloc_code_real_type) reloc;
      }

  return BFD_RELOC_UNUSED;
}

/* DO NOT USE UNTIL FURTHER NOTICE.  
   We can handle all these fixups through the
   generic emit_expr() routine in read.c */

/* Like normal .long/.short/.word, except support @got, etc.
   Clobbers input_line_pointer, checks end-of-line.  */
static void
trips_elf_cons (nbytes)
     register int nbytes;        /* 1=.byte, 2=.word, 4=.long, 8=.llong.  */
{
	/* Contents in this function are temporarily deleted by troore. */
}

/* Solaris pseduo op to change to the .rodata section.  */
static void
trips_elf_rdata (xxx)
     int xxx;
{
  char *save_line = input_line_pointer;
  static char section[] = ".rodata\n";

  /* Just pretend this is .section .rodata  */
  input_line_pointer = section;
  obj_elf_section (xxx);

  input_line_pointer = save_line;
}

/* Pseudo op to make file scope bss items.  */
static void
trips_elf_lcomm (xxx)
     int xxx ATTRIBUTE_UNUSED;
{
  register char *name;
  register char c;
  register char *p;
  offsetT size;
  register symbolS *symbolP;
  offsetT align;
  segT old_sec;
  int old_subsec;
  char *pfrag;
  int align2;

  name = input_line_pointer;
  c = get_symbol_end ();

  /* just after name is now '\0'.  */
  p = input_line_pointer;
  *p = c;
  SKIP_WHITESPACE ();
  if (*input_line_pointer != ',')
    {
      as_bad (_("Expected comma after symbol-name: rest of line ignored."));
      ignore_rest_of_line ();
      return;
    }

  input_line_pointer++;                /* skip ',' */
  if ((size = get_absolute_expression ()) < 0)
    {
      as_warn (_(".COMMon length (%ld.) <0! Ignored."), (long) size);
      ignore_rest_of_line ();
      return;
    }

  /* The third argument to .lcomm is the alignment.  */
  if (*input_line_pointer != ',')
    align = 8;
  else
    {
      ++input_line_pointer;
      align = get_absolute_expression ();
      if (align <= 0)
        {
          as_warn (_("ignoring bad alignment"));
          align = 8;
        }
    }

  *p = 0;
  symbolP = symbol_find_or_make (name);
  *p = c;

  if (S_IS_DEFINED (symbolP) && ! S_IS_COMMON (symbolP))
    {
      as_bad (_("Ignoring attempt to re-define symbol `%s'."),
              S_GET_NAME (symbolP));
      ignore_rest_of_line ();
      return;
    }

  if (S_GET_VALUE (symbolP) && S_GET_VALUE (symbolP) != (valueT) size)
    {
      as_bad (_("Length of .lcomm \"%s\" is already %ld. Not changed to %ld."),
              S_GET_NAME (symbolP),
              (long) S_GET_VALUE (symbolP),
              (long) size);

      ignore_rest_of_line ();
      return;
    }

  /* Allocate_bss.  */
  old_sec = now_seg;
  old_subsec = now_subseg;
  if (align)
    {
      /* Convert to a power of 2 alignment.  */
      for (align2 = 0; (align & 1) == 0; align >>= 1, ++align2);
      if (align != 1)
        {
          as_bad (_("Common alignment not a power of 2"));
          ignore_rest_of_line ();
          return;
        }
    }
  else
    align2 = 0;

  record_alignment (bss_section, align2);
  subseg_set (bss_section, 0);
  if (align2)
    frag_align (align2, 0, 0);
  if (S_GET_SEGMENT (symbolP) == bss_section)
    symbol_get_frag (symbolP)->fr_symbol = 0;
  symbol_set_frag (symbolP, frag_now);
  pfrag = frag_var (rs_org, 1, 1, (relax_substateT) 0, symbolP, size,
                    (char *) 0);
  *pfrag = 0;
  S_SET_SIZE (symbolP, size);
  S_SET_SEGMENT (symbolP, bss_section);
  subseg_set (old_sec, old_subsec);
  demand_empty_rest_of_line ();
}

#if BFD_DEFAULT_TARGET_SIZE == 64
/* Don't emit .TOC. symbol.  */
int
trips_elf_frob_symbol (sym)
     symbolS *sym;
{
  const char *name;

  name = S_GET_NAME (sym);
  if (name != NULL && strcmp (name, ".TOC.") == 0)
    {
      S_CLEAR_EXTERNAL (sym);
      return 1;
    }

  return 0;
}
#endif
#endif /* OBJ_ELF */



/* We need to keep a list of fixups.  We can't simply generate them as
   we go, because that would require us to first create the frag, and
   that would screw up references to ``.''.  */

struct trips_fixup
{
  expressionS exp;
  int opindex;
  bfd_reloc_code_real_type reloc;
};

#define MAX_INSN_FIXUPS (5)

/* This routine is called for each instruction to be assembled.  */

/* NOTE: For TRIPS, md_assemble will be called multiple times, one for
   each IW, to assemble a complete VLIW */

// char op_string[10];

void md_assemble (char *str)
{
	char *s, *pch = NULL;
	char op_string[10], lchar;
	const struct trips_opcode *popcode;
	unsigned long insn;
	int i;
	int res;
	int seq_num = -1;
	int pr = 1; /* The reserving PR value. */
	struct trips_instruction_specifier * i_spec;
	struct trips_node_specifier *src_n_spec, *t1_n_spec, *t2_n_spec, *t3_n_spec, *t4_n_spec;
	trips_word the_word = 0;
	int slot1, slot2, slot3, slot4;
	int msb2_1, msb2_2, msb2_3, msb2_4;

	/* so now create the object to hold all the info about the word. */
	i_spec = _trips_make_instruction_specifier (tp);

	/* Now get the source node. */
	res = 0;
	src_n_spec = trips_find_node (str, &res);
	if (res)
	{
		str += res;
	}
	if (src_n_spec)
	{
		i_spec->source_node = src_n_spec;
	}
	while (ISSPACE (*str))
	{
		str++;
	}

	/* Time to get the sequence number.  */
	if (*str == '<')
	{
		str++;
	}
	if (ISDIGIT(*str)) 
	{
		seq_num = (int)(*str - '0');
		str++;
		while (ISDIGIT (*str))
		{
			seq_num = (seq_num * 10) + (*str - '0');
			str++; 
		}
	}
	if (*str == '>') 
	{
		str++;
	}

	if (seq_num >= 0) 
	{
		i_spec->sequence_number = seq_num;
	}
	while (ISSPACE (*str))
	{
		++str;
	}
	/* Time to get the opcode.  */
	for (s = str, i = 0; *s != '\0' && ! ISSPACE (*s); s++)
	{
		op_string[i++] = TOLOWER(*s);
	}
	op_string[i] = '\0';

	/*Test whether this instruction is predicated or nor.  */
	pch = strchr (op_string, '_');
	if (pch)
	{
		char flag = op_string[pch - &op_string[0] + 1];
		if (flag == 't')
		{
			pr = 3;
		}
		else if (flag == 'f')
		{
			pr = 2;
		}
		else
		{
			as_bad(_("Unrecognized opcode: '%s'"), op_string);

			return;
		}
		*pch = '\0';
	}
	else
	{
		pr = 0;
	}
	/* Whether this instruction need this field or not, we'd better set it. */
	i_spec->insn_fields->PR = pr;

	/* Right now, 's', which is the same as 'str', should be
	   pointing at the non-space character just to the right of the 
	   opcode, typically a '(' or a '[' 
	   */

	/* Look up the lowercased opcode in the hash table.  */
	popcode = (const struct trips_opcode *) hash_find (trips_hash, op_string);
	if (popcode == (const struct trips_opcode *) NULL) 
	{
		as_bad (_("Unrecognized opcode: `%s'"), op_string);

		return;
	}
	i_spec->popcode = popcode;
	i_spec->instruction_format = popcode->format;
	/* insn = opcode->opcode; */ /* success--we've got a live one */

	/* nobody else needs the opcode--just this instruction */
	/* i_spec->opcode = opcode; */

	str = s;
	while (ISSPACE (*str))
	{
		++str;
	}

	i_spec->insn_fields->V = 1; /* Ditto. */
	i_spec->insn_fields->LSID = 0; /* Default lsid value. */
	i_spec->insn_fields->EXIT = 0; /* Default exit_id value. */
	i_spec->insn_fields->IMM = 0; /* Default immediate value. */

	/* Begin to deal with the str after opcode. */
	t1_n_spec = t2_n_spec = t3_n_spec = t4_n_spec = NULL;
	res = 0;
	if (str)
	{
		switch (i_spec->instruction_format)
		{
			case G_FORM:
			case R_FORM:
			case W_FORM:
				/* G: N[source_node] opcode N[target,slot] or
				 * G: N[source_node] opcode N[target,slot] N[target,slot] or
				 * R: R[read_queue_id] read G[arch_reg_id] N[target,slot] N[target,slot] or
				 * R: R[read_queue_id] read G[arch_reg_id] N[target,slot] or
				 * W: W[write_queue_id] write G[arch_reg_id]*/
				if ((i_spec->instruction_format == W_FORM || i_spec->instruction_format == R_FORM) && TOUPPER (*str) != 'G')
				{
					/* error: Invalid instruction. */
				}
				if (TOUPPER (*str) == 'G' && (i_spec->instruction_format == W_FORM || i_spec->instruction_format == R_FORM))
				{
					int x = 0;

					str++;
					if (*str != '[')
					{
						/* error: '[' is missed. */
					}
					str++;
					if (!ISDIGIT(*str))
					{
						/* error: Invalid arch_reg_id. */
					}
					while (ISDIGIT(*str))
					{
						x = 10 * x + (*str - '0');
						str++;
					}
					i_spec->insn_fields->GR = x / 4;
					if (*str != ']')
					{
						/* error: ']' is missed. */
					}
					str++;

					if (*str == '\0' && i_spec->instruction_format == W_FORM)
					{
						break;
					}
				}
				if (*str != ' ')
				{
					/* error: Invalid instruction. */
				}
				while (ISSPACE (*str))
				{
					str++;
				}
				if (i_spec->instruction_format == R_FORM && TOUPPER (*str) == 'W')
				{
					/* error: The R format instruction shouldn't have GPR target. */
				}
				t1_n_spec = trips_find_node (str, &res);
				if (t1_n_spec->ntype != ALU_TYPE && t1_n_spec->ntype != GPR_TYPE)
				{
					/* error: Invalid instruction. */
				}
				i_spec->target_one = t1_n_spec;
				str += res;	
				if (*str != '\0')
				{
					if (!ISSPACE(*str))
					{
						/* error: Invalid instruction. */
					}
					while (ISSPACE(*str))
					{
						str++;
					}
					if (i_spec->instruction_format == R_FORM && TOUPPER (*str == 'W'))
					{
						/* error: The R format instruction shouldn't have GPR target. */
					}
					res = 0;
					t2_n_spec = trips_find_node (str, &res);
					if (t1_n_spec->ntype != ALU_TYPE && t1_n_spec->ntype != GPR_TYPE)
					{
						/* error: Invalid instruction. */
					}
					i_spec->target_two = t2_n_spec;
					str += res;
					if (*str != '\0')
					{
						/* error: Invalid instrucion. */
					}
				}
				break;
			case I_FORM:
			case C_FORM:
				/* I: N[source_node] opcode immediate_value N[target,slot] or
				 * I: N[source_node] opcode N[target,slot] or
				 * C: N[source_node] constant_opcode big_immediate_value N[target,slot]. */
				if (*(str + 1) != '[')
				{
					/* We think we encounter the immediate value. */
					_trips_parse_immediate_field (i_spec, &str);
				}
				if (TOUPPER(*str) != 'N' && TOUPPER(*str) != 'W')
				{
					/* error: Invalid instruction. */
				}
				res = 0;
				t1_n_spec = trips_find_node (str, &res);
				if (t1_n_spec->ntype != ALU_TYPE && t1_n_spec->ntype != GPR_TYPE)
				{
					/* error: Invalid instruction. */
				}
				i_spec->target_one = t1_n_spec;
				str += res;
				if (*str != '\0')
				{
					/* error: Invalid targets list. */
				}
				break;
			case L_FORM:
			case S_FORM:
			case B_FORM:
				if ((TOUPPER(*str) == 'L' || TOUPPER(*str) == 'S' || TOUPPER(*str) == 'B') && *(str + 1) == '[')
				{
					int x = 0;

					str += 2;	/* skip '[' */
					while (ISDIGIT(*str))
					{
						x = 10 * x + (*str - '0');
						str++;
					}
					if (*str != ']')
					{
						/* error: ']' is missed. */
					}
					str++;	/* skip ']' */
					if (i_spec->instruction_format == L_FORM || i_spec->instruction_format == S_FORM)
					{
						i_spec->insn_fields->LSID = x;
					}
					else
					{
						/* i_spec->instruction_format == B_FORM. */
						i_spec->insn_fields->EXIT = x;
					}
				}
				while (ISSPACE (*str))
				{
					str++;
				}
				/* S: N[source_node] store_opcode (S[LSID]) or
				 * B: N[source_node] branch_opcode (B[EXIT_ID]) */
				if (*str == '\0' && (i_spec->instruction_format == S_FORM || i_spec->instruction_format == B_FORM))
				{
					break;
				}

				if (*str && !ISSPACE(*str))
				{
					/* error: there should be spaces between instruction fields. */
				}
				else if (*str == '\0')
				{
					/* error: Invalid instruction. */
					/* L_FORM instruction stops after load_opcode or L[LSID]. */
				}
				else
				{
					while (ISSPACE(*str))
					{
						str++;
					}
				}
				if ((TOUPPER(*str) == 'N' || TOUPPER(*str) == 'W') && *(str + 1) == '[')
				{
					/* L: N[source_node] load_opcode (L[LSID]) N[target,slot] */

					if (i_spec->instruction_format != L_FORM)
					{
						/* error: Invalid instruction format. */
					}
					res = 0;
					t1_n_spec = trips_find_node (str, &res);
					if (t1_n_spec->ntype != ALU_TYPE && t1_n_spec->ntype != GPR_TYPE)
					{
						/* error: Invalid instruction. */
					}
					i_spec->target_one = t1_n_spec;
					str += res;
					if (*str == '\0')
					{
						break;
					}
					else
					{
						/* error: Invalid instruction format. */
					}
				}
				else
				{
					/* L: N[source_node] load_opcode (L[LSID]) immediate_value N[target,slot] or
					 * S: N[source_node] store_opcode (S[LSID]) immediate_value or
					 * B: N[source_node] branch_opcode (B[EXIT_ID]) branch_offset. */

					if (i_spec->instruction_format == L_FORM || i_spec->instruction_format == S_FORM)
					{
						_trips_parse_immediate_field (i_spec, &str);
					}
					else
					{
						_trips_parse_boffset_field (i_spec, &str);
					}

					if (*str == '\0')
					{
						if (i_spec->instruction_format == S_FORM || i_spec->instruction_format == B_FORM)
						{
							break;
						}
						else
						{
							/* error: Invalid instruction format. */
						}
					}
					else if (i_spec->instruction_format == L_FORM)
					{
						if (TOUPPER(*str) != 'N' && TOUPPER(*str) != 'W')
						{
							/* error: Invalid instruction. */
						}
						res = 0;
						t1_n_spec = trips_find_node (str, &res);
						if (t1_n_spec->ntype != ALU_TYPE && t1_n_spec->ntype != GPR_TYPE)
						{
							/* error: Invalid target for L_FORM instruction. */
						}
						i_spec->target_one = t1_n_spec;
						str += res;
						if (*str != '\0')
						{
							/* error: Invalid targets list. */
						}
					}
				}
				break;
			case M3_FORM:
			case M4_FORM:
				res = 0;
				t1_n_spec = trips_find_node(str, &res);
				str += res;
				if (!ISSPACE(*str))
				{
					/* error: Invalid instruction. */
				}
				while (ISSPACE(*str))
				{
					str++;
				}
				res = 0;
				t2_n_spec = trips_find_node(str, &res);
				str += res;
				if (!ISSPACE(*str))
				{
					/* error: Invalid instruction. */
				}
				while (ISSPACE(*str))
				{
					str++;
				}
				res = 0;
				t3_n_spec = trips_find_node(str, &res);
				str += res;
				if (i_spec->instruction_format == M3_FORM)
				{
					if (*str != '\0')
					{
						/* error: Invalid instruction. */
					}
					if ((t1_n_spec && (t1_n_spec->ntype == ALU_TYPE || t1_n_spec->ntype == GPR_TYPE)) &&
					    (t1_n_spec && (t1_n_spec->ntype == ALU_TYPE || t1_n_spec->ntype == GPR_TYPE)) &&
					    (t1_n_spec && (t1_n_spec->ntype == ALU_TYPE || t1_n_spec->ntype == GPR_TYPE)) )
					{
						i_spec->target_one = t1_n_spec;
						i_spec->target_two = t2_n_spec;
						i_spec->target_three = t3_n_spec;
					}
					else
					{
						/* error: Invalid instruction. */
					}
					slot1 = t1_n_spec->slot;
					slot2 = t2_n_spec->slot;
					slot3 = t3_n_spec->slot;
					if (!(slot1 == slot2 && slot2 == slot3))
					{
						/* errro: Invalied instructin. */
					}
					/* i_spec->insn_fields->M3TX = slot1; */
				}
				else
				{
					/* i_spec->instruction_format == M4_FORM. */
					if (!ISSPACE(*str))
					{
						/* error: Invalid instruction. */
					}
					while (ISSPACE(*str))
					{
						str++;
					}

					res = 0;
					t4_n_spec = trips_find_node(str, &res);
					str += res;

					if (*str != '\0')
					{
						/* error: Invalid instruction. */
					}
					if ((t1_n_spec && (t1_n_spec->ntype == ALU_TYPE || t1_n_spec->ntype == GPR_TYPE)) &&
						(t2_n_spec && (t2_n_spec->ntype == ALU_TYPE || t2_n_spec->ntype == GPR_TYPE)) &&
						(t3_n_spec && (t3_n_spec->ntype == ALU_TYPE || t3_n_spec->ntype == GPR_TYPE)) &&
						(t4_n_spec && (t4_n_spec->ntype == ALU_TYPE || t4_n_spec->ntype == GPR_TYPE)) )
					{
						i_spec->target_one = t1_n_spec;
						i_spec->target_two = t2_n_spec;
						i_spec->target_three = t3_n_spec;
						i_spec->target_four = t4_n_spec;
					}
					else
					{
						/* error: Invalid instruction. */
					}

					slot1 = t1_n_spec->slot;
					slot2 = t2_n_spec->slot;
					slot3 = t3_n_spec->slot;
					slot4 = t4_n_spec->slot;

					msb2_1 = (t1_n_spec->flat_name >> M4_TARGET_BITS) & 0x03;
					msb2_2 = (t2_n_spec->flat_name >> M4_TARGET_BITS) & 0x03;
					msb2_3 = (t3_n_spec->flat_name >> M4_TARGET_BITS) & 0x03;
					msb2_4 = (t4_n_spec->flat_name >> M4_TARGET_BITS) & 0x03;
					if (!(slot1 == slot2 && slot2 == slot3 && slot3 == slot4) ||
						!(msb2_1 == msb2_2 && msb2_2 == msb2_3 && msb2_4))
					{
						/* errro: Invalid instruction. */
					}
					/* i_spec->insn_fields->M4TX = (((slot1 << 2) | msb2_1) & 0x0f); */
				}

				break;
		}
	}

	the_word = _trips_build_the_instruction (i_spec, tp);
	i_spec->the_word = the_word;

	if (the_word != BAD_WORD)
	{
		if (i_spec->instruction_format == R_FORM || i_spec->instruction_format == W_FORM)
		{
			_trips_place_read_or_write_instruction_in_block_header (i_spec, tp);
		}
		else
		{
			_trips_place_normal_instruction_in_grid (i_spec, tp);	
		}
	}
	else
	{
		/* error: */
		as_bad (_("This instruction couldn't be assebled. Sorry."));
	}

}

static void _trips_parse_immediate_field (i_spec, pstr)
	struct trips_instruction_specifier *i_spec;
	char **pstr;
{
	if ((**pstr) && ((**pstr == '-' || **pstr == '+' || **pstr == '0' || ISDIGIT(**pstr) || **pstr != '%')))
	{
		/* We think we've got hard-coded immediate value
		 * or it could be a symbol form the .set pseudo-op.*/
		char *orig_line = input_line_pointer;
		expressionS new_exp;

		input_line_pointer = *pstr;
		expression(&new_exp);
		if (new_exp.X_op == O_constant)
		{
			int i;

			i = (int)(new_exp.X_add_number);
			*pstr = input_line_pointer;
			if (i_spec->instruction_format == C_FORM)
			{
				i_spec->insn_fields->CON = i;
			}
			else
			{
				/* i_spec->instruction_format == (I_FORM || L_FORM || S_FORM) */
				i_spec->insn_fields->IMM = i;
			}
			input_line_pointer = orig_line;
		}
		else
		{
			input_line_pointer = orig_line;
			if (i_spec->instruction_format == C_FORM)
			{
				i_spec->insn_fields->CON = UNKNOWN_VALUE;
				as_bad(_("Expecting an constant value but got: '%s'"), *pstr);
			}
			else
			{
				i_spec->insn_fields->IMM = UNKNOWN_VALUE;
				as_bad(_("Expecting an immediate value but got: '%s'"), *pstr);
			}
		}
	}
	else if (**pstr == '%')
	{
		char *orig_line = input_line_pointer;
		trips_parse_field_operator(*pstr, i_spec);
		*pstr = input_line_pointer;
		input_line_pointer = orig_line;
	}
	else
	{
		/* error: Where is the immediate value must needed in this instruction? */
	}
	while (ISSPACE (**pstr))
	{
		(*pstr)++;
	}
}

	static int 
trips_parse_field_operator (str, is)
	char *str;
	trips_instruction_specifier_t * is;
{
	char *s;
	trips_relocator_t * pthe_insn; 
	pthe_insn = malloc(sizeof (trips_relocator_t));
	memset (pthe_insn, '\0', sizeof (trips_relocator_t));
	s = str;
	char *s1;
	char *op_arg = NULL;

	/* Check for %hi, %mid, %lo, %bottom  */
	if (1) {
		static const struct ops {
			/* The name as it appears in assembler.  */
			char *name;
			/* strlen (name), precomputed for speed */
			int len;
			/* The reloc this pseudo-op translates to.  */
			int reloc;
			/* Non-zero if can be used in pc-relative contexts.  */
			int pcrel_p;
		} ops[] = {
			/* midhi must appear before mid so %midhi won't be
			   mistaken for %mid.  */
			{ "hi", 2, BFD_RELOC_TRIPS_HI_13, 0 },
			{ "mid", 3, BFD_RELOC_TRIPS_MID_13, 0 },
			{ "lo", 2, BFD_RELOC_TRIPS_LO_13, 0 },
			{ "bottom", 6, BFD_RELOC_TRIPS_BOTTOM_13, 0 },
			{ NULL, 0, 0, 0 }
		};
		const struct ops *o;

		for (o = ops; o->name; o++)
			if (strncmp (s + 1, o->name, o->len) == 0)
				break;
		if (o->name == NULL)
			return -1;

		if (s[o->len + 1] != '(')
		{
			as_bad (_("Illegal operands: %%%s requires arguments in ()"), o->name);
			return -1;
		}

		op_arg = o->name;
		pthe_insn->reloc = o->reloc;
		pthe_insn->pcrel = o->pcrel_p;
		s += o->len + 2;
	}

	if (op_arg) {
		for (s1 = s; *s1 && *s1 != ')'; s1++) {
			;
		}

		if (*s1 != ')') {
			as_bad (_("Illegal operands: %%%s requires arguments in ()"), op_arg);
			return -1;
		}
		*s1 = '\0';
		(void) get_expression (s, pthe_insn);
		*s1 = ')';
		s = s1 + 1;

		/* we are done for now--carry on */
		is->insn_fields->CON = RELOCATABLE_VALUE;
		input_line_pointer = s;
		is->aux_info = (PTR)pthe_insn;
	}
	return 0;
}

static void _trips_parse_boffset_field (i_spec, pstr)
	struct trips_instruction_specifier *i_spec;
	char **pstr;
{
	char *s;
	trips_relocator_t *pthe_insn;

	pthe_insn = malloc(sizeof (trips_relocator_t));
	memset (pthe_insn, '\0', sizeof (trips_relocator_t));

	(void) get_expression (*pstr, pthe_insn);
	(void) get_expression2 (current_block_specifier->name, pthe_insn);
	pthe_insn->reloc = BFD_RELOC_TRIPS_RELATIVE;
	pthe_insn->pcrel = 1;
	*pstr = expr_end;
	i_spec->aux_info = (PTR)pthe_insn;
	/* Just a temporary value, I will complete this part afterwords.*/
	i_spec->insn_fields->OFFSET = RELOCATABLE_VALUE;
}

static trips_word _trips_build_the_instruction  (is, tp)
	struct trips_instruction_specifier *is;
	trips_processor_obj_t *tp;
{
	trips_word the_word = 0;
	trips_word target1, target2, target3, target4;
	
	/* Order: Left->Right. */
	the_word |= ((is->popcode->match) & NORMAL_OPCODE_MASK);
	switch (is->instruction_format)
	{	
		case G_FORM:
			the_word |= ((is->insn_fields->PR << (XOP_BITS + 2 * NORMAL_TARGET_BITS)) & PR_MASK);
			the_word |= ((is->popcode->match) & XOP_MASK);

			target1 = _trips_get_normal_target_value (is->target_one);
			target2 = _trips_get_normal_target_value (is->target_two);

			if (target2 != NO_TARGET_VALUE)
			{
				the_word |= ((target1 << NORMAL_TARGET_BITS) & NORMAL_T1_MASK);
				the_word |= (target2 & NORMAL_T0_MASK);
			}
			else
			{
				the_word |= (target1 & NORMAL_T0_MASK);
			}
			break;
		case I_FORM:
			the_word |= ((is->insn_fields->PR << (XOP_BITS + IMM_BITS + NORMAL_TARGET_BITS)) & PR_MASK);
			the_word |= ((is->popcode->match) & XOP_MASK);

			target1 = _trips_get_normal_target_value (is->target_one);

			the_word |= ((is->insn_fields->IMM << NORMAL_TARGET_BITS) & NORMAL_T1_MASK);
			the_word |= (target1 & NORMAL_T0_MASK);
			break;
		case L_FORM:
		case S_FORM:
			the_word |= ((is->insn_fields->PR << (LSID_BITS + IMM_BITS + NORMAL_TARGET_BITS)) & PR_MASK);
			the_word |= ((is->insn_fields->LSID << (IMM_BITS + NORMAL_TARGET_BITS)) & LSID_MASK);
			the_word |= ((is->insn_fields->IMM << NORMAL_TARGET_BITS) & IMM_MASK);
			if (is->instruction_format == L_FORM)
			{
				target1 = _trips_get_normal_target_value (is->target_one);
				the_word |= (target1 & NORMAL_T0_MASK);
			}
			break;
		case B_FORM:
			the_word |= ((is->insn_fields->PR << (EXIT_BITS + OFFSET_BITS)) & PR_MASK);
			the_word |= ((is->insn_fields->EXIT << OFFSET_BITS) & EXIT_MASK);
			the_word |= (is->insn_fields->OFFSET & OFFSET_MASK);
			break;
		case C_FORM:
			the_word |= ((is->insn_fields->CON << NORMAL_TARGET_BITS) & CONST_MASK);
			target1 = _trips_get_normal_target_value (is->target_one);
			the_word |= (target1 & NORMAL_T0_MASK);
			break;
		case R_FORM:
			the_word |= ((is->insn_fields->V << (READ_GR_BITS + 2 * READ_TARGET_BITS)) & READ_V_MASK);
			the_word |= ((is->insn_fields->GR << 2 * READ_TARGET_BITS) & READ_GR_MASK);

			target1 = _trips_get_normal_target_value (is->target_one);
			the_word |= ((target1  << READ_TARGET_BITS) & READ_RT0_MASK);
			if (is->target_two != (trips_node_specifier_t *)NULL)
			{
				target2 = _trips_get_normal_target_value (is->target_two);
				the_word |= (target2 & READ_RT1_MASK);
			}
			else
			{
				the_word |= (target1 & READ_RT1_MASK);
			}
			break;
		case W_FORM:
			the_word |= (is->insn_fields->V << WRITE_GR_BITS) & WRITE_V_MASK;
			the_word |= (is->insn_fields->GR & WRITE_GR_MASK);
			break;
		case M3_FORM:
			the_word |= ((is->insn_fields->PR << (M3TX_BITS + 3 * M3_TARGET_BITS)) & PR_MASK);
			target1 = _trips_get_normal_target_value (is->target_one);
			target2 = _trips_get_normal_target_value (is->target_two);
			target3 = _trips_get_normal_target_value (is->target_three);
			is->insn_fields->M3TX = (target1 >> M3_TARGET_BITS) & 0x3;
			the_word |= ((is->insn_fields->M3TX << 3 * M3_TARGET_BITS) & M3TX_MASK);
			the_word |= ((target1 << (2 * M3_TARGET_BITS)) & M3T0_MASK);
			the_word |= ((target2 << M3_TARGET_BITS) & M3T1_MASK);
			the_word |= (target3 & M3T2_MASK);
			break;
		case M4_FORM:
			target1 = _trips_get_normal_target_value (is->target_one);
			target2 = _trips_get_normal_target_value (is->target_two);
			target3 = _trips_get_normal_target_value (is->target_three);
			target4 = _trips_get_normal_target_value (is->target_four);
			is->insn_fields->M4TX = (target1 >> M4_TARGET_BITS) & 0xf;
			the_word |= ((is->insn_fields->M4TX << (4 * M4_TARGET_BITS)) & M4TX_MASK);
			the_word |= ((target1 << 3 * M4_TARGET_BITS) & M4T0_MASK);
			the_word |= ((target2 << 2 * M4_TARGET_BITS) & M4T1_MASK);
			the_word |= ((target3 << M4_TARGET_BITS) & M4T2_MASK);
			the_word |= (target4 & M4T3_MASK);
			break;
		default:
			/* error. */
			break;
	}

	return the_word;
}

static trips_word _trips_get_normal_target_value (n_spec)
	struct trips_node_specifier *n_spec;
{
	trips_word res = 0;

	if (n_spec != NULL)
	{
		if (n_spec->ntype == ALU_TYPE)
		{
			if (n_spec->slot == '0')
			{
				res |= (OP0_SLOT_VALUE << NORMAL_INST_ID_BITS); 
			}
			else if (n_spec->slot == '1')
			{
				res |= (OP1_SLOT_VALUE << NORMAL_INST_ID_BITS);
			}
			else
			{
				res |= (PREDICATE_SLOT_VALUE << NORMAL_INST_ID_BITS);
			}
			res |= n_spec->flat_name;
		}
		else
		{
			res |= ((WRITE_SLOT_VALUE << WRITE_ID_BITS) | n_spec->flat_name);
		}
	}
	else
	{
		res = NO_TARGET_VALUE;
	}

	return res;
}

static void _trips_place_read_or_write_instruction_in_block_header (is, tp)
	struct trips_instruction_specifier *is;
	struct trips_processor_obj *tp;
{
	if (is->instruction_format == R_FORM)
	{
		tp->block_header[is->source_node->flat_name].read_insn = is->the_word;
	}
	else
	{
		tp->block_header[is->source_node->flat_name].write_insn = is->the_word;
	}
}

static void _trips_place_normal_instruction_in_grid (is, tp)
	struct trips_instruction_specifier *is;
	struct trips_processor_obj *tp;
{
	int r, c, f, abs;
	struct trips_node_specifier *incoming_source_node = is->source_node;

	abs = incoming_source_node->flat_name;
	/* 
	 * Key mapping functions: Given a flat name, produce the
	 * row/colum/frame coordinates.
	 *
	 * For example, assume we have:
	 *		111		11	11
	 *		frame	row	col
	 *
	 * Then we would want:
	 * f = (abs >> 4) & 0x7;
	 * r = (abs >> 2) & 0x3;
	 * c = abs & 0x3;
	 * */
	f = (abs >> (R_BITS + C_BITS)) & F_MASK;
	r = (abs >> C_BITS) & R_MASK;
	c = abs & C_MASK;

	if (abs > current_block_specifier->max_alu_flat_name)
	{
		current_block_specifier->max_alu_flat_name = abs;
	}

	if (tp->grid[f][r][c] != 0)
	{
		as_warn (_("Not allowed to schedule twice on the same node: N[%d]"), incoming_source_node->flat_name);
	}
	
	tp->grid[f][r][c] = is->the_word;
	if (is->insn_fields->IMM == (trips_word)RELOCATABLE_VALUE || is->insn_fields->OFFSET == (trips_word)RELOCATABLE_VALUE 
			|| is->insn_fields->CON == (trips_word)RELOCATABLE_VALUE)
	{
		fixup_grid[f][r][c] = is;
	}
}


/* Subroutine of trips_parse_field_operator to parse an expression.  */

static int
get_expression (str, tr)
  char *str;
  trips_relocator_t * tr;
{
  char *save_in;
  segT seg;

  save_in = input_line_pointer;
  input_line_pointer = str;
  seg = expression (&(tr->exp));
  if (seg != absolute_section
      && seg != text_section
      && seg != data_section
      && seg != bss_section
      && seg != undefined_section)
    {
      tr->error = _("bad segment");
      expr_end = input_line_pointer;
      input_line_pointer = save_in;
      return 1;
    }
  expr_end = input_line_pointer;
  input_line_pointer = save_in;
  return 0;
}

static int
get_expression2 (str, tr)
	char *str;
	trips_relocator_t *tr;
{
  char *save_in;
  segT seg;

  save_in = input_line_pointer;
  input_line_pointer = str;
  seg = expression (&(tr->exp2));
  if (seg != absolute_section
      && seg != text_section
      && seg != data_section
      && seg != bss_section
      && seg != undefined_section)
    {
      tr->error = _("bad segment");
      expr_end = input_line_pointer;
      input_line_pointer = save_in;
      return 1;
    }
  expr_end = input_line_pointer;
  input_line_pointer = save_in;
  return 0;

}



#ifdef OBJ_ELF
/* For ELF, add support for SHF_EXCLUDE and SHT_ORDERED.  */

int
trips_section_letter (letter, ptr_msg)
     int letter;
     char **ptr_msg;
{
  if (letter == 'e')
    return SHF_EXCLUDE;

  *ptr_msg = _("Bad .section directive: want a,e,w,x,M,S in string");
  return 0;
}

int
trips_section_word (str, len)
     char *str;
     size_t len;
{
  if (len == 7 && strncmp (str, "exclude", 7) == 0)
    return SHF_EXCLUDE;

  return -1;
}

int
trips_section_type (str, len)
     char *str;
     size_t len;
{
  if (len == 7 && strncmp (str, "ordered", 7) == 0)
    return SHT_ORDERED;

  return -1;
}

int
trips_section_flags (flags, attr, type)
     int flags;
     int attr;
     int type;
{
  if (type == SHT_ORDERED)
    flags |= SEC_ALLOC | SEC_LOAD | SEC_SORT_ENTRIES;

  if (attr & SHF_EXCLUDE)
    flags |= SEC_EXCLUDE;

  return flags;
}
#endif /* OBJ_ELF */


/* Pseudo-op handling.  */

/* The .byte pseudo-op.  This is similar to the normal .byte
   pseudo-op, but it can also take a single ASCII string.  */

static void
trips_byte (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  if (*input_line_pointer != '\"')
    {
      cons (1);
      return;
    }

  /* Gather characters.  A real double quote is doubled.  Unusual
     characters are not permitted.  */
  ++input_line_pointer;
  while (1)
    {
      char c;

      c = *input_line_pointer++;

      if (c == '\"')
        {
          if (*input_line_pointer != '\"')
            break;
          ++input_line_pointer;
        }

      FRAG_APPEND_1_CHAR (c);
    }

  demand_empty_rest_of_line ();
}

/* Turn a string in input_line_pointer into a floating point constant
   of type TYPE, and store the appropriate bytes in *LITP.  The number
   of LITTLENUMS emitted is stored in *SIZEP.  An error message is
   returned, or NULL on OK.  */

char *
md_atof (type, litp, sizep)
     int type;
     char *litp;
     int *sizep;
{
  int prec;
  LITTLENUM_TYPE words[4];
  char *t;
  int i;

  switch (type)
    {
    case 'f':
      prec = 2;
      break;

    case 'd':
      prec = 4;
      break;

    default:
      *sizep = 0;
      return _("bad call to md_atof");
    }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
    input_line_pointer = t;

  *sizep = prec * 2;

  if (target_big_endian)
    {
      for (i = 0; i < prec; i++)
        {
          md_number_to_chars (litp, (valueT) words[i], 2);
          litp += 2;
        }
    }
  else
    {
      for (i = prec - 1; i >= 0; i--)
        {
          md_number_to_chars (litp, (valueT) words[i], 2);
          litp += 2;
        }
    }

  return NULL;
}

/* Write a value out to the object file, using the appropriate
   endianness.  */

void
md_number_to_chars (buf, val, n)
     char *buf;
     valueT val;
     int n;
{
    number_to_chars_bigendian (buf, val, n);
}

/* Align a section (I don't know why this is machine dependent).  */

valueT
md_section_align (seg, addr)
     asection *seg;
     valueT addr;
{
  int align = bfd_get_section_alignment (stdoutput, seg);

  return ((addr + (1 << align) - 1) & (-1 << align));
}

/* We don't have any form of relaxing.  */

int
md_estimate_size_before_relax (fragp, seg)
     fragS *fragp ATTRIBUTE_UNUSED;
     asection *seg ATTRIBUTE_UNUSED;
{
  abort ();
  return 0;
}

/* Convert a machine dependent frag.  We never generate these.  */

void
md_convert_frag (abfd, sec, fragp)
     bfd *abfd ATTRIBUTE_UNUSED;
     asection *sec ATTRIBUTE_UNUSED;
     fragS *fragp ATTRIBUTE_UNUSED;
{
  abort ();
}

/* We have no need to default values of symbols.  */

symbolS *
md_undefined_symbol (name)
     char *name ATTRIBUTE_UNUSED;
{
  return 0;
}

/* Functions concerning relocs.  */

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc. On the trips, PC-relative offsets are
   relative to the address of current block, the address of the block
   name (symbol), but as for add_number in the phase of Assembler(tas), 
   pcrel_from_section = addr(cnt_block), because add_number = 
   addr(cnt_insn) - pcrel_from_section. */

long
md_pcrel_from_section (fixp, sec)
     fixS *fixp;
     segT sec ATTRIBUTE_UNUSED;
{
  return fixp->fx_subsy->bsym->value;
}


#ifdef OBJ_ELF
int
trips_fix_adjustable (fix)
     fixS *fix;
{
  return (fix->fx_r_type != BFD_RELOC_16_GOTOFF
          && fix->fx_r_type != BFD_RELOC_LO16_GOTOFF
          && fix->fx_r_type != BFD_RELOC_HI16_GOTOFF
          && fix->fx_r_type != BFD_RELOC_HI16_S_GOTOFF
          && fix->fx_r_type != BFD_RELOC_GPREL16
          && fix->fx_r_type != BFD_RELOC_VTABLE_INHERIT
          && fix->fx_r_type != BFD_RELOC_VTABLE_ENTRY
          && ! S_IS_EXTERNAL (fix->fx_addsy)
          && ! S_IS_WEAK (fix->fx_addsy)
          && (fix->fx_pcrel
              || (fix->fx_subsy != NULL
                  && (S_GET_SEGMENT (fix->fx_subsy)
                      == S_GET_SEGMENT (fix->fx_addsy)))
              || S_IS_LOCAL (fix->fx_addsy)));
}
#endif

/* Apply a fixup to the object code.  This is called for all the
   fixups we generated by the call to fix_new_exp, above.  In the call
   above we used a reloc code which was the largest legal reloc code
   plus the operand index.  Here we undo that to recover the operand
   index.  At this point all symbol values should be fully resolved,
   and we attempt to completely resolve the reloc.  If we can not do
   that, we determine the correct reloc code and put it back in the
   fixup.  */

void
md_apply_fix3 (fixP, valP, seg)
     fixS *fixP;
     valueT *valP;
     segT seg ATTRIBUTE_UNUSED;
{
  valueT value = *valP;  /* if BFD64, this will be 8 bytes; */
                         /* else limited to 32-bits (4 bytes). */
  int val = 0;
  trips_word insn;
  char *where;
  bfd_reloc_code_real_type r_type = fixP->fx_r_type;

  /* Fetch the instruction, insert the fully resolved operand
     value, and stuff the instruction back again.  */

  where = fixP->fx_frag->fr_literal + fixP->fx_where;
  fixP->fx_addnumber = value;

#ifdef FAT_BITS 
  insn = bfd_getb64 ((unsigned char *) where);
#else /* FAT_BITS */
  insn = bfd_getb32 ((unsigned char *) where);
#endif /* FAT_BITS */
#ifdef BFD64
  fixP->fx_no_overflow = 1;
#endif
  /* Mask off all but the 13 bits we care about. */
  switch (fixP->fx_r_type) {
	  case BFD_RELOC_TRIPS_HI_13:
	  case BFD_RELOC_TRIPS_MID_13:
	  case BFD_RELOC_TRIPS_LO_13:
	  case BFD_RELOC_TRIPS_BOTTOM_13:
		  val = (0x1ce << 9);	/* I haven't known why tas uses 0x1ce as stuff so far... */
		  break;
	  case BFD_RELOC_TRIPS_RELATIVE:
		  val = fixP->fx_addnumber;
		  break;
	  default:
		  break;
  }
  if (fixP->fx_r_type == BFD_RELOC_TRIPS_RELATIVE)
  {
	  /* zero the old OFFSET field */
	  insn &= (0xfff00000);
	  /* "or in" the resolved OFFSET value */
	  insn |= (val & 0xfffff);
  }
  else if (r_type == BFD_RELOC_TRIPS_HI_13 || r_type == BFD_RELOC_TRIPS_MID_13 ||
		   r_type == BFD_RELOC_TRIPS_LO_13 || r_type == BFD_RELOC_TRIPS_BOTTOM_13)
  {
	  /* zero the old CONSTANT field. */
	  insn &= (0xfe0001ff);
	  /* "or in" the resolved CONSTANT value */
	  insn |= (val & 0x01fffe00);
  }
#ifdef FAT_BITS 
  bfd_putb64 ((bfd_vma) insn, (unsigned char *) where);
#else /* FAT_BITS */
  bfd_putb32 ((bfd_vma) insn, (unsigned char *) where);
#endif /* FAT_BITS */
  /* ok -- go home */
//  fixP->fx_done = 1;
}

arelent *
tc_gen_reloc (seg, fixp)
     asection *seg ATTRIBUTE_UNUSED;
     fixS *fixp;
{
/* Generate a reloc for a fixup.  */
  arelent *reloc;

  reloc = (arelent *) xmalloc (sizeof (arelent));

  reloc->sym_ptr_ptr = (asymbol **) xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;
  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);
  if (reloc->howto == (reloc_howto_type *) NULL)
    {
     /*  as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("reloc %d not supported by object file format"),
                    (int) fixp->fx_r_type); */
      return NULL;
    }
  reloc->addend = fixp->fx_addnumber;

  return reloc;
}
/************************************************************
        Parse an ALU specifier.

        We expect the incoming string to lead with
        a digit, a plus (+), or a minus (-).  We are looking for 
        a string of the form: "d,d,d[,d]", where the last
        digit is optional.

        Each of the digits must be a single digit.

        Currently, only the 'x' specifier (first) can be negative.

        Currently, 'y' must be > 0, translating into download 
        movement on the frame.

        'z' must be 0 (default--stays in same frame) or positive
        (goes to the frame below)

        We transform into the appropriate target value.

        Return the length of the string.
*/
static int 
_trips_capture_specific_alu_node (s, alu_spec)
	char *s;
	struct trips_node_specifier * alu_spec; 
{
	int x = 0;
	int res = 3;  /* we have already stripped the initial "N[", and we expect to strip the ']', so we count res = 3.*/
	char slot_char;

	alu_spec->parse_status = UNPARSED;
	while (ISSPACE (*s)) 
	{
		s++; res++;
	}
	if (ISDIGIT(*s)) 
	{
		x = 0;
		while (ISDIGIT (*s)) 
		{
			x = (x * 10) + (*s - '0');
			s++; res++;
		}
		if (x > tp->x_config * tp->y_config * tp->pframes_per_aframe)
		{
			/* error: need to use a parse status macro to define
			 *  this kind of error.
			 */
			return res;
		}
		alu_spec->flat_name = x;
		while (ISSPACE (*s))
		{
			s++; res++;
		}
	}
	if (*s == ',') 
	{
		s++; res++;
		while (ISSPACE (*s)) 
		{
			s++; res++;
		}
		if (ISDIGIT(*s)) 
		{
			slot_char = *s;
			if (slot_char != '0' && slot_char != '1') 
			{
				alu_spec->parse_status = INVALID_SLOT_NUMBER;
				return res;
			}
			else
			{
				alu_spec->slot = slot_char;
			}
		}
		else if (TOUPPER(*s) == 'P')
		{
			alu_spec->slot = 'P';
		}
		else
		{
			alu_spec->parse_status = INVALID_SLOT_NUMBER;
			return res;
		}
		s++; res++;
	}
	else
	{
		/* no explicit slot infomation, use the default slot value. */
		alu_spec->slot = '0';
	}
	while (ISSPACE(*s)) 
	{
		s++; res++;
	}
	/* needed! */
	if (*s != ']')
	{
		alu_spec->parse_status = NODE_DELIMETER_LOST;
		return res;
	}
	alu_spec->parse_status = PARSED_OK;
	alu_spec->ntype = ALU_TYPE;

	return res;
}
/********************************
    _trips_capture_any_node (str, n_spec)


    This routine is the frontend both to register and node
    syntax.

    We expect a string of the form "R[<b>]", where <b> is
    any digit string. We set the value of register as a result,
    and return the length of the string to the read.c calling
    routine for that routine to update the input parse string.

    If the incoming string is of the form "N[<n>]", we call
    the _trips_capture_specific_alu_node() routine to handle
    the parse.

    This routine has a lot of vestigal code from when the
    syntax was quite different; for example,

    * ALUs were delimited with brackets ([]).

    * Both ALUs and registers could include comma-separated
      specifiers for grid position and slot or read and write 
	  queue index.

*/
static int _trips_capture_any_node (char *str, struct trips_node_specifier *n_spec)
{
	char * s;
	int rregister = -1;
	int res = 0; 

	char lchar;

	for (s = str; *s != '\0' && (*s != ',') && ! ISSPACE (*s); s++) 
	{
		if ((TOUPPER(*s) == 'N') || (TOUPPER(*s) == 'R') || (TOUPPER(*s)) == 'W') 
		{
			lchar = TOUPPER(*s);
			s++; res++;
			if ((*s == '[') && ((lchar == 'R') || (lchar == 'W'))) 
			{
				n_spec->ntype = GPR_TYPE;
				if (lchar == 'W')
				{
					n_spec->slot = 'W';
				}
				/* Now get the register info */
				s++; res++;
				while (ISSPACE (*s))
				{
					s++; res++;
				}
				if (ISDIGIT(*s)) 
				{
					rregister = (int)(*s - '0');
					s++; res++;
					while (ISDIGIT (*s)) 
					{
						rregister = (rregister * 10) + (*s - '0');
						s++; res++;
					}
					if (rregister > MAX_READ_AND_WRITE_QUEUE_ID) 
					{
						/* MAX_READ_AND_QUEUE_ID is defined in trips.h. */
						n_spec->ntype = INVALID_TYPE;
						return res;
					}
					while (ISSPACE (*s)) 
					{
						s++; res++;
					}
					if (*s == ']') 
					{
						n_spec->flat_name = rregister;
						s++; res++; 
						return res;
					}
					else 
					{
						n_spec->ntype = INVALID_TYPE;
						return res;
					}
					n_spec->parse_status = PARSED_OK;
				}
				else 
				{
					n_spec->ntype = INVALID_TYPE;
					return res;
				}
			}
			/* Must be an ALU specifier */
			else if ((*s == '[') && (lchar == 'N')) 
			{
				res = _trips_capture_specific_alu_node ((s + 1), n_spec);
				if (n_spec->ntype != ALU_TYPE)
				{
					n_spec->ntype = INVALID_TYPE;
				}
				else 
				{
					n_spec->ntype = ALU_TYPE;
				}
				return res; /* all accounting is complete */
			}
			else 
			{
				n_spec->ntype = INVALID_TYPE;
				return res; 
			}
		}
	}
	while (*s != '\0' && *s != ',' && *s != ')' && *s != ']')
	{
		s++; res++;
	}

	return res;
}
	



static void
_trips_close_block_specifier (bs)
struct trips_block_specifier * bs;
{
	int k;
	enum ok_type checker;

	if (! bs) {
		as_warn (_("Would like to, but cannot close block."));
		return;
	}
	else {
		if (bs->valid_bend == false) {
			as_warn (_("You never officially ended the block named `%s'."),
					bs->name);
		}


		k = current_block_specifier->max_alu_flat_name + 1;
		current_block_specifier->frames_in_need = ((k % 32 == 0) ? (k / 32) : (k / 32 + 1)) * 2;

		_trips_flush_registers(tp);

		_trips_flush_grid(tp);

		if (tp->sequence_numbers) {
			// checker = _trips_check_sequence_number_table(NULL);
			// _trips_make_sequence_number_table(tp);
		}
		/* now destroy the thing */
		xfree(bs->name);
		xfree(bs);
		bs=NULL;
	}
}

static struct trips_block_specifier *
_trips_make_block_specifier (name, sym)
  char *name;
  symbolS *sym;
{
    struct trips_block_specifier * p; 
    char * n;
    if (current_block_specifier) {
      _trips_close_block_specifier (current_block_specifier);
      /* 
        Have to zero the thing ourselves because it's a static.
      */
      current_block_specifier = NULL;
    }
    p = xmalloc (sizeof (struct trips_block_specifier)); 
    if (strlen(name)) {
      n = xmalloc (strlen(name) + 1);
      strcpy (n, name);
      p->name = n;
    }
    else {
      as_warn (_("You ought to name this block--defaulting to `%s'."),UNSPECIFIED_BLOCK_NAME_STRING);
      n = xmalloc (strlen(UNSPECIFIED_BLOCK_NAME_STRING) + 1);
      strcpy (n, UNSPECIFIED_BLOCK_NAME_STRING);
      p->name = n;
      /* 
        This next code is called during a normal, user-initiated
        ".bbegin" pseudo-op.
      */
      symbolS *ext_sym;
      ext_sym = symbol_find_or_make (p->name);
      /* 
         Note that there can be only ONE block at a time that's
         actively being worked on.
      */
      if (ext_sym) {
        S_SET_SEGMENT (ext_sym, text_section);
        symbol_set_frag (ext_sym, frag_now);
        S_SET_EXTERNAL (ext_sym);
        S_SET_VALUE (ext_sym, frag_now_fix ());
      }
      visited_register_block = 0;
    }
    p->size              = 0;
    p->sym               = sym;
    p->valid_bend        = false;
    p->frame_initialized = false;
	p->max_alu_flat_name = -1;
	p->frames_in_need = 0;
    // debug("%s\n", DOUBLE_DASH);
    _trips_make_fixup_grid();
    return p;
}
/*
   This routine is called from read.c when
   we first hit a node specifier in the line.
*/
struct trips_node_specifier * 
trips_find_node (str, s_length) 
	char *str;
	int * s_length;
{
	int res = 0;
	struct trips_node_specifier * n_spec = NULL;

	n_spec = _trips_make_node_specifier (tp);
	res = _trips_capture_any_node (str, n_spec);
	if (res)
	{
		*s_length = res;
		switch (n_spec->ntype) 
		{
			case GPR_TYPE:
				break;
			case ALU_TYPE:
				break;
			case INVALID_TYPE:
				/* n_spec->nfunction = INVALID_FUNCTION; */
				_trips_deal_with_invalid_node(str, n_spec);
				return NULL;
				break;
			default:
				_trips_deal_with_invalid_node(str, n_spec);
				return NULL;
				break;
		}        
	}

	return n_spec;  /* a custom piece of memory space,  
					   assuming we've reached this point */
}



void
trips_cleanup () {
  /* 
    trips_cleanup() is called after the assembler has finished parsing
    the input file.
  */
  if (last_completed_node && current_block_specifier) {
    _trips_close_block_specifier (current_block_specifier);
    /* 
        Have to zero the thing ourselves because it's a static.
    */
    current_block_specifier = NULL;
    return;
  }
  /* else, we haven't gotten a file or block or word to process */
  return;
}
/*
  NOTE:  Not only does this routine print an assembler warning
  but it frees the instruction specifier!  Potentially dangerous:
  Call only when you are in a position to discard the instruction.
*/
static void
_trips_deal_with_invalid_instruction (str, i_spec)
    char * str;
    struct trips_instruction_specifier * i_spec;
{
    if (str) {
        as_warn (_("%s"), str);
     }
     else {
        as_warn (_("Sorry--invalid instruction"));
     }
    _trips_free_instruction_specifier (i_spec, tp);
    return;
}

static void
_trips_deal_with_invalid_node (str, n_spec)
    char * str;
    struct trips_node_specifier * n_spec;
{
  char emess[100];
  /* NOTE--therefore--maximum error message is 100 bytes */
  strcpy(emess,"Unknown error.");
  switch (n_spec->parse_status) {
    case ALU_NAME_TOO_BIG:
      strcpy(emess,"The node specifier is too big.");
      break;
    case SOURCE_NODE_DISALLOWS_RS_SPEC:
      strcpy(emess,"You're not allowed to include a reservation station specifier in an ALU name used as a source.");
      break;
    case ROW_TOO_BIG:
      strcpy(emess,"The row specifier is too big.");
      break;
    case ALU_NAME_NOT_A_DIGIT:
      strcpy(emess,"You must use name ALUs with digits, 0-9.");
      break;
    case NO_OFFSETS_FOR_ABSOLUTE:
      strcpy(emess,"You can't use a +/- in an absolute name.");
      break;
    case ROW_TOO_BIG_X_BITS:
      strcpy(emess,"The row specifier is too big for the number of 'x' bits.");
      break;
    case ROW_NEGATIVE:
      strcpy(emess,"The row specifier is negative.");
      break;
    case ROW_NOT_A_DIGIT:
      strcpy(emess,"The row specifier is not a digit.");
      break;
    case COLUMN_TOO_BIG:
      strcpy(emess,"The column specifier is too big.");
      break;
    case COLUMN_TOO_BIG_RELATIVE:
      strcpy(emess,"The column specifier is too big for the number of 'y' bits.");
      break;
    case COLUMN_NEGATIVE:
      strcpy(emess,"The column specifier is negative.");
      break;
    case COLUMN_NOT_A_DIGIT:
      strcpy(emess,"The column specifier is not a digit.");
      break;
    case FRAME_TOO_BIG:
      strcpy(emess,"The frame or z-specifier is too big.");
      break;
    case FRAME_NEGATIVE:
      strcpy(emess,"The frame or z-specifier is negative.");
      break;
    case FRAME_NOT_A_DIGIT:
      strcpy(emess,"The frame or z-specifier is not a digit.");
      break;
    case BANK_TOO_BIG:
      strcpy(emess,"The bank specifier is too big.");
      break;
    case RELATIVE_SPEC_FOR_SOURCE_NODE:
      strcpy(emess,"You are specifying a source node as a relative entity. ");
      break;
    case BANK_NOT_A_DIGIT:
      strcpy(emess,"The bank specifier is invalid--must be '0' or '1'.");
      break;
    case MISSING_BANK_SPECIFIER:
      strcpy(emess,"You're missing a bank specifier--should be '0' or '1'.");
      break;
    case BAD_NODE_CODE:
      break;
    case BAD_NODE_START:
      strcpy(emess,"The node has incorrect syntax.");
      break;
    case GPR_BANK_TOO_BIG:
      strcpy(emess,"The register bank specifier is too big.");
      break;
    case GPR_ENTRY_TOO_BIG:
      strcpy(emess,"The register entry is too big.");
      break;
    case GPR_BANK_NOT_A_DIGIT:
      strcpy(emess,"The register bank specifier is not a digit.");
      break;
    case GPR_ENTRY_NOT_A_DIGIT:
      strcpy(emess,"The register entry is not a digit.");
      break;
      case GPR_SPECIFIER_INVALID:
      strcpy(emess,"The register entry is invalid.");
      break;
    default:
      break;
  }
  as_bad (_("%s is an invalid node:\n\t%s"), str, emess);
  _trips_free_node_specifier(n_spec, tp);
  return;
}

static void _trips_pseudo_op_sequence (ignore)
	int ignore ATTRIBUTE_UNUSED;
{
}

static void
_trips_pseudo_op_block_begin (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  char *name;
  char *s;
  char endc;
  symbolS *ext_sym;
  name = xmalloc(strlen(input_line_pointer) + 1);
  strcpy (name, input_line_pointer);
  s = input_line_pointer;
  int i = 0;
  while ((s[i] != '\0') && (s[i] != ' ') && (s[i] != '\n')) {
        name[i] = s[i];
        i++;
  }
  name[i] = '\0';
  endc = get_symbol_end ();
  *input_line_pointer = endc;

  demand_empty_rest_of_line ();
  /* 
     Following check is for sanity purposes only.  We should
     always make a new block upon a .bblock macro.
  */
  ext_sym = symbol_find_or_make (name);
  /* 
     Note that there can be only ONE block at a time that's
     actively being worked on.
  */
  if (ext_sym) {
      S_SET_SEGMENT (ext_sym, text_section);
      symbol_set_frag (ext_sym, frag_now);
      /* S_SET_EXTERNAL (ext_sym); -- No more -- 04/09/03
         By default instruction blocks are locally scoped.  */
      S_SET_VALUE (ext_sym, frag_now_fix ());
  }
  current_block_specifier = _trips_make_block_specifier(name, ext_sym);
  _trips_make_registers (tp); /* freshen the block header for the comming block. */
  /* visited_register_block = 0; */
  xfree (name);
  name=NULL;
}

static void
_trips_pseudo_op_block_end (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  char *name;
  char *s;
  char endc;
  int name_supplied = 0;
  if ((input_line_pointer) && (*input_line_pointer != '\n')) {
    name = xmalloc(strlen(input_line_pointer) + 1);
    strcpy (name, input_line_pointer);
    s = input_line_pointer;
    int i = 0;
    while ((s[i] != '\0') && (s[i] != ' ') && (s[i] != '\n')) {
        name[i] = s[i];
        i++;
    }
    name[i] = '\0';
    if (strlen(name)) {
      name_supplied = 1;
      endc = get_symbol_end ();
      *input_line_pointer = endc;
    }
  }
  if (current_block_specifier) {
    if (name_supplied) {
      if (strcmp (name, current_block_specifier->name)) {
        as_warn (_("What's the deal?  This ending block name, `%s', is\n\tdifferent from the one you've been scheduling, `%s'."),
        name, current_block_specifier->name);
      } 
    }
    current_block_specifier->valid_bend = true;
    /* close off the current block */
    _trips_close_block_specifier (current_block_specifier);
  }
  /* 
      Have to zero the thing ourselves because it's a static.
  */
  current_block_specifier = NULL;
  demand_empty_rest_of_line ();
}

static void
_trips_pseudo_op_extract (ignore)
     int ignore ATTRIBUTE_UNUSED;
{
  char *name;
  char *s;
  char endc;
  if ((input_line_pointer) && (*input_line_pointer != '\n')) {
    name = xmalloc(strlen(input_line_pointer) + 1);
    strcpy (name, input_line_pointer);
    s = input_line_pointer;
    int i = 0;
    while ((s[i] != '\0') && (s[i] != ' ') && (s[i] != '\n')) {
        name[i] = s[i];
        i++;
    }
    name[i] = '\0';
    if (strlen(name)) {
      endc = get_symbol_end ();
      *input_line_pointer = endc;
    }
  }
  demand_empty_rest_of_line ();
}

void 
trips_after_parse_args ()
{
  /* NOTE: The bfd structure is not yet initialized */
}


static void
_trips_flush_grid (tp)
struct trips_processor_obj * tp;
{
  trips_instruction_specifier_t * is;
  int r, c, f;
  char *frag;

  for (f = 0; f < current_block_specifier->frames_in_need; f++)
  {
	  for (r = 0; r < tp->x_config; r++)
	  {
		  for (c = 0; c < tp->y_config; c++) 
		  {
			  if (fixup_grid[f][r][c])
			  {
				  trips_set_fix(fixup_grid[f][r][c], 4);
			  }
			  else
			  {
				  frag = frag_more (4); /* bytes per word  */
				  md_number_to_chars (frag, tp->grid[f][r][c], 4); 
			  }
		  }
	  }
  }
  _trips_make_grid(tp);  /* use the global processor object */
  visited_register_block  = 0;
}

static void
_trips_flush_registers (tp)
struct trips_processor_obj * tp;
{
	int i, store_flag;
	char *f;

	tp->block_header[0].header_nibble = tp->block_header[1].header_nibble = 0xf;
	tp->block_header[2].header_nibble = 0x0;
	tp->block_header[3].header_nibble = current_block_specifier->frames_in_need / 2;
	tp->block_header[4].header_nibble = tp->block_header[5].header_nibble = 0x0;
	for (i = 6; i < 16; ++i)
	{
		tp->block_header[i].header_nibble = 0;
	}
	//tp->block_header[7].header_nibble = 0x1;
	store_flag = tp->aframe_stats->store_flag;
	for (i = 16; i < 24; ++i) 
	{
		tp->block_header[i].header_nibble = (store_flag & 0x0f);
		store_flag >> 4;
	}
	for (i = 24; i < 32; ++i)
	{
		tp->block_header[i].header_nibble = 0;
	}

	for (i = 0; i < HEADER_CHUNK_SIZE; ++i)
	{
		tp->block_header_words[i] |= (tp->block_header[i].write_insn & WRITE_INSN_MASK);
		tp->block_header_words[i] |= ((tp->block_header[i].read_insn << WRITE_INSN_BITS) & READ_INSN_MASK);
		tp->block_header_words[i] |= ((tp->block_header[i].header_nibble << (READ_INSN_BITS + WRITE_INSN_BITS)) & HEADER_NIBBLE_MASK);

		f = frag_more(4);
		md_number_to_chars(f, tp->block_header_words[i], 4);
	}
}

static enum ok_type 
_trips_check_operand_range (is, tp)
  trips_instruction_specifier_t *is;
  trips_processor_obj_t *tp;
{
}

static void
_trips_make_fixup_grid() 
{
   int r, c, f;
   for (r=0; r < MAX_NUM_ROWS_IN_GRID; r++) {
     for (c=0; c < MAX_NUM_COLS_IN_GRID; c++) {
       for (f=0; f < MAX_NUM_PFRAMES_IN_GRID; f++) {
             fixup_grid[r][c][f] = NULL;
       }
     }
   }
}

static void
trips_set_fix(is, num_bytes) 
  trips_instruction_specifier_t * is;
  int num_bytes;
{
  trips_relocator_t * sinsn = (trips_relocator_t *)is->aux_info;

  char *toP = frag_more (num_bytes);

  offsetT offset;
  symbolS *add = sinsn->exp.X_add_symbol;
  symbolS *sub = current_block_specifier->sym;

  md_number_to_chars(toP, is->the_word, num_bytes);

  if (sinsn->reloc != BFD_RELOC_NONE) {

      fix_new_exp (frag_now,  /* which frag.  */
         (toP - frag_now->fr_literal),  /* where.  */
         num_bytes,    /* size.  */
         &sinsn->exp,  /* the thingy bop that needs fixing */
		 &sinsn->exp2,	/* exp2 contains the current_block_specifier->symbol */
         sinsn->pcrel, /* sinsn->pcrel */
         sinsn->reloc); /* the callback ID for the type of relocation */
  }
  return;
}
