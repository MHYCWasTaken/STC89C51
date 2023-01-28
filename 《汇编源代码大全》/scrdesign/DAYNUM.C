/*
**  DAYNUM.C - Functions to return statistics about a given date.
**
**  public domain by Bob Stout - uses Ray Gardner's SCALDATE.C
*/

int isleap (unsigned yr);
long ymd_to_scalar (unsigned yr, unsigned mo, unsigned day);
static long jan1date;

/*
**  Define ISO to be 1 for ISO (Mon-Sun) calendars
**
**  ISO defines the first week with 4 or more days in it to be week #1.
*/

#ifndef ISO
 #define ISO 0
#endif

#if (ISO != 0 && ISO != 1)
 #error ISO must be set to either 0 or 1
#endif

/*
**  Determine if a given date is valid
*/

int valiDate(unsigned yr, unsigned mo, unsigned day)
{
      unsigned int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

      if (1 > mo || 12 < mo)
            return 0;
      if (1 > day || day > (days[mo - 1] + (2 == mo && isleap(yr))))
            return 0;
      else  return 1;
}

/*
**  Return the day of the year (1 - 365/6)
*/

int daynum(int year, int month, int day)
{
      jan1date = ymd_to_scalar(year, 1, 1);
      return (int)(ymd_to_scalar(year, month, day) - jan1date + 1L);
}

/*
**  Return the week of the year (1 - 52)
*/

int weeknum(int year, int month, int day)
{
      int wn, j1n, dn = daynum(year, month, day);

      dn += (j1n = (int)((jan1date - (long)ISO) % 7L)) - 1;
      wn = dn / 7;
      if (ISO)
            wn += (j1n < 4);
      else  ++wn;
      return wn;
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

void do_err(void);

void main(int argc, char *argv[])
{
      int day, month, year;

      if (4 > argc)
      {
            puts("Usage: DAYNUM month day year");
            return;
      }

      month = atoi(argv[1]);
      day   = atoi(argv[2]);
      year  = atoi(argv[3]);
      if (100 > year)
            year += 1900;

      printf("%d/%d/%d is day #%d in week %d\n", month, day, year,
            daynum(year, month, day), weeknum(year, month, day));
}

#endif /* TEST */
