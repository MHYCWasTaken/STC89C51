/*
**  Case-sensitive Boyer-Moore-Horspool pattern match
**
**  public domain by Raymond Gardner 7/92
**
**  limitation: pattern length + string length must be less than 32767
*/

#include <stddef.h>
#include <string.h>

typedef unsigned char uchar;

#define LARGE 32767

static int patlen;
static int skip[256];
static int skip2;
static uchar *pat;

void bmh_init(const char *pattern)
{
          int i, lastpatchar;

          pat = (uchar *)pattern;
          patlen = strlen(pattern);
          for (i = 0; i < 256; ++i)
                skip[i] = patlen;
          for (i = 0; i < patlen; ++i)
                skip[pat[i]] = patlen - i - 1;
          lastpatchar = pat[patlen - 1];
          skip[lastpatchar] = LARGE;
          skip2 = patlen;                 /* Horspool's fixed second shift */
          for (i = 0; i < patlen - 1; ++i)
          {
                if (pat[i] == lastpatchar)
                      skip2 = patlen - i - 1;
          }
}

char *bmh_search(const char *string, const int stringlen)
{
      int i, j;
      char *s;

      i = patlen - 1 - stringlen;
      if (i >= 0)
            return NULL;
      string += stringlen;
      for ( ;; )
      {
            while ( (i += skip[((uchar *)string)[i]]) < 0 )
                  ;                           /* mighty fast inner loop */
            if (i < (LARGE - stringlen))
                  return NULL;
            i -= LARGE;
            j = patlen - 1;
            s = (char *)string + (i - j);
            while (--j >= 0 && s[j] == pat[j])
                  ;
            if ( j >= 0 )
                  i += skip2;
            else  return s;
      }
}
