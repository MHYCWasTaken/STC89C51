/* routines to read break status and set break status (to on or off) */

#include <dos.h>

bstat()  /* get break status 0=off, 1=on */
{
  union REGS inregs, outregs;

  inregs.h.al = 0;  /* request break status */
  inregs.h.ah = 0x33;  /* DOS function for ctrl-break check */
  intdos(&inregs, &outregs);
  return outregs.h.dl;
}

bset(onoff)  /* set break status 0=off 1=on */
{
  union REGS inregs, outregs;

  inregs.h.al = 1;  /* set break status */
  inregs.h.ah = 0x33;  /* DOS function for ctrl-break check */
  inregs.h.dl = onoff;	/* set to on or off */
  intdos(&inregs, &outregs);
}
