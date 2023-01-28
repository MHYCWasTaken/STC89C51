/*
**  TAIL.C  Display the last "n" lines of a file (5 lines by default).
**  Written February 4, 1986 by Joe Huffman
**  Modified October 5, 1986 by Joe Huffman
**    Utilize prototyping, fixed a bug, added (a few) comments and help.
**
**  Not copyrighted.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char head1[] = {"\n------- \""},
              head2[] = {" -------\n"};
FILE *fp;
int filenum, cc;
unsigned int linenum = 5, indx;
long int *tail;
void help (void);
void getlinenum (int n, unsigned char *str[]);
void gettail (void);
void LtoA (long int val, unsigned char *cp);

main (int argc, unsigned char *argv[])
{
      if (argc <= 1)
      {
            help ();
            return (1);
      }

      getlinenum (argc, argv);

      for (filenum = 1; filenum < argc; ++filenum)
      {
            if (*argv[filenum] == '/')
                  continue;
            fp = fopen (argv[filenum], "rb");
            if (!fp)
            {
                  fputs (head1, stderr);
                  fputs (argv[filenum], stderr);
                  fputs ("\" not found.", stderr);
                  fputs (head2, stderr);
            }
            else
            {
                  fputs (head1, stdout);
                  fputs (argv[filenum], stdout);
                  gettail ();
                  fputs (head2, stdout);
                  for (cc = getc (fp); cc != EOF; cc = getc (fp))
                        fputc (cc, stdout);
                  fclose (fp);
            }
      }
}

/*
**  Get the number of lines to display at the "tail" of each file from
**  the command line.
*/

void getlinenum (int n, unsigned char *str[])
{
      for (--n; n; --n)
      {
            ++str;
            if (**str == '/')
            {
                  linenum = atoi (*(str) + 1);
                  if (linenum <= 0)
                        linenum = 5;
            }
      }
      linenum++;
      /* Because we save a pointer to the end of the PREVIOUS line. */
}

/*
**  Set the file pointer "fp" to "linenum - 1" lines before the end of
**  the file.
*/

void gettail (void)
{
      unsigned char outstr[15];
      void *malloc (unsigned int);
      unsigned long int currline = 0L;

      tail = (long int *)malloc (sizeof(*tail) * linenum);
      if (!tail)
      {
            fputs ("Insufficient memory.", stderr);
            exit (1);
      }
      tail[0] = ftell (fp);
      indx = 0;

      for (cc = getc (fp); cc != EOF; cc = getc (fp))
      {
            if (cc == '\r')
            {
                  ++currline;
                  cc = getc (fp);
                  if (cc != '\n')
                        ungetc (cc, fp);
                  ++indx;
                  indx %= linenum;
                  tail[indx] = ftell (fp);
            }
            else
            {
                  if (cc == '\n')
                  {
                        ++currline;
                        cc = getc (fp);
                        if (cc != '\r')
                              ungetc (cc, fp);
                        ++indx;
                        indx %= linenum;
                        tail[indx] = ftell (fp);
                  }
            }
      }
      fputs ("\" ", stdout);
      LtoA (currline, outstr);
      fputs (outstr, stdout);
      fputs (" lines", stdout);
      if (currline >= linenum - 1)
      {
            indx++;
            indx %= linenum;
      }
      else  indx = 0;

      if (fseek (fp, tail[indx], 0) == -1)
      {
            fputs ("\nFile seek error.", stderr);
            exit (1);
      }
      free (tail);
}

/*
**  long integer to ASCII.  Use this function rather than the ltoa()
**  library function to save many bytes in the executable file.
*/

void LtoA (long int val, unsigned char *cp)
{
      unsigned char tempc[15], *tcptr;
      int sign;
      static unsigned char dig[] = {"0123456789"};

      if (val < 0)
      {
            val = -val;
            sign = 1;
      }
      else  sign = 0;

      *(tcptr = tempc + 11) = '\0';
      do 
      {
            *--tcptr = dig[(int)(val % 10)];
      }while (val /= 10);
      if (sign)
            *--tcptr = '-';
      strcpy (cp, tcptr);
}

/*
**  Tell the user what the program is and how to use it.
*/

void help (void)
{
      char *ptr;
      static char help_str[] = { "Command error: No file(s) specified.\n"
            "Written by Joe Huffman.\n"
            "TAIL is a program that prints out only the last lines of a "
            "program.\nusage:\nTAIL filename [filename] [/n]\n\n"
            "filename - The name of a valid file, wildcards accepted.\n"
            "n - number of lines to print out, 5 by default."
            };

      for (ptr = &help_str[0]; *ptr; ptr++)
            fputc (*ptr, stdout);
}
