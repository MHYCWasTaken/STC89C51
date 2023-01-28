/*
**  random.c -- "Minimal Standard" integer random number generator
**
**  based on "Random Number Generators: Good Ones Are Hard to Find",
**  S.K. Park and K.W. Miller, Communications of the ACM 31:10 (Oct 1988).
**
**  linear congruential generator f(z) = 16807 z mod (2 ** 31 - 1)
**
**  uses L. Schrage's method to avoid overflow problems
**
**  adapted for C by R. Gardner, public domain
*/

static long int z = 1;

#define a 16807
#define m 2147483647                                  /* 2 ** 31 - 1 */
#define q (m / a)
#define r (m % a)

long random (void)                  /* returns number in [1 .. 2147483646] */
{
      long int lo, hi, test;

      hi = z / q;
      lo = z % q;
      test = a * lo - r * hi;
      if (test > 0)
            z = test;
      else  z = test + m;
      return z;
}

void srandom(unsigned int seed)                       /* set the seed */
{
      if (seed == 0)
            seed = 1;
      z = seed;
}
