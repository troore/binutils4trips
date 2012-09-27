/* This is the Assembler Pre-Processor
   Copyright 1987, 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000
   Free Software Foundation, Inc.

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

/* App, the assembler pre-processor.  This pre-processor strips out excess
   spaces, turns single-quoted characters into a decimal constant, and turns
   # <number> <filename> <garbage> into a .line <number>\n.file <filename>
   pair.  This needs better error-handling.  */

#include <stdio.h>
#include "as.h"			/* For BAD_CASE() only */

#if (__STDC__ != 1)
#ifndef const
#define const  /* empty */
#endif
#endif

#define scrub_m68k_mri 0

static char lex[256];
#ifdef TRIPS
static const char symbol_chars[] =
"$._ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789%";
#else /* TRIPS */
static const char symbol_chars[] =
"$._ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
#endif 

#define LEX_IS_SYMBOL_COMPONENT		1
#define LEX_IS_WHITESPACE		2
#define LEX_IS_LINE_SEPARATOR		3
#define LEX_IS_COMMENT_START		4
#define LEX_IS_LINE_COMMENT_START	5
#define	LEX_IS_TWOCHAR_COMMENT_1ST	6
#define	LEX_IS_STRINGQUOTE		8
#define	LEX_IS_COLON			9
#define	LEX_IS_NEWLINE			10
#define	LEX_IS_ONECHAR_QUOTE		11
#ifdef TC_V850
#define LEX_IS_DOUBLEDASH_1ST		12
#endif
#ifdef TC_M32R
#define DOUBLEBAR_PARALLEL
#endif
#ifdef DOUBLEBAR_PARALLEL
#define LEX_IS_DOUBLEBAR_1ST		13
#endif
#define LEX_IS_PARALLEL_SEPARATOR	14
#define IS_SYMBOL_COMPONENT(c)		(lex[c] == LEX_IS_SYMBOL_COMPONENT)
#define IS_WHITESPACE(c)		(lex[c] == LEX_IS_WHITESPACE)
#define IS_LINE_SEPARATOR(c)		(lex[c] == LEX_IS_LINE_SEPARATOR)
#define IS_PARALLEL_SEPARATOR(c)	(lex[c] == LEX_IS_PARALLEL_SEPARATOR)
#define IS_COMMENT(c)			(lex[c] == LEX_IS_COMMENT_START)
#define IS_LINE_COMMENT(c)		(lex[c] == LEX_IS_LINE_COMMENT_START)
#define	IS_NEWLINE(c)			(lex[c] == LEX_IS_NEWLINE)

static int process_escape PARAMS ((int));

/* FIXME-soon: The entire lexer/parser thingy should be
   built statically at compile time rather than dynamically
   each and every time the assembler is run.  xoxorich.  */

void
do_scrub_begin (m68k_mri)
     int m68k_mri ATTRIBUTE_UNUSED;
{
  const char *p;
  int c;

  lex[' '] = LEX_IS_WHITESPACE;
  lex['\t'] = LEX_IS_WHITESPACE;
  lex['\r'] = LEX_IS_WHITESPACE;
  lex['\n'] = LEX_IS_NEWLINE;
  lex[':'] = LEX_IS_COLON;

#ifdef TC_M68K
  scrub_m68k_mri = m68k_mri;

  if (! m68k_mri)
#endif
    {
      lex['"'] = LEX_IS_STRINGQUOTE;

#if ! defined (TC_HPPA) && ! defined (TC_I370)
      /* I370 uses single-quotes to delimit integer, float constants */
      lex['\''] = LEX_IS_ONECHAR_QUOTE;
#endif

#ifdef SINGLE_QUOTE_STRINGS
      lex['\''] = LEX_IS_STRINGQUOTE;
#endif
    }

  /* Note: if any other character can be LEX_IS_STRINGQUOTE, the loop
     in state 5 of do_scrub_chars must be changed.  */

  /* Note that these override the previous defaults, e.g. if ';' is a
     comment char, then it isn't a line separator.  */
  for (p = symbol_chars; *p; ++p)
    {
      lex[(unsigned char) *p] = LEX_IS_SYMBOL_COMPONENT;
    }				/* declare symbol characters */

  for (c = 128; c < 256; ++c)
    lex[c] = LEX_IS_SYMBOL_COMPONENT;

#ifdef tc_symbol_chars
  /* This macro permits the processor to specify all characters which
     may appears in an operand.  This will prevent the scrubber from
     discarding meaningful whitespace in certain cases.  The i386
     backend uses this to support prefixes, which can confuse the
     scrubber as to whether it is parsing operands or opcodes.  */
  for (p = tc_symbol_chars; *p; ++p)
    lex[(unsigned char) *p] = LEX_IS_SYMBOL_COMPONENT;
#endif

  /* The m68k backend wants to be able to change comment_chars.  */
#ifndef tc_comment_chars
#define tc_comment_chars comment_chars
#endif
  for (p = tc_comment_chars; *p; p++)
    {
      lex[(unsigned char) *p] = LEX_IS_COMMENT_START;
    }				/* declare comment chars */

  for (p = line_comment_chars; *p; p++)
    {
      lex[(unsigned char) *p] = LEX_IS_LINE_COMMENT_START;
    }				/* declare line comment chars */

  for (p = line_separator_chars; *p; p++)
    {
      lex[(unsigned char) *p] = LEX_IS_LINE_SEPARATOR;
    }				/* declare line separators */

#ifdef tc_parallel_separator_chars
  /* This macro permits the processor to specify all characters which
     separate parallel insns on the same line.  */
  for (p = tc_parallel_separator_chars; *p; p++)
    {
      lex[(unsigned char) *p] = LEX_IS_PARALLEL_SEPARATOR;
    }				/* declare parallel separators */
#endif

  /* Only allow slash-star comments if slash is not in use.
     FIXME: This isn't right.  We should always permit them.  */
  if (lex['/'] == 0)
    {
      lex['/'] = LEX_IS_TWOCHAR_COMMENT_1ST;
    }

#ifdef TC_M68K
  if (m68k_mri)
    {
      lex['\''] = LEX_IS_STRINGQUOTE;
      lex[';'] = LEX_IS_COMMENT_START;
      lex['*'] = LEX_IS_LINE_COMMENT_START;
      /* The MRI documentation says '!' is LEX_IS_COMMENT_START, but
         then it can't be used in an expression.  */
      lex['!'] = LEX_IS_LINE_COMMENT_START;
    }
#endif

#ifdef TC_V850
  lex['-'] = LEX_IS_DOUBLEDASH_1ST;
#endif
#ifdef DOUBLEBAR_PARALLEL
  lex['|'] = LEX_IS_DOUBLEBAR_1ST;
#endif
#ifdef TC_D30V
  /* must do this is we want VLIW instruction with "->" or "<-" */
  lex['-'] = LEX_IS_SYMBOL_COMPONENT;
#endif
}				/* do_scrub_begin() */

/* Saved state of the scrubber */
static int state;
static int old_state;
static char *out_string;
static char out_buf[20];
static int add_newlines;
static char *saved_input;
static int saved_input_len;
static char input_buffer[32 * 1024];
static const char *mri_state;
static char mri_last_ch;

/* Data structure for saving the state of app across #include's.  Note that
   app is called asynchronously to the parsing of the .include's, so our
   state at the time .include is interpreted is completely unrelated.
   That's why we have to save it all.  */

struct app_save {
  int          state;
  int          old_state;
  char *       out_string;
  char         out_buf[sizeof (out_buf)];
  int          add_newlines;
  char *       saved_input;
  int          saved_input_len;
#ifdef TC_M68K
  int          scrub_m68k_mri;
#endif
  const char * mri_state;
  char         mri_last_ch;
#if defined TC_ARM && defined OBJ_ELF
  const char * symver_state;
#endif
};

char *
app_push ()
{
  register struct app_save *saved;

  saved = (struct app_save *) xmalloc (sizeof (*saved));
  saved->state = state;
  saved->old_state = old_state;
  saved->out_string = out_string;
  memcpy (saved->out_buf, out_buf, sizeof (out_buf));
  saved->add_newlines = add_newlines;
  if (saved_input == NULL)
    saved->saved_input = NULL;
  else
    {
      saved->saved_input = xmalloc (saved_input_len);
      memcpy (saved->saved_input, saved_input, saved_input_len);
      saved->saved_input_len = saved_input_len;
    }
#ifdef TC_M68K
  saved->scrub_m68k_mri = scrub_m68k_mri;
#endif
  saved->mri_state = mri_state;
  saved->mri_last_ch = mri_last_ch;
#if defined TC_ARM && defined OBJ_ELF
  saved->symver_state = symver_state;
#endif

  /* do_scrub_begin() is not useful, just wastes time.  */

  state = 0;
  saved_input = NULL;

  return (char *) saved;
}

void
app_pop (arg)
     char *arg;
{
  register struct app_save *saved = (struct app_save *) arg;

  /* There is no do_scrub_end ().  */
  state = saved->state;
  old_state = saved->old_state;
  out_string = saved->out_string;
  memcpy (out_buf, saved->out_buf, sizeof (out_buf));
  add_newlines = saved->add_newlines;
  if (saved->saved_input == NULL)
    saved_input = NULL;
  else
    {
      assert (saved->saved_input_len <= (int) (sizeof input_buffer));
      memcpy (input_buffer, saved->saved_input, saved->saved_input_len);
      saved_input = input_buffer;
      saved_input_len = saved->saved_input_len;
      free (saved->saved_input);
    }
#ifdef TC_M68K
  scrub_m68k_mri = saved->scrub_m68k_mri;
#endif
  mri_state = saved->mri_state;
  mri_last_ch = saved->mri_last_ch;
#if defined TC_ARM && defined OBJ_ELF
  symver_state = saved->symver_state;
#endif

  free (arg);
}				/* app_pop() */

/* @@ This assumes that \n &c are the same on host and target.  This is not
   necessarily true.  */
static int
process_escape (ch)
     int ch;
{
  switch (ch)
    {
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case '\'':
      return '\'';
    case '"':
      return '\"';
    default:
      return ch;
    }
}

/* This function is called to process input characters.  The GET
   parameter is used to retrieve more input characters.  GET should
   set its parameter to point to a buffer, and return the length of
   the buffer; it should return 0 at end of file.  The scrubbed output
   characters are put into the buffer starting at TOSTART; the TOSTART
   buffer is TOLEN bytes in length.  The function returns the number
   of scrubbed characters put into TOSTART.  This will be TOLEN unless
   end of file was seen.  This function is arranged as a state
   machine, and saves its state so that it may return at any point.
   This is the way the old code used to work.  */

int
do_scrub_chars (get, tostart, tolen)
	int (*get) PARAMS ((char *, int));
	char *tostart;
	int tolen;
{
	char *to = tostart;
	char *toend = tostart + tolen;
	char *from;
	char *fromend;
	int fromlen;
	register int ch, ch2 = 0;
	int fromeof = 0, tofull = 0;

	/*State 0: beginning of normal line
			3: after some non-white and one white on line (flush white)
			5: parsing a string, then go to old-state
			11: After seeing a symbol character in state 0 (eg a label definition) or in state 3
	*/

	/* This macro gets the next input character. */

#define GET()							\
	(from < fromend						\
	 ? * (unsigned char *) (from++)				\
	 : (saved_input = NULL,					\
		 fromlen = (*get) (input_buffer, sizeof input_buffer),	\
		 from = input_buffer,					\
		 fromend = from + fromlen,					\
		 (fromlen == 0						\
		  ? EOF							\
		  : * (unsigned char *) (from++))))

	/* This macro pushes a character back on the input stream.  */

#define UNGET(uch) (*--from = (uch))

	/* This macro puts a character into the output buffer.  If this
	   character fills the output buffer, this macro jumps to the label
	   TOFULL.  We use this rather ugly approach because we need to
	   handle two different termination conditions: EOF on the input
	   stream, and a full output buffer.  It would be simpler if we
	   always read in the entire input stream before processing it, but
	   I don't want to make such a significant change to the assembler's
	   memory usage.  */

#define PUT(pch)			\
	do					\
	{					\
		*to++ = (pch);			\
		if (to >= toend)			\
			tofull = 1;			\
	}					\
	while (0)

	if (saved_input != NULL)
	{
		from = saved_input;
		fromend = from + saved_input_len;
	}
	else
	{
		fromlen = (*get) (input_buffer, sizeof input_buffer);
		if (fromlen == 0)
			return 0;
		from = input_buffer;
		fromend = from + fromlen;
	}

	do
	{
		ch = GET();
		if (ch == EOF)
		{
			if (state != 0)
			{
				as_warn (_("end of file not at end of a line; newline inserted"));
				state = 0;
				PUT('\n'); /* There must be a sentinal to the buffer. */
			}
			fromeof = 1;
			break;
		}
		switch (state)
		{
			case 0:
				switch (lex[ch])
				{
					case LEX_IS_COLON:
					case LEX_IS_WHITESPACE:
					case LEX_IS_LINE_COMMENT_START:
						UNGET (ch);
						state = 11;
						break;
					case LEX_IS_NEWLINE:
						state = 0;
						PUT ('\n');
						break;
					case LEX_IS_SYMBOL_COMPONENT:
					case LEX_IS_STRINGQUOTE:
						UNGET (ch);
						state = 3;
						break;
					default:
						/* As to illegal chars, output them into the to buffer
						   as if they never exit. */
						PUT (ch);
						break;
				}
				break;
			case 11:
				switch (lex[ch])
				{
					case LEX_IS_WHITESPACE:
						do 
						{
							ch = GET ();
						}
						while (IS_WHITESPACE(ch));
						UNGET (ch);
						PUT (' ');
						state = 3;
						break;
					case LEX_IS_NEWLINE:
						PUT ('\n');
						state = 0;
						break;
					case LEX_IS_COLON:
						PUT (ch);
						ch = GET ();
						if (lex[ch] == LEX_IS_WHITESPACE)
						{
							do
							{
								ch = GET ();
							}
							while (IS_WHITESPACE(ch));
						}
						UNGET (ch);
						PUT (' ');
						state = 3;
						break;
					case LEX_IS_LINE_COMMENT_START:
					case LEX_IS_STRINGQUOTE:
						UNGET (ch);
						state = 3;
						break;
				}
				break;
			case 3:
				switch (lex[ch])
				{
					case LEX_IS_SYMBOL_COMPONENT:
						{
							int type = lex[ch];

							while ((type == 0 || type == LEX_IS_SYMBOL_COMPONENT) && !tofull)
							{
								PUT (ch);
								ch = GET ();
								type = lex[ch];
							}
							if (type != 0 && type != LEX_IS_SYMBOL_COMPONENT)
							{
								UNGET (ch);
								state = 11;
							}
							if (tofull)
							{
								UNGET (ch);
							}
						} 
						break;
					case LEX_IS_LINE_COMMENT_START:
						while (ch != EOF && !IS_NEWLINE(ch))
						{
							ch = GET ();
						}
						if (IS_NEWLINE(ch))
						{
							state = 0;
							PUT ('\n');
						}
						else
						{
							/* ch == EOF. */
							UNGET (ch);
						}
						break;
					case LEX_IS_NEWLINE:
						PUT ('\n');
						state = 0;
						break;
					case LEX_IS_STRINGQUOTE:
						PUT (ch);
						old_state = state;
						state = 5;
						break;
					default:
						PUT (ch);
						break;
				}
				break;
			case 5:
				/* We are going to copy as many chars in a pair of quotes
				 * as possible.
				 */
				while (ch != EOF && ch != '"' && !tofull)
				{
					PUT (ch);
					ch = GET ();
				}
				if (tofull)
				{
					UNGET (ch);
				}
				else
				{
					if (ch == EOF)
					{
						as_warn (_("end of file in string; inserted '\"'"));
						PUT ('"');
						UNGET (ch);
					}
					else 
					{
						PUT (ch);
					}
					state = old_state;
				}
				break;
		}
	}
	while (!fromeof && !tofull);

	/* We have reached the end of the input. */
	if (fromeof == 1)
	{
		return to - tostart;
	}
	if (tofull == 1)
	{
		if (fromend > from)
		{
			saved_input = from;
			saved_input_len = fromend - from;
		}
		else
		{
			saved_input = NULL;
		}
	}

	return to - tostart;
}
