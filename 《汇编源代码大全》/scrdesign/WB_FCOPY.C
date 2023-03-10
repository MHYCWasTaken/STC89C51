/* 
** by: Walter Bright via Usenet C newsgroup
**
** modified by: Bob Stout based on a recommendation by Ray Gardner
**
** modified by: David Gersic to deal with binary files
**
** There is no point in going to asm to get high speed file copies. Since it
** is inherently disk-bound, there is no sense (unless tiny code size is
** the goal). Here's a C version that you'll find is as fast as any asm code
** for files larger than a few bytes (the trick is to use large disk buffers):
*/

#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#if !defined(__ZTC__) && !defined(__TURBOC__)
 #include <sys\types.h>
#endif

#include <sys\stat.h>

#if Afilecopy
int file_copy(from,to)
#else
int file_append(from,to)
#endif
char *from,*to;
{       int fdfrom,fdto;
        int bufsiz;

        fdfrom = open(from,O_RDONLY|O_BINARY,0);
        if (fdfrom < 0)
                return 1;
#if Afileappe
        /* Open R/W by owner, R by everyone else        */
        fdto=open(to,O_BINARY|O_CREAT|O_APPEND|O_RDWR,S_IREAD|S_IWRITE);
        if (fdto < 0)
            goto err;
#else
        fdto=open(to,O_BINARY|O_CREAT|O_TRUNC|O_RDWR,S_IREAD|S_IWRITE);
        if (fdto < 0)
            goto err;
#endif

        /* Use the largest buffer we can get    */
        for (bufsiz = 0x4000; bufsiz >= 128; bufsiz >>= 1)
        {   register char *buffer;

            buffer = (char *) malloc(bufsiz);
            if (buffer)
            {   while (1)
                {   register int n;

                    n = read(fdfrom,buffer,bufsiz);
                    if (n == -1)                /* if error             */
                        break;
                    if (n == 0)                 /* if end of file       */
                    {   free(buffer);
                        close(fdto);
                        close(fdfrom);
                        return 0;               /* success              */
                    }
                    if (n != write(fdto,buffer,(unsigned) n))
                        break;
                }
                free(buffer);
                break;
            }
        }
err2:   close(fdto);
        remove(to);                             /* delete any partial file */
err:    close(fdfrom);
        return 1;
}
