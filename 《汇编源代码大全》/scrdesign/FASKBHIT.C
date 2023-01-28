/*
**  by David Goodenough & Bob Stout
*/

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

static int FAR * const bios = (int FAR *) 0x00400000l;

int fast_kbhit(void)
{
      return(bios[0x1a] - bios[0x1c]);
}

void fast_kbflush(void)
{
      bios[0x1a] = bios[0x1c];
}
