/*
**  PRTSC.C - Access the BIOS print screen function
**
**  public domain demo by Bob Stout
*/

#include <dos.h>

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

/*
**  Get screen printing status
**
**  0 - Ready
**  1 - Screen printing in process
**  2 - Error occurred last time
*/

int PrtScrnStat(void)
{
      return ((int)*((char FAR *)(0x00500000)));
}

/*
**  Print the current screen
*/

int PrtScrn(void)
{

      union REGS regs;                    /* Dummy for use by int86()   */

      if (1 == PrtScrnStat())             /* Can we print now?          */
            return -1;                    /* Nope, return with error    */
      int86(5, &regs, &regs);             /* Issue Int 5                */
      return 0;
}

#ifdef TEST

#include <stdio.h>

void main(void)
{
      printf("PrtScrn() returned %d\n", PrtScrn());
      printf("PrtScrnStat() returned %d\n", PrtScrnStat());
}

#endif
