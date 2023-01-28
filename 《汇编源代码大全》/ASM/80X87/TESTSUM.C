/* testsum.c: Program to time assembler vs. C implementations of
 *	      array summation code.
 * Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.
 */

#include "testfpu.h"

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
  float *summe;
  int arrsize;

  if (argc != 3) {
    fprintf(stderr, "Usage: testsum #iters #elems\n");
    fprintf(stderr, "\t#iters = number of iterations of test\n");
    fprintf(stderr, "\t#elems = number of elements in arrays\n");
    exit(1);
  }
  else {
    char *sc;
    numreps = strtol(argv[1], &sc, 10);
    arrsize = atoi(argv[2]);
  }

  summe = (float *) malloc(arrsize * sizeof(float));
  for (i = 0; i < arrsize; i++)
    summe[i] = (double) rand() / (RAND_MAX / 2);

  printf("Calculating dead time...");
  gettime(&beg);
  for (i = 0; i < numreps; i++)
    ;
  gettime(&end);
  dead = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) dead/100);

  printf("Timing inline summation...");
  gettime(&beg);
  for (i = 0; i < numreps; i++) {
    double ret = 0;
    int j;
    for (j = 0; j < arrsize; j++)
      ret += summe[j];
  }
  gettime(&end);
  slow = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) slow/100);

  printf("Timing sumarray()...");
  gettime(&beg);
  for (i = 0; i < numreps; i++) {
    ret = sumarray(summe, arrsize);
  }
  gettime(&end);
  fast = diff_time(&beg, &end);
  printf("%.2f seconds\n", (float) fast/100);
  printf("sumarray() %d%% faster than inline code to do summation\n", percent_diff(dead, fast, slow));
  free(summe);
  return 0;
}
