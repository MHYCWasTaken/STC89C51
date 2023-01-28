/*
** figets like fgets only works backward from filepos pos instead of
** forward from the current filepointer
**
** returns fileposition of the begin of line read
**
** by Jan Vroonhof
*/

#include <stdio.h>
#include <string.h>

#define MAXLEN 90

long figets(FILE *file, char *buffer, long pos)
{
      char *ptr;
      long aap;

      aap = (pos - MAXLEN > 0 ? pos-MAXLEN : 0L);
      fseek(file, aap, SEEK_SET);
      fread(buffer + 100, 1, MAXLEN, file);
      buffer[pos - aap + 100] = 0;
      ptr = strrchr(buffer + 100, '\n');
      if (ptr)
      {
            *ptr = 0;
            ptr = strrchr(buffer + 100, '\n');
            if (ptr)
            {
                  strcpy(buffer, ptr + 1);
                  return aap + (ptr - buffer - 100) + 1;
            }
            else
            {
                  strcpy(buffer, buffer + 100);
                  return (aap ? -1L : 0L);
            }
      }
      else
      {
            strcpy(buffer, buffer + 100);
            return -1L;
      }
}

#ifdef TEST

void main(void)
{
      char buf[256];
      FILE *fp;
      long pos;

      fp = fopen("figets.c", "r");
      fseek(fp, 0L, SEEK_END);
      pos = ftell(fp);
      do {
            pos = figets(fp, buf, pos);
            puts (buf);
      } while (pos);
}

#endif /* TEST */
