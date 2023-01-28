/*
**  demo code for converting Pascal string to C strings
**
**  public domain by Bob Stout
*/

#include <string.h>

#define P2Cconvert(s) {int n = *(s); memmove((s), &(s)[1], n); s[n] = '\0';}
#define C2Pconvert(s) {int n = strlen(s); memmove(&(s)[1], (s), n); *(s) = n;}

        char string[81];

        fgets(string, 81, inFile);   /* get 80-char pascal string */
        P2Cconvert(string);          /* convert it in place       */
        C2Pconvert(string);          /* convert back              */
