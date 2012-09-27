/* trips.h -- Header file for Trips opcode table

    --> I am binutils/include/opcode/trips.h <--
    --> See also binutils/include/opcode/tcconfig.h <--
    --> See also binutils/include/elf/trips.h <--

Most of the include/elf/trips.h contents migrated here to opcode/trips.h
on Fri Sep 13 09:15:39 CDT 2002.  Remember Alligator Alley!
*/

#ifndef TRIPS_HEADER
#define TRIPS_HEADER
/* 
  Following are defined in libiberty/xmalloc.c for
  tracking memory usage.
*/
extern int trips_memory_call_counter;
extern int trips_memory_byte_counter;

typedef unsigned int trips_word; /* 32-bit on Linux */
#include "dis-asm.h"

enum ok_type
{
	OK,
	NOT_OK
};

#define HEADER_CHUNK_SIZE 32
#define INSTRUCTION_CHUNK_SIZE 32

#define MAX_READ_AND_WRITE_QUEUE_ID 31

#define MAX_NUM_ROWS_IN_GRID 8
#define MAX_NUM_COLS_IN_GRID 8
#define MAX_NUM_PFRAMES_IN_GRID 128

#define UNSPECIFIED_POSITIN 0

/* for immediate. */
#define UNKNOWN_VALUE -999999
#define RELOCATABLE_VALUE -999998

/* for sequence number of lack thereof. */
#define UNKNOWN_SEQUENCE_NUMBER 0x601d
#define BLANK_SEQUENCE_NUMBER 0xbade
#define UNUSED_SEQUENCE_NUMBER 0xbebe
#define IN_USE_SEQUENCE_NUMBER 1

#define UNKOWN_SEQUENCE_NUMBER 0x601d
#define BLANK_SEQUENCE_NUMBER 0xbade

#define NORMAL_OPCODE_MASK 0xfe000000
#define PR_MASK 0x01800000
#define NORMAL_T0_MASK 0x000001ff
#define NORMAL_T1_MASK 0x0003fe00
#define XOP_MASK 0x007c0000
#define IMM_MASK NORMAL_T1_MASK
#define LSID_MASK XOP_MASK
#define EXIT_MASK 0x00700000
#define CONST_MASK 0x01fffe00
#define OFFSET_MASK	0xfffff
#define M3TX_MASK 0x00600000
#define M3T0_MASK 0x001fc000
#define M3T1_MASK 0x00003f80
#define M3T2_MASK 0x0000007f
#define M4TX_MASK 0x00f00000
#define M4T0_MASK 0x000f8000
#define M4T1_MASK 0x00007c00
#define M4T2_MASK 0x000003e0
#define M4T3_MASK 0x0000001f
#define READ_V_MASK 0x00200000
#define READ_GR_MASK 0x001f0000
#define READ_RT0_MASK 0x0000ff00
#define READ_RT1_MASK 0x000000ff
#define WRITE_V_MASK 0x20
#define WRITE_GR_MASK 0x1f

#define WRITE_INSN_MASK 0x3f
#define READ_INSN_MASK 0x0fffffc0
#define HEADER_NIBBLE_MASK 0xf0000000

#define NO_TARGET_VALUE 0x00
#define PREDICATE_SLOT_VALUE 0x01
#define OP0_SLOT_VALUE 0x02
#define OP1_SLOT_VALUE 0x03
#define WRITE_SLOT_VALUE 0x01

/* NORMAL is relative to M3, M4. */
#define NORMAL_TARGET_BITS 9
#define PR_BITS 2
#define XOP_BITS 5
#define IMM_BITS NORMAL_TARGET_BITS
#define LSID_BITS XOP_BITS
#define EXIT_BITS 3
#define OFFSET_BITS 20
#define CONST_BITS 16
#define M3TX_BITS 2
#define M3_TARGET_BITS 7
#define M4_TARGET_BITS 5
#define READ_V_BITS 1
#define READ_GR_BITS 5
#define READ_TARGET_BITS 8
#define WRITE_V_BITS READ_V_BITS
#define WRITE_GR_BITS 5

#define NORMAL_INST_ID_BITS 7
#define WRITE_ID_BITS 5

#define HEADER_NIBBLE_BITS 4
#define READ_INSN_BITS 22
#define WRITE_INSN_BITS 6

#define BYTES_PER_GRID_WORD 4



/** TRIPS Opcode Numeric IDs **/
/** 000000000000000000000OO000000000 **/

#define ADD			(0x17)
#define ADDI		(0x1f)
/*
 *
 * Note that "AND" is defined originally in the ANSI compatibility
 * header file, namely in ../include/ansidecl.h, line 188.
 * We don't *need* that definition in our tools.
 *
 */
#undef AND
#define AND			(0x17)
#define ANDI		(0x1f)
#define APP			(0x04)
#define BR			(0x0c)
#define BRO			(0x08)
#define CALL		(0x0d)
#define CALLO		(0x09)
#define DIVS		(0x17)
#define DIVSI		(0x1f)
#define DIVU		(0x17)
#define DIVUI		(0x1f)
#define EXTSB		(0x13)
#define EXTSH		(0x13)
#define EXTSW		(0x13)
#define EXTUB		(0x13)
#define EXTUH		(0x13)
#define EXTUW		(0x13)
#define FADD		(0x16)
#define FDIV		(0x16)
#define FDTIO		(0x12)
#define FDTOS		(0x12)
#define FEQ			(0x16)
#define FGE			(0x16)
#define FGT			(0x16)
#define FITOD		(0x12)
#define FLE			(0x16)
#define FLT			(0x16)
#define FMUL		(0x16)
#define FNE			(0x16)
#define FSTOD		(0x12)
#define FSUB		(0x16)
#define GENS		(0x01)
#define GENU		(0x02)
#define LB			(0x20)
#define LBS			(0x24)
#define LD			(0x23)
#define LH			(0x21)
#define LHS			(0x25)
#define LOCK		(0x27)
#define LW			(0x22)
#define LWS			(0x26)
#define MFPC		(0x18)
#define MOV			(0x13)
#define MOV3		(0x06)
#define MOV4		(0x07)
#define MOVI		(0x18)
#define MUL			(0x17)
#define MULI		(0x1f)
#define NOP			(0x00)
/* In fact, this opcode is called NULL,
 * remember to deal it individually. */
#define NUL			(0x10)
#define OR			(0x17)
#define ORI			(0x1f)
#define RET			(0x0e)
#define SB			(0x28)
#define SCALL		(0x0a)
#define SD			(0x2b)
#define SH			(0x29)
#define SLL			(0x17)
#define SLLI		(0x1f)
#define SRA			(0x17)
#define SRAI		(0x1f)
#define SRL			(0x17)
#define SRLI		(0x1f)
#define SUB			(0x17)
#define SUBI		(0x1f)
#define SW			(0x2a)
#define TEQ			(0x17)
#define TEQI		(0x1f)
#define TGE			(0x17)
#define TGEI		(0x1f)
#define TGEU		(0x17)
#define TGEUI		(0x1f)
#define TGT			(0x17)
#define TGTI		(0x1f)
#define TGTU		(0x17)
#define	TGTUI		(0x1f)
#define TLE			(0x17)
#define TLEI		(0x1f)
#define TLEU		(0x17)
#define TLEUI		(0x1f)
#define TLT			(0x17)
#define TLTI		(0x1f)
#define TLTU		(0x17)
#define TLTUI		(0x1f)
#define TNE			(0x17)
#define TNEI		(0x1f)
#define XOR			(0x17)
#define XORI		(0x1f)
#define READ		(0x00)
#define WRITE		(0x00)

/** 00000000000000000000000000000000 **/
/** END: TRIPS Opcode Numeric IDs **/

enum insn_format
{
	UNKNOWN_FORM,
	G_FORM,
	I_FORM,
	L_FORM,
	S_FORM,
	B_FORM,
	C_FORM,
	M3_FORM,
	M4_FORM,
	R_FORM,
	W_FORM
};

#define BAD_WORD 0xdeadbeaf

enum node_type {
  ANY_TYPE,
  GPR_TYPE,
  ALU_TYPE,
  INVALID_TYPE
};

/* used both for registers and nodes */
typedef struct trips_node_specifier {
  int   flat_name;    /* we use this field to hold the 0..127 for N and 0...31 for R or W */
  char slot;   /* the slot specifier, if ntype == ALU_TYPE, '0' for left, '1' for right, 'p' for p,
                   else, 'W' for write slot*/
  enum node_type ntype;             /* ALU or GPR */
  int parse_status;	/* tag of the parse node process. */
} trips_node_specifier_t;

/* The opcode table is an array of struct trips_opcode.  */
typedef struct trips_opcode {
  /* The opcode name.  */
  const char *name;

  /* Format of instruction indicated by this opcode. */
  enum insn_format format;

  /* The insn mask. Bit vector of bits that must match
   * for the instruction indicated by this opcode. */
  unsigned long mask;

  /*value for bits to match in instruction. */
  unsigned long match;
} trips_opcode_t;

/* The table itself is sorted by major opcode number, and is otherwise
   in the order in which the disassembler should consider
   instructions.  */
extern const struct trips_opcode trips_opcodes[];
extern const int trips_num_opcodes;

#define TRIPS_OPCODE_32 (010)
#define TRIPS_OPCODE_64 (020)
#define TRIPS_OPCODE_TRIPS (01)


typedef struct trips_instruction_specifier {
  struct trips_node_specifier *source_node;
  struct trips_opcode *popcode;
  struct trips_node_specifier *target_one;
  struct trips_node_specifier *target_two;
  /* For M3 and M4. */
  struct trips_node_specifier *target_three;
  struct trips_node_specifier *target_four;
  struct trips_instruction_fields *insn_fields;
  enum insn_format instruction_format;
  int sequence_number; 
  trips_word the_word; /* the 32-bit quantity that we must construct */
  PTR aux_info;	/* currently used only by TAS. */
} trips_instruction_specifier_t;

typedef struct trips_instruction_fields 
{
	/* the value of OPCODE has include in popcode in
	 * trips_instruction_specifier, so there is no 
	 * opcode in this structure.*/
	trips_word IMM;
	trips_word PR;
	trips_word XOP;
	trips_word LSID;
	trips_word EXIT;
	trips_word OFFSET;
	trips_word CON;
	/* For M3 and M4. */
	trips_word M3TX;
	trips_word M4TX;
	/* For read and write. */
	trips_word V;
	trips_word GR;
} trips_instruction_fields_t;


typedef struct trips_aframe_stats
{
  int store_flag; /* Bit vector of store instruction. */
} trips_aframe_stats_t;


typedef struct trips_block_header_entry 
{
	trips_word  header_nibble;
	trips_word  read_insn;
	trips_word  write_insn;
} trips_block_header_entry_t;

typedef struct trips_processor_obj {
   char *name;
   trips_aframe_stats_t * aframe_stats; /* DITTO */
   int x_config;              /* number of rows in the grid */
   int y_config;              /* number of columns in the grid */
   int pframes_per_aframe;
   int num_flat_nodes;       /* TOTAL in 3D: 0..num_flat_nodes - 1 */

   int initialized;

   int sequence_numbers;
   unsigned int sequence_number_table [MAX_NUM_ROWS_IN_GRID * MAX_NUM_COLS_IN_GRID * MAX_NUM_PFRAMES_IN_GRID];

   struct trips_block_header_entry block_header[HEADER_CHUNK_SIZE];
   trips_word block_header_words[HEADER_CHUNK_SIZE];
   trips_word grid[MAX_NUM_ROWS_IN_GRID][MAX_NUM_COLS_IN_GRID][MAX_NUM_PFRAMES_IN_GRID]; 
} trips_processor_obj_t;

#endif /* TRIPS_HEADER */

extern trips_node_specifier_t *_trips_make_node_specifer PARAMS ((trips_processor_obj_t *pp));
extern trips_instruction_specifier_t *_trips_make_instruction_specifier PARAMS ((trips_processor_obj_t *pp));
extern trips_processor_obj_t *_trips_make_processor_specifier PARAMS (());
