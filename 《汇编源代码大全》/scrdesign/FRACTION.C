/*
**  FRACTION.C - Convert a floating point value to an integer ratio
**
**  public domain by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char n, d;

double real2ratio(double Realnum)
{
      int i, Trial, Seed = 255;
      double Residue, Minres = 255.;

      if (1. == Realnum || 0. == Realnum)
      {
            n = (unsigned char) Realnum;
            d = (unsigned char) 1;
            return Realnum;
      }
      if (1.0 < Realnum)
      {
            Seed = (int)(255./Realnum);
            if (255 % (int)Realnum)
                  ++Seed;
      }
      for (i = Seed; i; --i)
      {
            Trial = (int)((Realnum * (double)i) + 0.5);
            if (255 < Trial)
                  continue;
            Residue = Realnum - ((double)Trial/(double)i);
            if (0. > Residue)
                  Residue = 0. - Residue;
            if (Residue < Minres)
            {
                  d = (unsigned char) i;
                  n = (unsigned char) Trial;
                  Minres = Residue;
            }
      }
      return ((double)n / (double)d);
}

void blow_off(void)
{
      puts("\aUsage:   FRACTION decimal_fraction");
      puts("returns: decimal_fraction = integer / integer");
}

void range_error(double err_num, char *string)
{
      blow_off();
      printf("\ndecimal_fraction must be %G or %s.\n", err_num, string);
      exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
      double Fraction, result;

      if (argc < 2)
      {
            blow_off();
            return EXIT_FAILURE;
      }
      Fraction = atof(argv[1]);
      if (255. < Fraction)
            range_error((255.), "less");
      if ((1./255.) > Fraction && Fraction != 0.)
            range_error((1./255.), "greater");
      result = real2ratio(Fraction);
      printf("\nClosest integer fraction approximating %G is:\n", Fraction);
      printf("%d / %d (%Xh / %Xh) = %G", n, d, n, d, result);
      printf(" (Error = %6.3G%%)\n", 100. * (Fraction - result) / Fraction);
      return EXIT_SUCCESS;
}
