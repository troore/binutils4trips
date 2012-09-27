/**************************************************************
  Name:         config.c

  Description:  Routines to execute parse the config file.

  Copyright:    Computer Architecture and Technology Laboratory 
                University of Texas at Austin 
                (c) 2002
                        
  Version:      $Revision: 1.20 $
**************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <string.h> 

#include "libiberty.h" /* for xmalloc() decl's */

/* in binutils/include/opcode */
#include "opcode/tcconfig.h"

#define MAX_CONFIG_LINE_LEN 256
#define TC_CONFIGNAME "/tcconfig.h"
#define SCAN_PARAM_FAILURE_VALUE -299

char BUILTIN_TC_CONFIGPATH[]=".:$HOME:/projects/trips/toolchain/current/etc/trips:/projects/trips/toolchain/stable/etc/trips";


/* Make these DATA available to everyone within this file */
static char hw_config_line[MAX_CONFIG_LINE_LEN];
static char * hw_config_input_line_pointer = NULL;
static char ultimate_path_name_to_config_file[444];
static const int trips_num_hw_configs; /* forward reference */

/* METHODS */
static int find_hw_config_file PARAMS ((void));
static int lookup_one_config_dir PARAMS ((char * ));
static int get_line PARAMS ((FILE *));
static void process_line PARAMS ((void));
static void eat_blanks PARAMS ((void));
static int get_define PARAMS ((void));
static int get_config_param PARAMS ((void));
static int get_config_value PARAMS ((void));
static int lookup_one_config_param_value PARAMS ((char * ));

static int
lookup_one_config_param_value (param_name)
  char * param_name;
{
  int retval = INDEX_NO_ENTRY;
  trips_config_knobs_t * hw_config;
  trips_config_knobs_t * end_hw_config;
  end_hw_config = trips_config_table + trips_num_hw_configs;
  for (hw_config = trips_config_table; hw_config < end_hw_config; hw_config++) {
      if (! (strcmp(hw_config->name, param_name))) {
        /* bingo */
        retval = hw_config->param_define;
        break;
      }
  }
  return retval;
}
/* if found, set the ultimate_path_name_to_config_file */
static int
lookup_one_config_dir (subdir)
  char * subdir;
{
  char * full_path;
  struct stat buf;
  char * env_var_name;
  char * env_var_value_0;
  char * env_var_value_1;

  char subdir_copy[256];
  strcpy (subdir_copy, subdir);

  full_path = xmalloc(256);
  full_path[0] = '\0';
  if (subdir_copy[0] == '$') {
    env_var_name = xmalloc(strlen(subdir_copy) + 1);
    env_var_value_0 = getenv(env_var_name);
    if (env_var_value_0) {
      full_path = strcat(env_var_value_0, TC_CONFIGNAME); 
    }
  }
  else {
    full_path = strcat(subdir_copy, TC_CONFIGNAME);
  }
  /* printf("fpath: %s\n", full_path); */
  if (! stat(full_path, &buf)) {
    strcpy(ultimate_path_name_to_config_file,full_path);
    if (S_ISREG(buf.st_dev)) {
      /* why does this check always fail? */
      return 1;
    }
    else {
      /* If this worked OK, we would return 0 here */
      return 1;
    }
  }
  else {
    return 0;
  }
}
static int
find_hw_config_file ()
{
  char * path = NULL;
  int found = 0;
  char sep_char = ':';
  int sindex, tick_pos;
  char * tc_path;
	char candidate_dir_name[300];

  path=getenv("TC_CONFIGPATH"); 
  if (!path) {
    path=BUILTIN_TC_CONFIGPATH;
  }
  tc_path = strdup(path);
  sindex = 0; tick_pos = 0;
  while (! found && (tc_path[sindex])) {
    if (tc_path[sindex] == sep_char) {
      candidate_dir_name[tick_pos] = '\0';
      found = lookup_one_config_dir(candidate_dir_name);
      if (! found) {
        /* start at next position */
        tick_pos=0; sindex++;
      }
    }
    else {
      candidate_dir_name[tick_pos++] = tc_path[sindex++];
    }
  }
  if (! found) {
      candidate_dir_name[tick_pos] = '\0';
      found = lookup_one_config_dir(candidate_dir_name);
  }
  return (found);
}
static int
get_line (f) 
/* THANKS TO K&R, p.29 */
  FILE *f;
{
  int c, i;
  for (i =0; i<MAX_CONFIG_LINE_LEN - 1 && (c = fgetc(f)) != EOF && c!='\n'; ++i) {
    hw_config_line[i] = c;
  }
  if (c == '\n') {
    hw_config_line[i] = c;
    ++i;
  }
  hw_config_line[i] = '\0';
  hw_config_input_line_pointer = hw_config_line;
  return i;
}
int
load_hw_config_file () 
{
  FILE * config_file;
  int slen;
	int found = 0;
	found = find_hw_config_file();
	if (! found) {
		return -1;
	}
  config_file = fopen(ultimate_path_name_to_config_file,"r");
  while ((slen = get_line(config_file)) > 0) {
    process_line();
  }
  fclose(config_file);
  return 0;
}

static void 
eat_blanks ()
{
  int i=0;
  while ((*hw_config_input_line_pointer ==' ') ||
    (*hw_config_input_line_pointer =='\t')) {
    hw_config_input_line_pointer++;
    i++; /* just for the heck of it */
  }
  return;
}
static int
get_config_value ()
{
  char sep_char = ' ';
  int scan_value = 0;
  int tick_pos;
  char candidate_hw_param_value_string[300];
  int candidate_hw_param_value;

  tick_pos = 0;
  while (*hw_config_input_line_pointer) {
    if ((*hw_config_input_line_pointer == sep_char) ||
        (*hw_config_input_line_pointer == '\n')) {
      candidate_hw_param_value_string[tick_pos] = '\0';
      /* printf("PARAM VALUE:%s.\n", candidate_hw_param_value_string); */
      scan_value=sscanf(candidate_hw_param_value_string, "%d", 
                       &candidate_hw_param_value);
      if (scan_value) {
        return candidate_hw_param_value; 
      }
      else {
        return SCAN_PARAM_FAILURE_VALUE; 
      }
    }
    else {
      candidate_hw_param_value_string[tick_pos++] = *hw_config_input_line_pointer++;
    }
  }
  return 0;
}
static int
get_config_param ()
{
  int found = 0;
  char sep_char = ' ';
  int tick_pos;
  char candidate_hw_param_name[300];
  tick_pos = 0;
  while (! found && *hw_config_input_line_pointer) {
    if (*hw_config_input_line_pointer == sep_char) {
      candidate_hw_param_name[tick_pos] = '\0';
      /* printf("PARAM NAME:%s.\n", candidate_hw_param_name); */
      found = lookup_one_config_param_value(candidate_hw_param_name);
      if (! found) {
        return 0;
      }
    }
    else {
      candidate_hw_param_name[tick_pos++] = *hw_config_input_line_pointer++;
    }
  }
  return found;
}

static int
get_define ()
{
  int res=0;
  int i;
  if (!(strncmp("#define", hw_config_input_line_pointer, 7))) {
    res=strlen("#define");
    for  (i=0; i < res; i++) {
      hw_config_input_line_pointer++;
    }
  }
  return res;
}
static void 
process_line ()
{
  int define_len = 0;
  int res = 0;
  int value = 0;

  eat_blanks();
  if ((define_len = get_define())) {
    eat_blanks();
  }
  else {
    return;
  }
  res = get_config_param();
  if (res) {
    eat_blanks();
    value = get_config_value();
  }
	else {
		return;
	}
  if (value != SCAN_PARAM_FAILURE_VALUE) {
		/* update that turkey */
		trips_config_table[res].value = value;
  }
  else {
    /* printf("NOT FOUND:%s.\n",hw_config_input_line_pointer); */
		return;
  }
  return;
}
trips_config_knobs_t trips_config_table[]={
  {"HW_CONFIG_NOT_USED", INDEX_NO_ENTRY,  -1},
  {"ISA_VERSION", INDEX_ISA_VERSION,          1},
  {"TCC_VERSION", INDEX_TCC_VERSION,          1},
  {"TGS_VERSION", INDEX_TGS_VERSION,          1},
  {"TAS_VERSION", INDEX_TAS_VERSION,          1},
  {"TLD_VERSION", INDEX_TLD_VERSION,          1},
  {"TEM_VERSION", INDEX_TEM_VERSION,          1},
  {"TSIM_VERSION", INDEX_TSIM_VERSION,         1},
  {"NUM_GPR_REGISTERS", INDEX_NUM_GPR_REGISTERS,    128},
  {"ABSOLUTE_ADDRESSING", INDEX_ABSOLUTE_ADDRESSING,  1},
  {"GRID_WIDTH", INDEX_GRID_WIDTH,           4},
  {"GRID_HEIGHT", INDEX_GRID_HEIGHT,          4},
  {"PFRAMES_PER_AFRAME", INDEX_PFRAMES_PER_AFRAME,   8},
  {"NUMBER_AFRAMES", INDEX_NUMBER_AFRAMES,       4   },
  {"DATA_WORD_SIZE", INDEX_DATA_WORD_SIZE,       64},
  {"INSTRUCTION_WORD_SIZE", INDEX_INSTRUCTION_WORD_SIZE,64},
  {"INSTRUCTION_WORD_SIZE", INDEX_INSTRUCTION_WORD_SIZE,32},
  {"STACK_SIZE_KB", INDEX_STACK_SIZE_KB,        (1024 * 2)},
  {"HEAP_SIZE_KB", INDEX_HEAP_SIZE_KB,        (1024 * 2)},
  {"PHYSICAL_MEM_KB", INDEX_PHYSICAL_MEM_KB,   (1024 * 16)},
  {"ADDRESS_SPACE_SIZE", INDEX_ADDRESS_SPACE_SIZE,   64},
  {"CODE_SPACE_SIZE", INDEX_CODE_SPACE_SIZE,      32},
  {"INSN_SEQUENCE_NUMBERS", INDEX_INSN_SEQUENCE_NUMBERS, 1},
  {"COMPACT_FORM", INDEX_COMPACT_FORM,          0},
  {"EXTENDED_FORM", INDEX_EXTENDED_FORM, 1},
  {"INSNS_PER_ENTER", INDEX_INSNS_PER_ENTER,           1},
  {"INSNS_PER_ENTERB", INDEX_INSNS_PER_ENTERB,          7},
  {"INSNS_PER_ENTERA", INDEX_INSNS_PER_ENTERA,          13},
  {"GRID_REDUCTION_VALUE", INDEX_GRID_REDUCTION_VALUE,      90},
};

static const int trips_num_hw_configs = sizeof (trips_config_table) / sizeof (trips_config_table[0]);
