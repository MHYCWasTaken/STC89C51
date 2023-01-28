/* handle internal errors */


/* global variable */
int brk_st;  /* orginal break status */

error(where,panic)
char *where;  /* call from what procedure */
int panic;  /* should I tell the user to panic? 0=no 1=yes */
{
  putn("MV stopped due to internal error in '", where, "'\n\015", 0);
  if (panic) {	/* this should never happen */
    putn("Because of this error, your disk directory might be in an\n\015",
	 "inconsistent state. Please run chkdsk on your disk to see\n\015",
	 "if this is so. Sorry.\n\015", 0);
  } else {  /* not to panic */
    putn("Do not worry, your disk is not screwed up, only that the\n\015",
	 "operation you requested was not completed.\n\015", 0);
  }
  putn("Please report this error to the author of this program. You can\n\015",
       "obtain his email address by typing 'mv' with no parameters.\n\015", 0);

  /* now restore break status to whatever the user set it to */
  bset(brk_st);
  exit(2);  /* indicate internal error */
}
