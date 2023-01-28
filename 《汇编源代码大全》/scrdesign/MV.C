/*
** mv.c -- move or rename files or directories  
** updated for multiple files, 5 jul 92, rlm    
** placed in the public domain via C_ECHO by the author, Ray McVay
**
** modified by Bob Stout, 28 Mar 93
*/

#include <stdio.h>
#include <string.h>
#include <dos.h>
 
/* For portability, make everything look like MSC 6 */
 
#if defined(__TURBOC__)
 #include <dir.h>
 #define _dos_findfirst(f,a,b) findfirst(f,b,a)
 #define find_t ffblk
 #define _A_SUBDIR FA_DIREC
 #define attrib ff_attrib
#else                   /* assume MSC/QC                                */
 #include <direct.h>
#endif

char target[FILENAME_MAX];
char *generr = "ERROR: mv - couldn't rename %s %s\n";

void help(char *s)
{
      puts("usage: mv <oldname [...]> <newname|newdir>");
      printf("error: %s\n", s);
}

isdir(char *path)
{
      struct  find_t f;

      return (_dos_findfirst(path, _A_SUBDIR, &f) == 0 &&
            (f.attrib & _A_SUBDIR));
}

main(int argc, char **argv)
{
      int src;

      puts("mv 1.2 (28 mar 93) - Ray L. McVay/Bob Stout");
      if (argc < 3)
            help("Not enough parameters");
      else if (isdir(argv[argc -1]))
      {
            for (src = 1; src < argc - 1; src++)
            {
                  strcpy(target, argv[argc - 1]);
                  strcat(target, "\\");
                  if (strrchr(argv[src], '\\'))
                        strcat(target, strrchr(argv[src], '\\') + 1);
                  else if (argv[src][1] == ':')
                        strcat(target, argv[src] + 2);
                  else  strcat(target, argv[src]);
                  if (rename(argv[src], target))
                        printf(generr, argv[src], target);
            }
      }
      else if (argc == 3)
      {
            if (rename(argv[1], argv[2]))
                  printf(generr, argv[1], argv[2]);
      }
      return 0;
}
