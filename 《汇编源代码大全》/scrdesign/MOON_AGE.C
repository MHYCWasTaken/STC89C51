/* PD by Michelangelo Jones, 1:1/124. */

/*
**  Returns 0 for new moon, 14 for full moon,
**  27 for the day before new, and so forth.
*/

/*
**  This routine sometimes gets "off" by a few days,
**  but is self-correcting.
*/

int moon_age(int month, int day, int year)
{
      static short int ages[] =
            {18, 0, 11, 22, 3, 14, 25, 6, 17,
            28, 9, 20, 1, 12, 23, 4, 15, 26, 7
            };
      int row;

      if (day == 31)
            day = 1;
      switch (month)
      {
      case 1:
            row = day - 1;
            break;
      case 2:
      case 4:
            row = (day + 1) % 30;
            break;
      case 3:
            row = day % 30;
            break;
      case 5:
            row = (day + 2) % 30;
            break;
      case 6:
            row = (day + 3) % 30;
            break;
      case 7:
            row = (day + 4) % 30;
            break;
      case 8:
            row = (day + 5) % 30;
            break;
      case 9:
      case 10:
            row = (day + 7) % 30;
            break;
      case 11:
      case 12:
            row = (day + 9) % 30;
      }

      return ((ages[(year + 1) % 19] + row + (year < 1900)) % 30);

}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

static char *description[] = {
      "new",                  /* totally dark                         */
      "waxing crescent",      /* increasing to full & quarter light   */
      "in its first quarter", /* increasing to full & half light      */
      "waxing gibbous",       /* increasing to full & > than half     */
      "full",                 /* fully lighted                        */
      "waning gibbous",       /* decreasing from full & > than half   */
      "in its last quarter",  /* decreasing from full & half light    */
      "waning crescent"       /* decreasing from full & quarter light */
      };

int main(int argc, char *argv[])
{
      int month, day, year, phase;

      if (4 > argc)
      {
            puts("Usage: MOON_AGE month day year");
            return EXIT_FAILURE;
      }
      month = atoi(argv[1]);
      day   = atoi(argv[2]);
      year  = atoi(argv[3]);
      if (100 > year)
            year += 1900;
      printf("moon_age(%d, %d, %d) returned %d\n", month, day, year,
            phase = moon_age(month, day, year));
      printf("Moon phase on %d/%d/%d is %s\n", month, day, year,
            description[(int)((phase + 2) * 16L / 59L)]);
}

#endif /* TEST */
