; MOUSE.ASM (Turbo Assembler syntax)
; Assorted routines for interfacing to a Microsoft-compatible mouse driver.
; Public domain by Matthew Hildebrand (FidoNet 1:247/128.2)
; This module may be overlaid


IDEAL
P286
MODEL	LARGE


	CODESEG

; resets the mouse driver.  Returns 1 if mouse installed, 0 if not
; char resetMouse(void);
	PUBLIC	C	resetMouse
PROC	C	resetMouse
  xor	ax,ax
  int	33h
  cmp	ax,0FFFFh
  jne	@@L1
  mov	ax,1
  retcode
	@@L1:
  xor	ax,ax
  retcode
ENDP

; returns the number of buttons on the mouse
; CAUTION!  Also resets the mouse
; char getButtonsMouse(void);
	PUBLIC	C	getButtonsMouse
PROC	C	getButtonsMouse
  xor	ax,ax
  int	33h
  mov	al,bl
  xor	ah,ah
  retcode
ENDP

; shows the mouse pointer
; void showMouse(void);
	PUBLIC	C	showMouse
PROC	C	showMouse
  mov	ax,1
  int	33h
  retcode
ENDP

; hides the mouse pointer
; void hideMouse(void);
	PUBLIC	C	hideMouse
PROC	C	hideMouse
  mov	ax,2
  int	33h
  retcode
ENDP

; returns the current mouse (x,y) coordinates
; void getPosMouse(int far *x, int far *y);
	PUBLIC	C	getPosMouse
PROC	C	getPosMouse
	ARG	x:DATAPTR, y:DATAPTR
  mov	ax,3
  int	33h
  les	bx,[x]
;  shr	cx,1				; adjust for mode 13h bug
  mov	[es:bx],cx
  les	bx,[y]
  mov	[es:bx],dx
  leave
  retcode
ENDP

; returns true if a button is pressed
; int buttonMouse(void);
	PUBLIC	C	buttonMouse
PROC	C	buttonMouse
  mov	ax,3
  int	33h
  test	bl,1
  jz	@@L1
  mov	ax,1
  retcode

	@@L1:
  test	bl,2
  jz	@@L2
  mov	ax,2
  retcode
  	@@L2:
  test	bl,4
  jz	@@L3
  mov	ax,3
  retcode
  	@@L3:
  xor	ax,ax
  retcode
ENDP

; returns true if the left button is pressed
; int leftButtonMouse(void);
	PUBLIC	C	leftButtonMouse
PROC	C	leftButtonMouse
  mov	ax,3
  int	33h
  test	bl,1
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; returns true if the right button is pressed
; int rightButtonMouse(void);
	PUBLIC	C	rightButtonMouse
PROC	C	rightButtonMouse
  mov	ax,3
  int	33h
  test	bl,2
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; returns true if the center button is pressed
; int centerButtonMouse(void);
	PUBLIC	C	centerButtonMouse
PROC	C	centerButtonMouse
  mov	ax,3
  int	33h
  test	bl,4
  jnz	@@L1
  xor	ax,ax
  retcode
	@@L1:
  mov	ax,1
  retcode
ENDP

; sets the position of the mouse pointer
; void setPosMouse(unsigned x,unsigned y);
	PUBLIC	C	setPosMouse
PROC	C	setPosMouse
	ARG	x:WORD, y:WORD
  mov	ax,4
  mov	cx,[x]
;  shl	cx,1				; adjust for mode 13h bug
  mov	dx,[y]
  int	33h
  leave
  retcode
ENDP

; Returns the button press counter.
; unsigned buttonPressMouse(unsigned button, far *x, far *y);
	PUBLIC	C	buttonPressMouse
PROC	C	buttonPressMouse
	ARG	button:WORD, x:DATAPTR, y:DATAPTR
  mov	ax,5
  mov	bx,[button]
  dec	bx
  int	33h
  mov	ax,bx				; save press counter for return
  les	bx,[x]
;  shr	cx,1				; Adjust for mode 13h bug
  mov	[es:bx],cx			; x coordinate
  les	bx,[y]
  mov	[es:bx],dx			; y coordinate
  leave
  retcode
ENDP

; Returns the button release counter.
; unsigned buttonReleaseMouse(unsigned button, far *x, far *y);
	PUBLIC	C	buttonReleaseMouse
PROC	C	buttonReleaseMouse
	ARG	button:WORD, x:DATAPTR, y:DATAPTR
  mov	ax,6
  mov	bx,[button]
  dec	bx
  int	33h
  mov	ax,bx				; save release counter for return
  les	bx,[x]
;  shr	cx,1				; Adjust for mode 13h bug
  mov	[es:bx],cx			; x coordinate
  les	bx,[y]
  mov	[es:bx],dx			; y coordinate
  leave
  retcode
ENDP

; set the horizontal limits for the mouse pointer
; void setHorizLimitsMouse(unsigned min,unsigned max);
	PUBLIC	C	setHorizLimitsMouse
PROC	C	setHorizLimitsMouse
	ARG	min:WORD, max:WORD
  mov	ax,7
  mov	cx,[min]
  mov	dx,[max]
;  shl	dx,1				; adjust for mode 13h bug
  int	33h
  leave
  retcode
ENDP

; set the vertical limits for the mouse pointer
; void setVertLimitsMouse(unsigned min,unsigned max);
	PUBLIC	C	setVertLimitsMouse
PROC	C	setVertLimitsMouse
	ARG	min:WORD, max:WORD
  mov	ax,8
  mov	cx,[min]
  mov	dx,[max]
  int	33h
  leave
  retcode
ENDP

; set the graphics pointer shape
; void setPointerMouse(int xoff,int yoff,void *p);
	PUBLIC	C	setPointerMouse
PROC	C	setPointerMouse
	ARG	xOff:WORD, yOff:WORD, p:DATAPTR
  mov	ax,9
  mov	bx,[xOff]
  mov	cx,[yOff]
  les	dx,[p]
  int	33h
  leave
  retcode
ENDP

; set a mouse pointer exclusion area.  The pointer is not displayed when
; inside the specified coordinates.  Only one is active at a time.
; Cancelled by a call to resetMouse() or showMouse()
; void setExclusionMouse(unsigned ulx,unsigned uly,unsigned lrx,unsigned lry);
	PUBLIC	C	setExclusionMouse
PROC	C	setExclusionMouse
	ARG	ulx:WORD, uly:WORD, lrx:WORD, lry:WORD
  push	si di
  mov	ax,10
  mov	cx,[ulx]
;  shr	cx,1				; adjust for mode 13h bug (?)
  mov	dx,[uly]
  mov	si,[lrx]
;  shr	si,1				; adjust for mode 13h bug (?)
  mov	di,[lry]
  int	33h
  pop	di si
  leave
  retcode
ENDP

; Set the mickeys to pixels ratio (mickeys/8 pixels)
	PUBLIC	C	setRatioMouse
PROC	C	setRatioMouse
  	ARG	horiz:WORD, vert:WORD
  mov	ax,000Fh
  mov	cx,[horiz]
  mov	dx,[vert]
  int	33h
  leave
  retcode
ENDP

; Returns the size of the mouse save state buffer
	PUBLIC	C	getSaveSizeMouse
PROC	C	getSaveSizeMouse
  mov	ax,0015h
  int	33h
  mov	ax,bx
  retcode
ENDP

; Save the current state of the mouse driver
	PUBLIC	C	saveStateMouse
PROC	C	saveStateMouse
	ARG	data:DATAPTR
  mov	ax,0016h
  les	dx,[data]
  int	33h
  leave
  retcode
ENDP

; Restore the state of the mouse driver
	PUBLIC	C	restoreStateMouse
PROC	C	restoreStateMouse
	ARG	data:DATAPTR
  mov	ax,0017h
  les	dx,[data]
  int	33h
  leave
  retcode
ENDP

; Set the mouse sensitivity (mickeys/8 pixels)
	PUBLIC	C	setSensitivityMouse
PROC	C	setSensitivityMouse
	ARG	horiz:WORD, vert:WORD, doubleSpeed:WORD
  mov	ax,001Ah
  mov	bx,[horiz]
  mov	cx,[vert]
  mov	dx,[doubleSpeed]
  int	33h
  leave
  retcode
ENDP

; Get the mouse sensitivity (mickeys/8 pixels)
	PUBLIC	C	getSensitivityMouse
PROC	C	getSensitivityMouse
  	ARG	horiz:DATAPTR, vert:DATAPTR, doubleSpeed:DATAPTR
  push	di

  mov	ax,001Bh
  int	33h
  les	di,[horiz]
  mov	[es:di],bx
  les	di,[vert]
  mov	[es:di],cx
  les	di,[doubleSpeed]
  mov	[es:di],dx

  pop	di
  leave
  retcode
ENDP

; Disable the mouse driver and return the previous int 33h handler address
	PUBLIC	C	disableMouse
PROC	C	disableMouse
  mov	ax,001Fh
  int	33h
  cmp	ax,0FFFFh
  je	@@Error
  mov	dx,es
  mov	ax,bx
  retcode

  	@@Error:
  xor	dx,dx
  xor	ax,ax
  retcode
ENDP

; enables the mouse driver
; void enableMouse(void);
	PUBLIC	C	enableMouse
PROC	C	enableMouse
  mov	ax,20h
  int	33h
  retcode
ENDP

; same as resetMouse(), but no initialization of mouse hardware
; void softResetMouse(void);
	PUBLIC	C	softResetMouse
PROC	C	softResetMouse
  mov	ax,21h
  int	33h
  retcode
ENDP

; waits for the specified button to be released before returning
; void waitReleaseMouse(int button);
	PUBLIC	C	waitReleaseMouse
PROC	C	waitReleaseMouse
	ARG	button:WORD
  dec	[button]
	@@L1:
  mov	ax,3
  int	33h
  mov	cx,[button]
  mov	dx,1
  shl	dx,cl
  and	bx,dx
  or	bx,bx
  jnz	@@L1
  leave
  retcode
ENDP

ENDS
END