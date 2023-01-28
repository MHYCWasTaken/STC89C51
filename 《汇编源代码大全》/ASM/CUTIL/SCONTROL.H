/*                       *** scontrol.h ***                          */
/*                                                                   */
/* IBM - PC microsoft "C"                                            */
/*                                                                   */
/* Header file used by the screen and keyboard control library that  */
/* defines the various commands.  The extended screen and keyboard   */
/* device driver must be configured into DOS before these commands   */
/* will work.                                                        */
/*                                                                   */
/* Written by L. Cuthbertson, March 1984                             */
/*                                                                   */
/*********************************************************************/
/*                                                                   */
 
/* cursor control */
 
char CUP[7] = {'\033','[','#',';','#','H','\000'};
char CUU[5] = {'\033','[','#','A','\000'};
char CUD[5] = {'\033','[','#','B','\000'};
char CUF[5] = {'\033','[','#','C','\000'};
char CUB[5] = {'\033','[','#','D','\000'};
 
char HVP[7] = {'\033','[','#',';','#','f','\000'};
char DSR[5] = {'\033','[','6','n','\000'};
char CPR[7] = {'\033','[','#',';','#','R','\000'};
char SCP[4] = {'\033','[','s','\000'};
char RCP[4] = {'\033','[','u','\000'};
 
/* erase controls */
 
char ED[5] = {'\033','[','#','J','\000'};
char EL[5] = {'\033','[','#','K','\000'};
 
/* mode of operation */
 
char SGR[5] = {'\033','[','#','m','\000'};
char SM[6] = {'\033','[','=','#','h','\000'};
char RM[6] = {'\033','[','=','#','l','\000'};
 
/* keyboard reassignment */
 
char KEY[5] = {'\033','[','#','p','\000'};
 
/* other odds and ends */
 
#define NULL '\000'
#define ESC '\033'
#define POUND '#'
#define LBRACK '['
#define SEMI ';'
