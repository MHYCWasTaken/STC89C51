/*                          *** reads.c ***                          */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Function to read a string from stdin.  Returns the number of      */
/* characters entered including the NULL byte.                       */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#define NULL '\000'
#define CONC '\003'
#define BELL '\007'
#define CONX '\030'
#define ESC '\033'
#define DEL '\177'

int reads(s,maxlen)
char s[];
int maxlen;
{
	int len,c,readc(),writec();

	/* acceptance loop */
	for (len=1;((c=readc()) != '\r') && (c != '\n');len++) {

		/* handle backspaces and deletes */
		if ((c == '\b') || (c == DEL)) {
			if (len > 1) {
				writec('\b');
				writec(' ');
				writec('\b');
				len -= 2;
			} else {
				len = 0;
			}

		/* handle escape and control-z */
		} else if ((c == ESC) || (c == CONX) || (c < 0)) {
			while (len > 1) {
				writec('\b');
				writec(' ');
				writec('\b');
				len--;
			}
			len = 0;

		/* handle control-c */
		} else if (c == CONC) {
			exit();

		/* ring their bell if length exceeded */
		} else if (len >= maxlen) {
			writec (BELL);
			len--;

		/* handle strange control characters */
		} else if (c < ' ') {
			writec (BELL);
			len--;

		/* got a valid character */
		} else {
			s[len-1] = c;
			writec(c);
		}

	}

	/* add string terminator */
	s[len-1] = NULL;

	/* return total length of string entered */
	return (len);
}
