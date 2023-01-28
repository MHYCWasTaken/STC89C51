	TITLE CRSONOFF
	PAGE ,132

;	Title:	CRSONOF.ASM
;	Date:	sometime 1985
;	Syntax:	do CURSON
;			do CURSOFF
;	Note:	turns cursor ON and OFF
;
;	Revised PBaenziger, pbprograms, 1215 Lane, Kalamzoo, MI 49001
;	(616) 349-9720 (Evenings), 323-7392 (Days, 8-4:30 EDT)
;	
;	Works for all monitors, simplified
;
public	CURSON			; set cursor ON        
public	CURSOFF			; set cursor OFF       
;
;
extrn	_RET:far			; Clipper return Null
;==================================================
DGROUP	GROUP	DATASG
datasg segment PUBLIC  'DATA'
;==================================================

CURS_OFF_VAL	DW  0F00H		; Seems to work under all conditions
CURR_CURS_ON	DW  0		; Current cursor on value
MONO_CURS_ON	DW  0B0CH		;0C0DH - alternate value
CG_CURS_ON	DW  0607H

;==================================================
datasg ends
;==================================================

;
;
;==================================================
_prog  segment byte		; byte aligned         
assume cs:_prog, DS:DGROUP                    
;==================================================
;

CURSON	proc	far			; far process
		
		push	bp			; Not really needed, routine doesn't
						; change BP. However, just in case of
						; a bad clone BIOS

		; Works with MDA, CGA and EGA and clones in text modes
		;!!!! Not for GRAPHIC modes !!!!!

        	CMP [CURR_CURS_ON],0	; Check if this is the first time
		JNE TURN_ON			; Jump if not, we know what board
							; we're running under

		;First time called, find out the video state
		MOV CX, [MONO_CURS_ON]	; Assume it's going to be MDA
		MOV AH, 0FH			; Find out video status
		INT 10H
		CMP AL, 7				; 7 = MDA, all others assumed CGA/EGA
		JE STORE_IT         	; Jump if MDA
		MOV CX, [CG_CURS_ON]	; No, it's CGA/EGA - replace values

STORE_IT:	MOV [CURR_CURS_ON], CX	; And save the value for future use
		
TURN_ON:	MOV CX, [CURR_CURS_ON]
		MOV AH, 1
		INT 10H

		pop	bp			; restore base pointer

		call	_RET			; call Clipper return for nothing
		
		ret				; far return

CURSON	endp				; end of process
;
;
CURSOFF	proc	far			; far process

		push	bp			; save return address

        	MOV CX, [CURS_OFF_VAL]
		MOV AH, 1
		INT 10H

		pop	bp			; restore base pointer
	
		call	_RET			; call Clipper return for nothing

		ret				; far return

CURSOFF	endp				; end of process
;
_prog	ends				; end of segment
		end				; end of programme
;
;
