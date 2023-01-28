/*                          *** writec.c ***                        */
/*                                                                   */
/* IBM-PC microsoft "C" under PC-DOS v2.0                            */
/*                                                                   */
/* Function to write a character to the screen using a DOS function. */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */
int writec(c)
char c;
{
	int iret,bdos();

	iret = bdos(2,c);
	return (iret);
}
