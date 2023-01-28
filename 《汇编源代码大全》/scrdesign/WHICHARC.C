/*
   --------------------------------------------------------------------
   Module:     WHICHARC.C
   Subject:    tries to determine the archiver used to compress files
   Author:     Heinz Ozwirk
               modified for SNIPPETS by Bob Stout
   Status:     public domain
   Started:    28.09.1991   13:35:57
   Modified:   13.10.1991   14:15:57
   --------------------------------------------------------------------
   Prototype:  int WhichArc(char *pName)
      pName    address of full path name of file to examine
      Result   -1:      file not found
               UNKNOWN: unknown packer
               ARC:     ARC or PKARC
               ARJ:     ARJ
               LHA:     LHARC or LHA
               ZIP:     PKZIP
               ZOO:     Zoo

   LHARC/LHA
      No archive header. WhichArc examines the checksum of the first
      file header. If the checksum is valid and if the string -lh?-
      is found, LHA or LHARC is assumed.

   ARJ
      If a file starts with 0x60, 0xEA, ARJ is assumed.

   ZIP
      If the file begins with "PK", PKZIP is assumed.

   ZOO
      Zoo'ed archives always start with "ZOO x.xx Archive". WhichArc
      only looks for "ZOO".

   ARC
      No header. Files starting with 0x1A are assumed to be ARCed.
   --------------------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>

typedef unsigned char BYTE;

#define strNcmp(s1,s2,n) !strncmp((const char *)(s1), (s2), (n))

enum {UNKNOWN, ARC, ZOO, ARJ, LHARC, LHA, ZIP}

WhichArc(char *pName)
{
      FILE  *fp;
      BYTE  header[128];
      int   c, i, n;

      memset(header, 0, sizeof(header));
      fp = fopen(pName, "rb");
      if (fp == NULL)
            return -1;
      n = fread(header, sizeof(BYTE), sizeof(header) - sizeof(BYTE), fp);
      fclose(fp);

      if (n <= 0)
            return -1;

      if (n >= 7 && n >= header[0] + 2)
      {
            for (c = 0, i = header[0]; i--; c += (header+2)[i])
                  ;
            if ((BYTE)(c & 0x00FF) == header[1]       &&
                  strNcmp(&header[2], "-lh", 3) &&
                  '-' == header[6])
            {
                  return (header[5] > '1') ? LHA : LHARC;
            }
      }

      if (n >= 2)
      {
            if (strNcmp(header, "\x60\xEA", 2))
                  return ARJ;
            if (strNcmp(header, "PK", 2))
                  return ZIP;
      }

      if (n >= 3 && strNcmp(header, "ZOO", 3))
            return ZOO;

      if (n >= 25 && *header == 0x1A) return ARC;

      return UNKNOWN;
}
