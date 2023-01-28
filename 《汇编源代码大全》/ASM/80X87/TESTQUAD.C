/* testquad.c: Program to test the correctness of the quadratic-
 *	       solving routine in 80x87 assembler, and to illustrate
 *	       its use.
 * Copyright (C) 1991 by Nicholas Wilt.  All rights reserved.
 */

#include "testfpu.h"

void
main()
{
  char inp[256];	/* Array for input string */

  int done = 0; 	/* Termination var */
  double a, b, c;	/* Coefficients of quadratic */
  double x1, x2;	/* Roots of quadratic, if any */

  while (! done) {
    printf("a = ");    gets(inp);    a = atof(inp);
    printf("b = ");    gets(inp);    b = atof(inp);
    printf("c = ");    gets(inp);    c = atof(inp);
    if (solve_quadratic(a, b, c, &x1, &x2))
      printf("Roots are %.2f and %.2f\n", x1, x2);
    else
      printf("No real roots.\n");
    printf("Test another (Y/N)? ");
    gets(inp);
    done = ! (toupper(inp[0]) == 'Y');
  }
}
