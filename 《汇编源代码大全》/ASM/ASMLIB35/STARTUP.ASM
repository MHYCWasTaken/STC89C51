include	asm.inc

extrn	endprog:proc
extrn	breaktrap:proc
extrn	breakrelease:proc
IFDEF	HEAP
extrn	hinit:proc
ENDIF
extrn	mymain:proc

; make LINK search through ASMLIB for external references
; so I don't have to specify ASMLIB on the command line
IF codesize EQ 1
IF datasize EQ 2
	IFDEF	NOXT
	includelib	286huge
	ELSE
	includelib	asmhuge
	ENDIF
ELSE
	IFDEF	NOXT
	includelib	286lib
	ELSE
	includelib	asmlib
	ENDIF
ENDIF
ELSE
	IFDEF	NOXT
	includelib	286small
	ELSE
	includelib	asmsmall
	ENDIF
ENDIF

stacksize	equ	1024

.stack stacksize

.data
public	pspseg
pspseg		dw	?		; storage for PSP segment

IFDEF	HEAP
; make some space available for the local heap
heapsize	equ	32767
heapbuffer	db	heapsize dup(0)
ENDIF

.code
start:
; start by pointing DS to DGROUP
	mov	dx,@data
	mov	ds,dx
	assume	ds:@data

; adjust for MS-LINK bug that puts SS in wrong segment
; THIS ADJUSTMENT IS REQURIED BY SEVERAL ASMLIB SUBROUTINES
	mov	ax,ss
	sub	ax,dx			; paragraph difference
	mov	cl,4
	shl	ax,cl			; converted to bytes
	add	sp,ax			; fix SP for new SS
	mov	ss,dx			; assume ss:DGROUP

; save PSP segment for DOS 2.xx
	mov	pspseg,es

; next, release memory beyond the end of the program
; The file ENDPROG.ASM has a definition for ZSEG, marking the
; end of the program's code, data and stack area.  I want to
; release memory beyond ZSEG so I can use DOS function 48h for
; memory allocation.
; When linking ENDPROG.OBJ with your other object files, be sure
; ENDPROG is at the end of your list of object files.  The new segment
; ZSEG surprises link, and it has little choice but to put ZSEG
; at the end of the program.
	call	endprog			; returns AX = last segment 

	mov	bx,es			; first segment (ES = seg PSP)
	sub	bx,ax
	neg	bx			; size of program in paragraphs
	mov	ah,4Ah			; resize memory block
	int	21h

IFDEF	HEAP
; need to initialize the heap in order to work properly.  A common cause
; of bombed programs is failure to initilaize the heap.
	lea	bx,heapbuffer
	mov	ax,heapsize
	call	hinit
ENDIF

; prevent an unintended program crash; trap Ctrl+Break, Ctrl+C and
; Ctrl+Alt+Del key combinations
	call	breaktrap

; call your main program here
	call	mymain

exit:
; de-activate the Ctrl+Break trap
	call	breakrelease

; normal program exit
	mov	ax,4C00h
	int	21h

end	start
	end
