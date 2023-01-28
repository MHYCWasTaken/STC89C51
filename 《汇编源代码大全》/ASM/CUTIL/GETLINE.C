/*                       *** getline.c ***                           */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Function to read a record from a file into a string.  Returns the */
/* length of the string including the NULL, an EOF if EOF is reached,*/
/* or a -1 if an error occured.                                      */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#include <stdio.h>

int getline(fp,line,len)
char line[];
int len;
FILE *fp;
{
	int c,inlen;

	/* read character at a time */
	for(inlen=0;(((c=getc(fp)) != '\r') && (c != '\n'));inlen++) {

		/* error checking */
		if (inlen >= len) return (-1);

		/* EOF check */
		if (c == EOF) {
			line[inlen] = NULL;
			return(EOF);
		}

		/* move character into line */
		line[inlen] = c;
	}

	/* replace CR or NL with a NULL */
	line[inlen] = NULL;

	/* done */
	return(inlen+1);
}
