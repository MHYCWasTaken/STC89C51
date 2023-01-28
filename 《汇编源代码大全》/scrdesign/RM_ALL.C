/*
**  Remove all files and (optionally) subdirectories
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <ctype.h>

#define LAST_CHAR(str) (str[strlen(str) - 1])
#define MAX_PATH 80

#ifdef __TURBOC__
 #include <dir.h>
 #define find_1st(n,a,b) (findfirst((n),(b),(a)))
 #define find_nxt(b) (findnext(b))
 #define find_t ffblk
 #define name ff_name
 #define attrib ff_attrib
 #define _A_SUBDIR FA_DIREC
#else
 #include <direct.h>
 #define find_1st(n,a,b) (_dos_findfirst((n),(a),(b)))
 #define find_nxt(b) (_dos_findnext(b))
#endif

/* Select one of the following - remove() is ANSI       */

#define rmfunc remove
/* #define rmfunc unlink */

#define show(s) fputs((s), stderr)

typedef enum {ERROR = -1, SUCCESS, FALSE = 0, TRUE} LOGICAL;
LOGICAL recurse = FALSE, gobble = FALSE;

char *mask = "*.*";

/*
**  Clean all files from a directory
*/

void clean_dir(char *path)
{
      char rmpath[MAX_PATH], *rmfile;
      struct find_t fbuf;

      strcpy(rmpath, path);
      if ('\\' != LAST_CHAR(rmpath))
            strcat(rmpath, "\\");
      rmfile = &rmpath[strlen(rmpath)];
      strcpy(rmfile, mask);
      if (0 == find_1st(rmpath, 0, &fbuf)) do
      {
            strcpy(rmfile, fbuf.name);
            rmfunc(rmpath);
            printf("deleting %s\n", rmpath);
      } while (0 == find_nxt(&fbuf));
}

/*
**  Process directories
*/

void do_dir(char *path)
{
      char search[MAX_PATH], new[MAX_PATH];
      struct find_t ff;

      strcpy(search, path);
      if ('\\' != LAST_CHAR(search))
            strcat(search, "\\");
      strcat(search, "*.*");
      if (SUCCESS == find_1st(search, 0xff, &ff)) do
      {
            if (ff.attrib & _A_SUBDIR && '.' != *ff.name)
            {
                  strcpy(new, path);
                  if ('\\' != LAST_CHAR(new))
                        strcat(new, "\\");
                  strcat(new, ff.name);
                  do_dir(new);
            }
      } while (SUCCESS == find_nxt(&ff));
      clean_dir(path);
      if (gobble)
            rmdir(path);
}

/*
**  Tell 'em they messed up
*/

void usage(LOGICAL errstat)
{
      if (errstat)
            fputc('\a', stderr);
      show("Usage: RM_ALL directory [...directory] [-eFNAME.EXT] [-rg?]\n");
      show("switches: -eFNAME.EXT  Remove only files matching mask "
            "(default is \"-e*.*\")\n");
      show("          -r           Recurse subdirectories\n");
      show("          -g           Gobble (delete) empty subdirectories\n");
      show("          -?           Display help (this message)\n");
      exit(errstat);
}

/*
**  RM_ALL - Deletes all files and (optionally) subdirectories
*/

int main(int argc, char *argv[])
{
      char rmpath[MAX_PATH], *rmfile;
      int i;
      LOGICAL found_dir = FALSE;
      void (*clean_func)(char *) = clean_dir;

      for (i = 1; i < argc; ++i)          /* Check for switches         */
      {
            if (NULL == strchr("-/", *argv[i]))
                  continue;               /* Assume it's a filename     */
            switch (toupper(argv[i][1]))
            {
            case 'R':
                  clean_func = do_dir;
                  break;

            case 'G':
                  gobble = TRUE;
                  break;

            case '?':
                  usage(FALSE);
                  break;

            case 'E':
                  if (0 == strlen(&argv[i][2]))
                        usage(ERROR);
                  else  mask = strupr(&argv[i][2]);
                  break;

            default:
                  usage(ERROR);
            }
      }
      for (i = 1; i < argc; ++i)          /* Scan filenames             */
      {
            if (strchr("/-", *argv[i]))
                  continue;
            found_dir = TRUE;
            clean_func(argv[i]);
      }
      if (!found_dir)
            usage(TRUE);
      else  return 0;
}
