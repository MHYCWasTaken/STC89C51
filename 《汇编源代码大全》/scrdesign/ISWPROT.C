/*
**  ISWPROT.C - Detect if floppy drive is write protected
**
**  public domain by Bob Stout
*/

#include <dos.h>

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

/*
**  isWprot()
**
**  Parameters: 1 - Drive number (A: = 0, B: = 1)
**
**  Returns: -1 - Error
**            0 - Not write protected
**            1 write protected
**
**  Note: If drive door is open, an error is returned but the critical
**        error handler is NOT tripped
*/

int isWprot(int drive)
{
      union REGS regs;
      struct SREGS sregs;
      char buf[512], FAR *bufptr = (char FAR *)buf;   /* Needed by MSC  */

      /* First read sector 0  */

      segread(&sregs);
      regs.x.ax = 0x201;
      regs.x.cx = 1;
      regs.x.dx = drive & 0x7f;
      sregs.es  = FP_SEG(bufptr);
      regs.x.bx = FP_OFF(bufptr);
      int86x(0x13, &regs, &regs, &sregs);
      if (regs.x.cflag && regs.h.ah != 6)
            return -1;

      /* Try to write it back */

      segread(&sregs);
      regs.x.ax = 0x301;
      regs.x.cx = 1;
      regs.x.dx = drive & 0x7f;
      sregs.es  = FP_SEG(bufptr);
      regs.x.bx = FP_OFF(bufptr);
      int86x(0x13, &regs, &regs, &sregs);
      return (3 == regs.h.ah);
}

#ifdef TEST

#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
      int drive;

      if (2 > argc)
      {
            puts("Usage: ISWPROT drive_letter");
            return -1;
      }
      drive = toupper(argv[1][0]) - 'A';
      printf("isWprot(%c) returned %d\n", drive, isWprot(drive));
      return 0;
}

#endif
