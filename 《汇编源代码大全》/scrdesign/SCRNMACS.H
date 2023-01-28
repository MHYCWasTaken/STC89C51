/*
**  Macros for managing direct video writes by Jerry Houston
*/

#if defined(__TURBOC__)
 #define FAR far
#else
 #define FAR _far
#endif
 
#ifndef MK_FP
 #define MK_FP(seg,off) ((void far *)(((long)(seg) << 16)|(unsigned)(off)))
#endif
 
/*
**  Functions in VIDPORT.C in SNIPPETS
*/
 
void GotoXY(int col, int row);
void ClrScrn(int vattrib);
 
#if !defined(COLORMODE)
 #define COLORMODE  ((*(char FAR *)0x0449) != 7)
 #define EXT_KBD    (*(char FAR *)0x0496 & 16)
 #define VIDPAGE    (*((unsigned char far *)0x0462))
 #define ROWSIZE    (*(int FAR *)0x044A)
 #define SCANLINES  ((int)*(char FAR*)0x0461)
 #define SCRBUFF    ((unsigned FAR *)((COLORMODE)?0xB8000000:0xB0000000))
 #define SCREENSEG  ((_segment)((COLORMODE)?0xB800:0xB000))
 #define SCREENSIZE ((*(int FAR *)0x044C) >> 1)
 #define SCREENCOLS (*(int FAR *)0x044A)
 #define SCREENROWS ((*(char FAR *)0x0484)?1+(*(char FAR *)0x0484):25)
#endif

/*
     COLORMODE  = true/false, are we using color?
     EXT_KBD    = true/false, extended keyboard in use?
     VIDPAGE    = current video page in use
     SCANLINES  = number of scan lines in a character.
     SCRBUFF    = returns B800:0000 if using color, B000:0000 if mono.
     SCREENSEG  = when you just need the segment portion.
     SCREENSIZE = number of (2-byte) cells required to save screen.
     SCREENCOLS = number of columns, often 80.
     SCREENROWS = number of rows, usually defaults to 25.
*/
