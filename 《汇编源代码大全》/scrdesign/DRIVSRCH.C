/*
**  DRIVSRCH.C - public domain by Marty Connelly, Victoria, BC 1992
**  
**  Routine checks how many valid disk drives are available on machine,
**  both physical and logical drives
*/

/*
  Includes drive letters assigned with DOS SUBST command
  Networked drives are left as an exercise as I don't have access
  to them to check.
  The routine uses undocumented DOS interrupt 32H.

 Compiled Under TC LARGE memory Model
 Compatible with MSC 5 and 6, ZTC++, other DOS compilers

   DS:BX contains the address of a Disk Info Block for a requested
 drive. This is useful for applications which perform sector_level
 access of disk drives supported by device drivers. Some disk drives
 (especially non-bootable drives) might not contain valid information
 in the boot record and partion table making it hard to locate the
 root directory size or number of FATs. This info is contained in the
 Disk Info Block Structure
*/

#include <stdio.h>
#include <dos.h>

union REGS inregs,outregs;

void main(void)
{
      int i;
      int unsigned result;
      int drivestatus[27];

      char drive [27] ={'0','A','B','C','D','E','F','G','H',
                        'l','J','K','L','M','N','O','P','Q',
                        'R','S','T','U','V','W','X','Y','Z'};

      /* routine checks for all valid drive possibilities from A to Z  */

      /* 0 indicates current drive */

      /*
            if removeable media drive ie. floppy drive A: has a latch door
            open you will get "Abort Retry" panic message
      */

      for (i = 0; i <= 26; i++)
      {
            /* drive number (0=default, 1=A, 2=B,etc.)*/
            inregs.h.dl = (unsigned)i;

            inregs.h.ah=0x32; /* DOS interrupt 32H */
                              /* was undocumented for DOS release 3.2*/
            intdos(&inregs,&outregs);

            result=outregs.h.al;

            /* result =0  then valid drive
                      =55 then  DOS comand SUBST used on drive
                          such as  SUBST P: C:\DOS\TEST
                      =255 or ff hex then invalid or non-existent drive
                      =??? networked drive
            */
            drivestatus[i]=result;
      }

      for (i = 1; i <= 26; i = i + 2)
      {
            printf("drive %c: status code =%d drive %c: status code =%d\r\n",
                  drive[i],drivestatus[i],drive[i+1],drivestatus[i+1]);
      }
      return;
}
