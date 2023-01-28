/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1986, S.E. Margison
**  Copyright 1989, Robert B.Stout
**
**  Subset version released to the public domain, 1992
**
**  Make an ascii hexadecimal string into an integer.
*/

#include <stdio.h>
#include <ctype.h>

#define NUL '\0'

unsigned int hstr_i(char *cptr)
{
      unsigned int i, j = 0;

      while ((NUL != *cptr) && isxdigit(*cptr))
      {
            i = *cptr++ - '0';
            if (9 < i)
                  i -= 7;
            j <<= 4;
            j |= (i & 0x0f);
      }
      return(j);
}
