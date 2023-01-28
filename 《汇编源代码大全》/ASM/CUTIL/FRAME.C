/*                             *** frame.c ***                       */
/*                                                                   */
/* IBM-PC microsoft "C" under PC-DOS                                 */
/*                                                                   */
/* Function to draw a box given the upper left corner and the lower  */
/* right corner.  Uses the extended character set - graphics board   */
/* not needed.  Uses direct BIOS calls.  Returns a 0 if successful   */
/* or a -1 if invalid parameters.                                    */
/*                                                                   */
/* *** NOTE ***                                                      */
/* The upper left corner of the screen is 0,0 and the bottom right   */
/* corner is 24,79.                                                  */
/*                                                                   */
/* Written by L. Cuthbertson, May 1984.                              */
/*                                                                   */
/*********************************************************************/
/*                                                                   */
int frame(ulrow,ulcol,lrrow,lrcol)
int ulrow,ulcol,lrrow,lrcol;
{
	static int horbar = 0xC4, verbar = 0xB3;
	static int ulcorn = 0xDA, urcorn = 0xBF;
	static int llcorn = 0xC0, lrcorn = 0xD9;
	int count,irow;

	/* error checking */
	if (ulrow < 0 || ulrow > 24) return(-1);
	if (ulcol < 0 || ulcol > 79) return(-1);
	if (lrrow < 0 || lrrow > 24) return(-1);
	if (lrcol < 0 || lrcol > 79) return(-1);
	if (lrrow < ulrow) return(-1);
	if (lrcol < ulcol) return(-1);

	/* do top line first */
	biosset(ulrow,ulcol);
	bioswc(ulcorn,1);
	count = lrcol-ulcol-1;
	if (count > 0) {
		biosset(ulrow,ulcol+1);
		bioswc(horbar,count);
	}
	biosset(ulrow,lrcol);
	bioswc(urcorn,1);

	/* do both sides at once */
	irow = ulrow + 1;
	while (irow < lrrow) {
		biosset(irow,ulcol);
		bioswc(verbar,1);
		biosset(irow,lrcol);
		bioswc(verbar,1);
		irow++;
	}

	/* do bottom line */
	biosset(lrrow,ulcol);
	bioswc(llcorn,1);
	count = lrcol-ulcol-1;
	if (count > 0) {
		biosset(lrrow,ulcol+1);
		bioswc(horbar,count);
	}
	biosset(lrrow,lrcol);
	bioswc(lrcorn,1);

	/* done */
	return(0);
}
