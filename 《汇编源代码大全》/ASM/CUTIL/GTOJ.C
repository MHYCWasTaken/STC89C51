/*                   *** gtoj.c ***                                  */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* integer function that returns the julian date (1 = 1st day AD)    */
/* associated with a gregorian date in the form mm/dd/yy.  Returns a */
/* -1 if an error occured.                                           */
/*                                                                   */
/* Written by L. Cuthbertson, March 1983.                            */
/*                                                                   */
/*********************************************************************/
                 
#define CENTRY 19	/* current century */

long gtoj(indate)
char indate[];
{
	static int monthd[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	int leapd,iyr,imo,iday;
	static long cdays = 36524, ydays = 365;

	/* convert into expanded format if necessary */
	if ((cvtdate(indate)) != 0) return(-1);

	/* parse gregorian date into its pieces */
	sscanf(indate,"%2d%*1c%2d%*1c%2d",imo,iday,iyr);

	/* adjust month array for leap year/non-leap year */
	if (iyr < 0 || iyr > 99) return(-1);
	if (iyr%4 == 0 && iyr != 0 || CENTRY%4 == 0)
		monthd[1] = 29;
	else
		monthd[1] = 28;

	/* check for invalid month */
	if (imo < 1 || imo > 12) return(-1);

	/* check for invalid day */
	if (iday < 1 || iday > monthd[imo-1]) return(-1);

	/* determine the number of "extra" leap years caused by the  */
	/* %400 criteria and add to it the number of leap years that */
	/* has occured up to the prior year of current century.      */
	leapd = CENTRY/4;
	if (iyr != 0) leapd += (iyr-1)/4;

	/* determine number of days elapsed in current year */
	for (i=0;i<(imo-1);i++)
		iday = iday + monthd[i];

	/* calculate julian date */
	return (CENTRY*cdays + iyr*ydays + leapd + iday);
}
