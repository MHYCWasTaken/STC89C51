;                         *** biosio.asm ***
;
; IBM-PC microsoft "C" under PC-DOS v2.00
;
; MICROSOFT "C" callable 8088 assembly routines that interface directly
; with the basic I/O system (BIOS).
;
; NOTE -- The IBM Technical Reference Manual contains a listing of the
;         BIOS and more complete descriptions of each interrupt.
;
; Written by L. Cuthbertson, April 1984
;
;**********************************************************************
;
PGROUP	GROUP	PROG
PROG	SEGMENT	BYTE PUBLIC 'PROG'

	PUBLIC	KEYRD,KEYHIT,KEYSHIF

	PUBLIC	BIOSINI,BIOSSET,BIOSPOS
	PUBLIC	BIOSUP,BIOSDWN,BIOSRCA,BIOSWCA,BIOSWC
	PUBLIC	BIOSWD,BIOSTTY,BIOSCUR

	PUBLIC	COMINI,COMOUT,COMIN,COMSTAT

	PUBLIC	INP,OUTP

	ASSUME	CS:PGROUP
;
;**********************************************************************
;
;                        *** KEYBOARD I/O ***
;
; NOTE - Keyboard interrupt description starts on page A-23 of Tech
;        Ref Manual.
;
;**********************************************************************
;
; Read a keyboard entry - wait for entry if one not ready.
;
; synopsis	c = keyrd();
;
;		int c;		high order bits contain scan code
;				low order bits contain character
;
; NOTE - Scan codes are described on page 2-17 of the Technical
;        Reference Manual.
;
KEYRD	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AH,0		; READ NEXT CHARACTER ENTERED
	INT	16H		; BIOS KEYBOARD I/O INTERRUPT
	POP	BP
	RET
KEYRD	ENDP
;
; Check to see if there is a character in the keyboard buffer.
;
; synopsis	iret = keyhit();
;
;		int iret;	= 0 if no character availible
;				= 1 character is availible
;
KEYHIT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AH,1		; CHECK KEYBOARD BUFFER FUNCTION
	INT	16H		; BIOS KEYBOARD I/O INTERRUPT
	MOV	AX,0		; ASSUME THAT NO CHARACTER READY
	JZ	K1
	MOV	AX,1		; CHARACTER READY
K1:
	POP	BP
	RET
KEYHIT	ENDP
;
; Check to see what the shift key status is.
;
; synopsis	iret = keyshif();
;
;		int iret;       shift key status
;
;			0x80 = insert state is active
;			0x40 = cap lock state has been toggled
;			0x20 = num lock state has been toggled
;			0x10 = scroll lock state has been toggled
;			0x08 = alternate shift key depressed
;			0x04 = control shift key depressed
;			0x02 = left shift key depressed
;			0x01 = right shift key depressed
;
; NOTE - from page A-2 and A-3 of Tech Ref Manual
;
KEYSHIF	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AH,2		; CHECK SHIFT STATUS FUNCTION
	INT	16H		; BIOS KEYBOARD I/O INTERRRUPT
	POP	BP
	RET
KEYSHIF	ENDP
;
; *********************************************************************
;
;			*** VIDEO I/O ***
;
; NOTE - the video I/O interrupt description starts on page A-43 of the
;        Tech Ref Manual.
;
; *********************************************************************
;
; Initialize screen I/O using the BIOS set mode call
;
; synopsis	biosini(stype);
;
;		int stype;	screen type
;
;			0 = 40x25 BW (power on default)
;			1 = 40x25 Color
;			2 = 80x25 BW
;			3 = 80x25 Color
;			graphics mode
;			4 = 320x200 Color
;			5 = 320x200 BW
;			6 = 640x200 BW
;			internal use only
;			7 = 80x25 BW card
;
BIOSINI	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; SCREEN TYPE IN AL
	MOV	AH,0		; SET MODE FUNCTION
	INT	10H		; BIOS VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSINI	ENDP
;
; Set the current cursor position.
;
; synopsis	biosset(irow,icol);
;
;		*** no value returned ***
;		int irow;	0 to 24
;		int icol;	0 to 79
;
BIOSSET	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DH,[BP+4]	; ROW
	MOV	DL,[BP+6]	; COLUMN
	MOV	BH,0		; CURRENT PAGE NUMBER
	MOV	AH,2		; CURSOR POSITION SET FUNTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSSET	ENDP
;
; Return the current cursor postion.
;
; synopsis	iret = biospos();
;
;		int iret;	high order bits contain row
;				low order bits contain column
;
BIOSPOS	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	BH,0		; CURRENT PAGE NUMBER
	MOV	AH,3		; CURSOR POSITION FUNCTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	MOV	AH,DH		; MOVE INT RETURN INTO FUNCTION RETURN
	MOV	AL,DL		; DITTO
	POP	BP
	RET
BIOSPOS	ENDP
;
; Scroll the screen up within a defined window.
;
; synopsis	biosup(numlines,trow,tlcol,brow,brcol,fchar);
;
;		int numlines;	number of lines to scroll up
;		int trow;	top row of window
;		int tlcol;	top left column of window
;		int brow;	bottom row of window
;		int brcol;	bottom right column of window
;		int fchar;	fill character of opened line
;
; note: numlines = 0 blanks entire window.  Upper left corner of full
;	screen is 0,0 while the bottom right corner of full screen is
;	24,79.
;
BIOSUP	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; NUMBER OF LINES TO SCROLL
	MOV	CH,[BP+6]	; TOP ROW OF WINDOW
	MOV	CL,[BP+8]	; TOP LEFT COLUMN OF WINDOW
	MOV	DH,[BP+10]	; BOTTOM ROW OF WINDOW
	MOV	DL,[BP+12]	; BOTTOM RIGHT COLUMN OF WINDOW
	MOV	BH,[BP+14]	; FILL CHARACTER
	MOV	AH,6		; SCROLL UP FUNCTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSUP	ENDP
;
; Scroll the screen down within a defined window.
;
; synopsis	biosdwn(numlines,trow,tlcol,brow,brcol,fchar);
;
;		int numlines;	number of lines to scroll down
;		int trow;	top row of window
;		int tlcol;	top left column of window
;		int brow;	bottom row of window
;		int brcol;	bottom right column of window
;		int fchar;	fill character of opened line
;
; note: numlines = 0 blanks entire window.  Upper left corner of full
;	screen is 0,0 while the bottom right corner of full screen is
;	24,79.
;
BIOSDWN	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; NUMBER OF LINES TO SCROLL
	MOV	CH,[BP+6]	; TOP ROW OF WINDOW
	MOV	CL,[BP+8]	; TOP LEFT COLUMN OF WINDOW
	MOV	DH,[BP+10]	; BOTTOM ROW OF WINDOW
	MOV	DL,[BP+12]	; BOTTOM RIGHT COLUMN OF WINDOW
	MOV	BH,[BP+14]	; FILL CHARACTER
	MOV	AH,7		; SCROLL DOWN FUNCTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSDWN	ENDP
;
; Read the contents of a given screen cell.
;
; synopsis	iret=biosrca();
;
;		int iret;	high order bits contain attributes
;				low order bits contain character
;
; NOTE - Attributes are defined on page 13-9 of the DOS v2.0 manual
;
BIOSRCA	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	BH,0		; ACTIVE DISPLAY PAGE
	MOV	AH,8		; READ CHARACTER + ATTRIBUTES FUNCTION
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSRCA	ENDP
;
; Write a character to the screen - with attributes.
;
; synopsis	bioswca(char,count,att);
;
;		*** no value returned ***
;		int char;	character to output
;		int count;	number of times to output character
;		int att;	character attribute
;
; NOTE - Attributes are defined on page 13-9 of the DOS v2.0 manual
;
BIOSWCA	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; CHARACTER
	MOV	CX,[BP+6]	; NUMBER OF CHARACTERS TO WRITE
	MOV	BL,[BP+8]	; CHARACTER ATTRIBUTE
	MOV	BH,0		; ACTIVE DISPLAY PAGE
	MOV	AH,9		; WRITE CHARACTER/w ATTRIBUTES FUNCTION
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSWCA	ENDP
;
; Write a character to the screen - no attributes.
;
; synopsis	bioswc(char,count);
;
;		*** no value returned ***
;		int char;	character to output
;		int count;	number of times to output character
;
BIOSWC	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; CHARACTER
	MOV	CX,[BP+6]	; NUMBER OF CHARACTERS TO WRITE
	MOV	BL,0;		; CHARACTER ATTRIBUTE - NULL
	MOV	BH,0		; ACTIVE DISPLAY PAGE
	MOV	AH,10		; WRITE CHARACTER ONLY FUNCTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSWC	ENDP
;
; Write a dot in graphics mode.
;
; synopsis	bioswd(irow,icol);
;
;		*** no value returned ***
;		int irow;
;		int icol;
;
BIOSWD	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	; ROW
	MOV	CX,[BP+6]	; COLUMN
	MOV	AL,1		; GREEN COLOR
	MOV	AH,12		; WRITE A DOT FUNCTION NUMBER
	INT	10H		; VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSWD	ENDP
;
; Write a character to the screen using the BIOS ascii teletype call.
; The teletype call will send cr/lf if column 79 is written to (0-79).
; It will scroll the screen up if row 24 (0-24) column 79 is written to.
; It will also beep the bell if ^g is received and provide a destructive
; backspace.
;
; synopsis	biostty(c);
;
;		char c;		character to write.
;
BIOSTTY	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AL,[BP+4]	; CHARACTER TO WRITE INTO AL
	MOV	AH,14		; TTY FUNCTION
	MOV	BH,0		; DISPLAY PAGE
	INT	10H		; BIOS VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSTTY	ENDP
;
; Return the current video state of the screen.
;
; synopsis	iret = bioscur();
;
;		int iret;	low bits are the mode currently set
;				(see biosini for description)
;				high bits are the number of columns on screen
;
BIOSCUR	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	AH,15		; CURRENT VIDEO STATE FUNCTION
	INT	10H		; BIOS VIDEO I/O INTERRUPT
	POP	BP
	RET
BIOSCUR	ENDP
;
; *******************************************************************
;
;			*** communications port ***
;
; NOTE - the communications port I/O is described starting on page A-20
;        of the Tech Ref Manual.
;
; *******************************************************************
;
; Initialize the communications port.
;
; synopsis	iret = comini(port,params);
;
;		int iret;	return status (see comstat)
;		int port;	communications port to initialize (0,1)
;		int params;	bit pattern for initialization -
;
;	7	6	5	4	3	2	1	0
;       ------BAUD RATE ---     --PARITY--   STOP BIT   --WORD LENGTH--
;
;	000 - 110		X0 - NONE	0 - 1	10 - 7 BITS
;	001 - 150		01 - ODD	1 - 2	11 - 8 BITS
;	010 - 300		11 - EVEN
;	011 - 600
;	100 - 1200
;	101 - 2400
;	110 - 4800
;	111 - 9600
;
COMINI	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	; COMM PORT TO INITIALIZE
	MOV	AL,[BP+6]	; PARAMETERS
	MOV	AH,0		; INITIALIZATION FUNCTION
	INT	14H		; COMM PORT I/O INTERRUPT
	POP	BP
	RET
COMINI	ENDP
;
; Write a character to the communications port.
;
; synopsis	iret = comout(port,c);
;
;		int iret;	error return ( >127 if error occured)
;				- see AH under modem control
;		int port;	communications port to write (0 or 1)
;		int c;		character to write.
;
COMOUT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	; COMM PORT TO WRITE
	MOV	AL,[BP+6]	; CHARACTER TO WRITE
	MOV	AH,1		; WRITE CHARACTER FUNCTION
	INT	14H		; COMM PORT I/O INTERRUPT
	POP	BP
	RET
COMOUT	ENDP
;
; Read a character from the communications port.  Waits for character if
; one is not ready.  See \comm\ibmtty.c for example of polling comm port
; for character without wait.
;
; synopsis	c = comin(port);
;
;		int c;		character read from comm port,
;				> 127 if error or no character ready.
;				- see AH under modem control
;		int port;	communications port to read (0 or 1)
;
COMIN	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	; COMM PORT TO READ
	MOV	AH,2		; READ CHARACTER FUNCTION
	INT	14H		; COMM PORT I/O INTERRUPT
	POP	BP
	RET
COMIN	ENDP
;
; Check the line and modem status
;
; synopsis	iret = comstat(port);
;
;		int iret;	; line and modem status
;		AH - high order bits contain line control status
;		bit 7 = time out
;		bit 6 = trans shift register empty
;		bit 5 = trans holding register empty
;		bit 4 = break detect
;		bit 3 = framing error
;		bit 2 = parity error
;		bit 1 = overrun error
;		bit 0 = data ready
;
;		AL - low order bits contain modem status
;		bit 7 = received line signal detect
;		bit 6 = ring indicator
;		bit 5 = data set ready
;		bit 4 = clear to send
;		bit 3 = delta receive line signal detect
;		bit 2 = trailing edge ring detector
;		bit 1 = delta data set ready
;		bit 0 = delta clear to send
;
; Note - from page A-21 of Tech Ref Manual
;
;		int port;	; communications port to check
;
COMSTAT	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	; COMM PORT TO CHECK
	MOV	AH,3		; STATUS FUNCTION
	INT	14H		; COMM PORT I/O INTERRUPT
	POP	BP
	RET
COMSTAT	ENDP
;
;**********************************************************************
;
;			*** io.asm ***
;
; IBM-PC 8088 assembly for interface with microsoft "C" under PC-DOS
;
; "C" callable subroutines that provide absolute pointer addressing.
; Pointers within a IBM-PC microsoft "C" program are relative to the
; start of the programs data segment.  These subroutines provide a
; mechanism to address absolute memory locations.
;
; Supplied by microsoft - commented by L. Cuthbertson, April 1984
;
;**********************************************************************
;
; Read an absolute memory location.
;
; synopsis	c = readabs(loc);
;
;		int c;		contents of memory location
;		int loc;	absolute memory location in hex
;
INP	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	;GET LOCATION ADDRESS
	IN	AL,DX		;READ LOCATION
	XOR	AH,AH		;CLEAR HIGH BYTE
	POP	BP
	RET
INP	ENDP
;
; Write an absolute memory location.
;
; synopsis	writeabs(loc,c);
;
;		int loc;	absolute memory location in hex
;		int c;		integer to write to memory location
;
OUTP	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[BP+4]	;GET LOCATION ADDRESS
	MOV	AX,[BP+6]	;GET ADDRESS OF INTEGER TO WRITE
	OUT	DX,AL		;WRITE TO MEMORY LOCATION
	POP	BP
	RET
OUTP	ENDP
;
;**********************************************************************
;
PROG	ENDS
	END
;

