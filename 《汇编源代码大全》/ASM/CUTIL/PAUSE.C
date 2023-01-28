/*                             *** pause.c ***                       */
/*                                                                   */
/* IBM-PC microsoft "C" under PC-DOS                                 */
/*                                                                   */
/* Function to cause a program to appear to pause for a given period */
/* time.  Uses the system clock for timing.  Accurate to 1/100's of  */
/* a second.                                                         */
/*                                                                   */
/* Written by L. Cuthbertson, May 1984.                              */
/*                                                                   */
/*********************************************************************/
/*                                                                   */

pause(seconds)
float seconds;			/* number of seconds to pause */
{
	int ih,im,is,iths;
	long current,offset,diff;

	/* determine the current time */
	dostime(&ih,&im,&is,&iths);

	/* calculate the current time in 1/100's of a second */
	offset = (ih*360000) + (im*6000) + (is*100) + iths;
	current = offset;

	/* calculate the number of 1/100's of a second to pause */
	diff = seconds*100;
	if (diff > 8640000) {		/* maximum of 1 day pause */
		return(-1);
	}

	/* keep on polling the clock until difference has elapsed */
	while ((current-offset) < diff) {
		dostime(&ih,&im,&is,&iths);
		current = (ih*360000) + (im*6000) + (is*100) + iths;
		if (current < offset) {		/* spans midnight */
			current += 8640000;
		}
	}

	/* done */
	return(0);

}
