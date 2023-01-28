/*
**  RFIND1ST.C - Our own non-compiler specific find first/next calls
**
**  Original Copyright 1988-1991 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  This subset version is functionally identical to the
**  version originally published by the author in Tech Specialist
**  magazine and is hereby donated to the public domain.
*/

#include        <stdio.h>
#include        <stdlib.h>
#include        <dos.h>
#include        "dirent.h"

/************************************************************************/
/*                                                                      */
/* rfind_1st() - Find first matching file                               */
/*                                                                      */
/* Parameters: 1 - Drive, path and filename of file to be found. May    */
/*                 include wildcards                                    */
/*             2 - Attribute of file to search for. Attributes are      */
/*                 described in the MS-DOS manual. The search strategy  */
/*                 is described under DOS call 0x4Eh.                   */
/*             3 - Disk transfer area buffer. If NULL, one will be      */
/*                 malloc'ed.                                           */
/* Returns: Pointer to a struct DSTRUCT. If error, NULL is returned and */
/*          _doserrno is set to the error #.                            */
/*                                                                      */
/************************************************************************/

struct DSTRUCT *rfind_1st(char *name, unsigned attribute, struct DSTRUCT *dta)
{
      struct DSTRUCT *my_dta;
      union REGS regs;

      if (NULL == dta)
            my_dta = (struct DSTRUCT *)malloc((int)sizeof(struct DSTRUCT));
      else  my_dta = dta;

      bdos(0x1A, (unsigned)my_dta, 0);    /* set DTA to my_dta          */
      regs.x.ax = 0x4E00;                 /* find first                 */
      regs.x.dx = (unsigned)name;
      regs.x.cx = attribute;
      intdos(&regs, &regs);
      if (regs.x.cflag)                   /* if error                   */
      {
            _doserrno = regs.x.ax;
            if (NULL == dta && my_dta != NULL)
                  free(my_dta);
            return (struct DSTRUCT *) NULL;
      }
      return my_dta;
}

/************************************************************************/
/*                                                                      */
/* rfind_nxt() - Find next matching file                                */
/*                                                                      */
/* Parameters: 1 - Pointer to DSTRUCT structure to use                  */
/*                                                                      */
/* Returns: Pointer to struct DSTRUCT,                                  */
/*          NULL if no more matching files found                        */
/*                                                                      */
/************************************************************************/

struct DSTRUCT *rfind_nxt(struct DSTRUCT *dta)
{
      union REGS regs;

      bdos(0x1A, (unsigned)dta, 0);       /* set DTA to dta             */
      regs.x.ax = 0x4F00;
      intdos(&regs,&regs);
      if (regs.x.cflag)                   /* if error                   */
      {
            _doserrno = regs.x.ax;
            return (struct DSTRUCT *) NULL;
      }
      return dta;
}
