/*                          *** writes.c ***                        */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Function to write a string to stdout.                             */
/* Returns the number of characters written including the NULL.      */
/*                                                                   */
/* written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#define NULL '\000'

int writes(s)
char s[];
{
	int i,writec();

	for (i=0;(s[i] != NULL);i++)
		writec(s[i]);

	return (i);
}
