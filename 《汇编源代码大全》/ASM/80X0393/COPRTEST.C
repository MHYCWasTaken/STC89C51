/*
**  coprtest.c
**  By: Doug Mansell
**  Re: VGA palette changing
**  Link copper.asm with coprtest.c for a little demo...
*/

#include <math.h>

#define SCREENLINES 400             /* suitable for 80x25 text */
#define PALSIZE SCREENLINES+1

struct _rgb 
{
  unsigned char red, green, blue;
} cols[PALSIZE];

void pascal copper(int lines, unsigned char DACnum,
                   void *colours, int count);

void main()
{
  int loop;
  double x, phase;

  /* construct a pretty rainbow colour palette */
  phase = 2 * M_PI / 3;

  for (loop = 0; loop < PALSIZE; loop++) 
  {
     x = 2 * M_PI * loop / PALSIZE;
     cols[loop].red   = 32 * sin(x) + 31;
     cols[loop].green = 32 * sin(x + phase) + 31;
     cols[loop].blue  = 32 * sin(x + 2*phase) + 31;
  }

  /* hold the palette steady for 500 screen refreshes :-) */
  copper(SCREENLINES, 0 ,cols, 500);
  puts("press a key to roll it up the screen"); 
  getch();
  copper(SCREENLINES - 1, 0, cols, 500);
} 
