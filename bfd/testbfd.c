#include "bfd.h"
#include "sysdep.h"
#include "bfdlink.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/ppc.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))

/*
 *  string databases of phrases
 */
#define NUM_PHRASE_A 	7
char *phrase_A[]= {
	"Compound",
	"Distributed",
	"Highly",
	"Integrated",
	"Media-Enriched",
	"Object-Oriented",
	"Validation"
};

#define TARGET "elf32-powerpc"

unsigned int number_of_sections (bfd *abfd) {

  int r = 0;
  /* This call is mainly for its side effect of reading in the sections.
     We follow the traditional behavior of `strings' in that we don't
     complain if we don't recognize a file to be an object file.  */

  if (bfd_check_format (abfd, bfd_object) == false)
    {
      bfd_close (abfd);
      return r;
   }
   r = bfd_count_sections(abfd);
   return r;
}

main ()
{
    int i,j,k,t;

    char * cptr, ch;

    int count=-1;

    bfd *pbfd;

    /* pbfd=bfd_openr("/tmp/buzzz.o",TARGET); */
    pbfd=bfd_openr("/tmp/buzzz.o",NULL);
    /* bfd *abfd = bfd_openr (file, target); */

    if (pbfd == NULL)
    {
      /* Treat the file as a non-object file.  */
      return false;
    }
    count=number_of_sections(pbfd);

    printf("Number of sections: %d\n",count);

    exit(0);
}

