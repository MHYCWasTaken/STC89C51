/* testpow.c: Program to time assembler vs. C implementations of
 *	      pow().
 * Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.
 */

#include "testfpu.h"

/* Number of repetitions to do */
#define NUM_REPS 1000L

/* Size of array to test sumarray() on */
#define ARRSIZE 2

int
main(int argc, char *argv[])
{
  struct time beg, end;
  double x;
  int y;
  double ret;
  long i;
  int j;
  long dead, slow, fast;
  long numreps;

  if (argc != 2) {
    fprintf(stderr, "Usage: testpow #iters\n");
    exit(1);
  }
  else {
    char *sc;
    numreps = strtol(argv[1], &sc, 10);
  }

  printf("Calculating dead time...");
  gettime(&beg);
  for (i = 0; i < numreps; i++) {
    x = (double) rand() / RAND_MAX;
    y = rand();
  }
  gettime(&end);
  dead = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) dead/100);

  printf("Timing pow()...");
  gettime(&beg);
  for (i = 0; i < numreps; i++) {
    x = (double) rand() / RAND_MAX;
    y = rand();
    ret = pow(x, y);
  }
  gettime(&end);
  slow = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) slow/100);

  printf("Timing intpow()...");
  gettime(&beg);
  for (i = 0; i < numreps; i++) {
    x = (double) rand() / RAND_MAX;
    y = rand();
    ret = intpow(x, y);
  }
  gettime(&end);
  fast = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) fast/100);
  printf("intpow() %d%% faster than pow()\n",
    percent_diff(dead, fast, slow));
  return 0;
}
