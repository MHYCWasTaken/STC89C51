/*
**  FMEMOPS.C - Emulate MSC's far memory functions in BC++ & ZTC++
**
**  Original Copyright 1988-1992 by Bob Stout as part of
**  the MicroFirm Function Library (MFL)
**
**  This subset version is hereby donated to the public domain.
*/

#include <stdlib.h>

#if defined(__TURBOC__) || defined(__ZTC__)

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

void FAR * _fmemcpy(void FAR *dest, void FAR *src, size_t count)
{
      void FAR *target =  dest;

      for ( ; count; --count)
      {
            *dest++ = *src++;
      }
      return target;
}

void FAR * _fmemmove(void FAR *dest, void FAR *src, size_t count)
{
      void FAR *target =  dest;

      if (src >= dest)
            _fmemcpy(dest, src, count);
      else  for (dest += count, src  += count ;count ;--count)
                  *(--dest) = *(--src);
      return target;
}

void _fmemset(void FAR *dest, int ch, size_t count)
{
      void FAR *target =  dest;

      for ( ; count; --count)
            *(unsigned char FAR *)dest++ = (unsigned char) ch;
      return target;
}

#endif
