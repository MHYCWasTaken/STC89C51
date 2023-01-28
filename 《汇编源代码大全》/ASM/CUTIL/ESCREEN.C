/*                        *** escreen.c ***                           */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Function to erase a line of the screen.                           */
/*                                                                   */
/*  0 = Erase form cursor to end of screen.                          */
/*  1 = Erase from start of screen to cursor.                        */
/*  2 = Erase entire screen.                                         */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#define NULL	'\000'
#define POUND	'#'

escreen(y)
int y;
{
	extern char ED[];
	char yrel[2],command[20];
	int i,inpos,outpos;

	/* initialize screen controls */
	scrinit();

	/* error check */
	if (y < 0 || y > 2)
		y = 2;

	/* convert integer relative motion into string */
	sprintf(yrel,"%d",y);

	/* build command line */
	inpos = 0;	/* position within control string */
	outpos = 0;	/* position within command string */

	while (ED[inpos] != POUND)
		command[outpos++] = ED[inpos++];

	for (i=0;yrel[i] != NULL;i++)
		command[outpos++] = yrel[i];

	inpos++;
	while (ED[inpos] != NULL)
		command[outpos++] = ED[inpos++];

	command[outpos] = NULL;

	/* write command to screen */
	writes(command);
}
