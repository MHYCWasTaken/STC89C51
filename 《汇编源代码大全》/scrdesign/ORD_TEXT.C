/*
**  Originally published as part of the MicroFirm Function Library
**
**  Copyright 1991, Robert B.Stout
**
**  Subset version with modifications suggested by Maynard Hogg
**  released to the public domain, 1992
**
**  Function to return ordinal text.
*/

static char *text[] = {"th", "st", "nd", "rd"};
static int index[]  = {0, 1, 2, 3, 0, 0, 0, 0, 0, 0};

char *ordinal_text(int number)
{
      if (((number %= 100) > 9 && number < 20) || (number %= 10) > 3)
            number = 0;
      return text[number];
}

#ifdef TEST

#include <stdio.h>

void main(void)
{
      int i;

      for (i = 0; i < 26; ++i)
            printf("%d%s\n", i, ordinal_text(i));
}

#endif /* TEST */
