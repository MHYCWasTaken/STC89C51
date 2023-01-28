/*                          *** readc.c ***                          */
/*                                                                   */
/* IBM-PC microsoft "C" under PC-DOS v2.0                            */
/*                                                                   */
/* Function to read a character to the screen using a DOS function.  */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */
int readc()
{
	int bdos();

	return(bdos(8));
}
