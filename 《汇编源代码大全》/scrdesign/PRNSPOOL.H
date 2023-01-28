/* prnspool.h 12-22-91 Robert Mashlan, public domain */
/* print spooler interface functions header file     */

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

#ifndef MK_FP
 #define MK_FP(seg,offset) \
        ((void _far *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))
#endif

struct PACKET {
	unsigned char level;
	char far *pathname;
};

extern int   printspool_errno;
extern char *printspool_errlist[];

int  printspool_installed(void);
int  printspool_submit( const char *pathname );
int  printspool_remove( const char far *fname );
int  printspool_cancel(void);
char far *printspool_getqueue(void);
void printspool_endhold(void);
int  printspool_errorcount(void);

#define PSENOERR   0x00
#define PSEINVFNC  0x01
#define PSENOFILE  0x02
#define PSENOPATH  0x03
#define PSEMFILE   0x04
#define PSEACCES   0x05
#define PSEQUEFUL  0x08
#define PSESPLBUSY 0x09
#define PSENME2LNG 0x0c
#define PSEINVDRV  0x0f
