/*                         *** scratt.c ***                          */
/*                                                                   */
/* IBM - PC microsoft "C" under PC-DOS                               */
/*                                                                   */
/* Function to turn on and off in a toggle switch manner various     */
/* screen attributes such as BOLD, blink, reverse video, etc.        */
/* Returns a 0 if successful or a -1 if not.                         */
/*                                                                   */
/* Written by L. Cuthbertson, April 1984.                            */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

#define NULL	'\000'
#define POUND	'#'

int scratt(iatt)
int iatt;
{
	extern char SGR[];
	char catt[3],command[20];
	static int switches[9] = {0,0,0,0,0,0,0,0,0};
	int i,j,inpos,outpos;
	int savpos,gotone;

	/* error check */
	if (iatt<0 || iatt>8)
		return(-1);

	/* toggle selected switch */
	if (iatt == 0) {
		for(i=0;i<9;i++) {
			switches[i] = 0;
		}
	} else {
		switches[iatt] = !(switches[iatt]);
	}

	/* initialize screen controls */
	scrinit();

	/* turn all attributes off ***********************************/
	catt[0] = '0';		/* turn all attributes off */
	catt[1] = NULL;

	/* build control sequence */
	inpos = 0;	/* position in control line */
	outpos = 0;	/* position in command line */

	while (SGR[inpos] != POUND)
		command[outpos++] = SGR[inpos++];

	for (i=0;catt[i] != NULL;i++)
		command[outpos++] = catt[i];

	inpos++;
	while(SGR[inpos] != NULL)
		command[outpos++] = SGR[inpos++];

	command[outpos] = NULL;

	/* write command to screen */
	writes(command);

	/* turn selected attibutes on ********************************/
	if (iatt != 0) {
		gotone = 0;

		/* loop through switches */
		for(j=1;j<9;j++) {
			if (switches[j] != 0) {

				/* got an attribute */
				gotone++;
				inpos = 0;
				outpos = 0;
				sprintf(catt,"%d",j);

				while (SGR[inpos] != POUND)
					command[outpos++] = SGR[inpos++];

				for (i=0;catt[i] != NULL;i++)
					command[outpos++] = catt[i];

				inpos++;
				while(SGR[inpos] != NULL)
					command[outpos++] = SGR[inpos++];

				command[outpos] = NULL;
				writes(command);
			}
		}
	}

	return(0);
}
