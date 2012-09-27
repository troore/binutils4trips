/******************************************************************/
/* tcconfig.h
 *
 * TRIPS Tool Chain configuration file
 *
 * $Revision: 1.16 $
*/
/******************************************************************/
#ifndef TCCONFIGGED
#define TCCONFIGGED

extern int load_hw_config_file PARAMS ((void));

#define GRID_WIDTH            4 
/* Used by the tas, tgs, and tem. */
#define GRID_HEIGHT           4 
/* ditto */
#define PFRAMES_PER_AFRAME    8 
/* With grid_width & grid_height, determines reachability. */

#define ISA_VERSION           15
/* Updated to reflect major document updates, such as new opcodes. */

#define TCC_VERSION           1
#define TGS_VERSION           1 
/* Tools will be interested mostly in the *downstream* versions. */

#define TAS_VERSION           1 
/* Tools should bump the value when their interfaces change. */

#define TLD_VERSION           1
#define TEM_VERSION           1
#define TSIM_VERSION          1 
/* Reserved for the timing simulator. */

#define NUM_GPR_REGISTERS     128 
/* Used by most tools. */

#define ABSOLUTE_ADDRESSING   1 
/* Are targets specified by 0..127 numbers or by offsets? */


#define NUMBER_AFRAMES        4    
/* Is this value available or useful to the tool chain? */

#define DATA_WORD_SIZE        64

#define INSTRUCTION_WORD_SIZE 64

#define STACK_SIZE_KB        2048
/* for memory organization - 2MB */

#define HEAP_SIZE_KB        2048
/* for memory organization - 2MB */

#define PHYSICAL_MEM_KB      16384
/* for memory organization - 16MB */

#define ADDRESS_SPACE_SIZE    64 
/* Important for code that generates data memory addresses. */

#define CODE_SPACE_SIZE       32
/* Important for any code that generates code addresses. */

#define INSN_SEQUENCE_NUMBERS  1 
/* For deterministic execution. */

#define COMPACT_FORM           0 
/* If and when we go to variable-sized instruction blocks. */

#define EXTENDED_FORM 1 
/* For uninhibited instruction word formats. */

/* Following are for the compiler to calculate instruction counts */
#define INSNS_PER_ENTER            1
#define INSNS_PER_ENTERB           7
#define INSNS_PER_ENTERA           13

/* represents the percent reduction in instructions that can
   actually be scheduled in the volume of  a given grid  */
#define GRID_REDUCTION_VALUE       90 

typedef struct trips_config_knobs {
  char * name;
  unsigned int param_define;
  int value;
} trips_config_knobs_t;

extern trips_config_knobs_t trips_config_table[];

/* NOTE: This definitions must coincide with the entries
   in the trips_config_table itself.  Otherwise, when
   you reference the table through one of them, you will
   get the wrong parameter value.

*/
#define INDEX_NO_ENTRY              0 
#define INDEX_ISA_VERSION           1 
#define INDEX_TCC_VERSION           2
#define INDEX_TGS_VERSION           3 
#define INDEX_TAS_VERSION           4 
#define INDEX_TLD_VERSION           5
#define INDEX_TEM_VERSION           6
#define INDEX_TSIM_VERSION          7 
#define INDEX_NUM_GPR_REGISTERS     8
#define INDEX_ABSOLUTE_ADDRESSING   9 
#define INDEX_GRID_WIDTH            10 
#define INDEX_GRID_HEIGHT           11
#define INDEX_PFRAMES_PER_AFRAME    12 
#define INDEX_NUMBER_AFRAMES        13
#define INDEX_DATA_WORD_SIZE        14
#define INDEX_INSTRUCTION_WORD_SIZE 15
#define INDEX_STACK_SIZE_KB         16
#define INDEX_HEAP_SIZE_KB          17
#define INDEX_PHYSICAL_MEM_KB       18
#define INDEX_ADDRESS_SPACE_SIZE    19
#define INDEX_CODE_SPACE_SIZE       20
#define INDEX_INSN_SEQUENCE_NUMBERS 21 
#define INDEX_COMPACT_FORM          22 
#define INDEX_EXTENDED_FORM         23
#define INDEX_INSNS_PER_ENTER       24
#define INDEX_INSNS_PER_ENTERB      25
#define INDEX_INSNS_PER_ENTERA      26 
#define INDEX_GRID_REDUCTION_VALUE  27
/******************************************************************/
#endif /* TCCONFIGGED */
