/*
**  FLENGTH.C - a simple function using all ANSI-standard functions
**              to determine the size of a file.
**
**  Public domain by Bob Jarvis.
*/

#include <stdio.h>
#include <io.h>

long flength(char *fname)
{
      FILE *fptr;
      long length = 0L;

      fptr = fopen(fname, "rb");
      if(fptr != NULL)
      {
            fseek(fptr, 0L, SEEK_END);
            length = ftell(fptr);
            fclose(fptr);
      }

      return length;
}

#ifdef TEST

void main(int argc, char *argv[])
{
      printf("Length of %s = %ld\n", argv[0], flength(argv[0]));
}

#endif /* TEST */
