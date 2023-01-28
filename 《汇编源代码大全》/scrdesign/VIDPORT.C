/*
**  Portable PC screen functions
**  Public domain by Bob Stout
**  Uses SCROLL.C, also from SNIPPETS
*/
 
#include <stdio.h>
#include <dos.h>
#include "scrnmacs.h"         /* Also in SNIPPETS     */

void scroll(int, int, int, int, int, int, int);
 
#define SCROLL_UP 0
#define SCROLL_DN 1

void GotoXY(int col, int row)
{
      union REGS regs;

      setbuf(stdout, NULL);
      regs.h.dh = (unsigned)row;
      regs.h.dl = (unsigned)col;
      regs.h.bh = VIDPAGE;
      regs.h.ah = 2;
      int86(0x10, &regs, &regs);
}

void ClrScrn(int vattrib)
{
      union REGS regs;
 
      scroll(SCROLL_UP, 0, vattrib, 0, 0, SCREENROWS, SCREENCOLS);
      GotoXY(0, 0);                     /* Home cursor  */
}
