/*                        *** eline.c ***                            */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Function to erase a line of the screen.                           */
/*                                                                   */
/*  0 = Erase from cursor to end of line.                            */
/*  1 = Erase from start of screen to cursor.                        */
/*  2 = Erase entire line containing cursor.                         */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#define NULL	'\000'
#define POUND	'#'

eline(y)
int y;
{
	extern char EL[];
	char yrel[2],command[20];
	int i,inpos,outpos;

	/* initialize screen controls */
	scrinit();

	/* error check */
	if (y < 0 || y > 3)
		y = 0;

	/* convert integer relative motion into string */
	sprintf(yrel,"%d",y);

	/* build command line */
	inpos = 0;	/* position within control string */
	outpos = 0;	/* position within command string */

	while (EL[inpos] != POUND)
		command[outpos++] = EL[inpos++];

	for (i=0;yrel[i] != NULL;i++)
		command[outpos++] = yrel[i];

	inpos++;
	while (EL[inpos] != NULL)
		command[outpos++] = EL[inpos++];

	command[outpos] = NULL;

	/* write command to screen */
	writes(command);
}
