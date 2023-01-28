/* testfpu.h: header file for testpow.c, testsum.c, and testquad.c
 * Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.
 */

/* This file contains external declarations and definitions of a
 * few functions common to the three .c files.
 *
 * Function definitions don't normally belong in include files,
 * but this header doesn't include anything intrinsic to the
 * material demonstrated by the programs.
 */

#include <math.h>
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

/* External declarations of the assembler functions. */
extern double intpow(double x, unsigned int y);
extern double sumarray(float *arr, int n);
extern int solve_quadratic(double a, double b, double c,
			   double *x1, double *x2);

/* Routine to calculate the number of hundredths of seconds in
   a given time structure. */
long
num_hunds(struct time *a)
{
  long hrs, mins, secs;
  hrs = a->ti_hour * 360000L;
  mins = a->ti_min * 6000L;
  secs = a->ti_sec * 100L;
  return(hrs + mins + secs + (long) a->ti_hund);
}

/* Routine to calculate the number of hundredths of seconds
   between two given time structures. */
long
diff_time(struct time *beg, struct time *end)
{
  return(num_hunds(end) - num_hunds(beg));
}

/*  Returns % faster that the "fast"
 *  time is compared to the "slow."
 */
int
percent_diff(long dead, long fast, long slow)
{
  double diff = (double) (slow - dead) / (fast - dead);
  return (int) (diff*100.0 - 100.0);
}
