	INCLUDE	EXTENDA.INC

;***********************************************************************
;
;	Equates
;

$define FALSE			0000H
$define TRUE			0001H

	CODESEG	ASMLIB06
	DATASEG


CLpublic <CURDIR, CURDRV, HOMEDIR>

; storage for full pathname and pointer to address it
CLstatic <byte ASCIIZ <<64 DUP(0)>>, cptr STRING ASCIIZ>


;******
;
;	CURDIR()
;
;	string = CURDIR(drive)
;
;		drive:   drive letter (A, B, ...)
;		     :	 current drive if omitted.
;
CLfunc char CURDIR <char drive>

CLcode
          PUSH	DS			; preserve
          PUSH	ES			; preserve
          LDS	SI, STRING		; point to mem block
          MOV	BYTE PTR [SI],0	; null string if error
          MOV	DL, 0			; assume default drive

          TESTNUL	drive		; test if parameter supplied
          JZ	FILL_ASCIIZ

          ; parameter supplied..get specified drive letter
          PUSH	ES			; preserve
          LES	BX, drive		; load pointer
          MOV	DL, ES:[BX]		; get drive letter
          AND	DL, 01011111B	; ensure upper case
          SUB	DL, ('A' - 1)	; convert to number ('A' = 1)
          POP	ES			; restore

FILL_ASCIIZ:
          PUSH  SI
          MOV	BYTE PTR [SI],'\'
          INC   SI
          DOSREQ      47H         ; Get current dir. string from DOS
          PUSH  DS
          PUSH  SI
          POP   DI
          POP   ES
          XOR   AX,AX             ; 
          CLD                     ; Scan direction forwards
          MOV   CX,0FFH           ; Max. bytes to scan
          REPNE SCASB             ; Find end of string
          MOV	BYTE PTR [DI],0   ; Mark it
          DEC   DI                ; Move back one
          MOV	BYTE PTR [DI],'\' ; And insert trailing backslash
          POP   SI
          POP	ES			; restore
          POP	DS			; restore

          ; return pointer to directory path
          CLRET	STRING

;****************************************************************************
;       CURDRV
;       This function checks and returns the DOS current disk drive as
;       an ASCIIZ string.
;       
;       Parameters :
;         None
;
;       Returns :
;         Current drive designation - A:, B:, C:, etc
;

CLFUNC CHAR CURDRV
CLCODE
          LDS   SI, STRING
	DOSREQ      19H               ; Get current drive service.
          ADD   AL,41H                  ; Convert digit to a letter
          MOV   DS:[SI+0],AL            ; and move it into the buffer.
          MOV   AL,':'                  ; Append a colon
          MOV   DS:[SI+1],AL            ; after the drive letter.
          MOV   AL,0                    ; Then null terminate
          MOV   DS:[SI+2],AL            ; the string
          CLRET STRING                  ; and return it to Clipper.

;****************************************************************************
;       HOMEDIR
;       This function returns the program home directory (the directory
;         in which the program is located) as an ASCIIZ string.
;       
;       Parameters :
;         None
;
;       Returns :
;         the program home directory as an ASCIIZ string.

CLFUNC    CHAR  HOMEDIR
CLCODE
	DOSREQ      62H         ; Get PSP segment in BX
          PUSH  BX                ; and move it
          POP   DS                ; into DS.
          MOV   CX,DS:2CH         ; Get environment block segment pointer
          PUSH  CX                ; from DS:2CH into CX and then move it
          POP   ES                ; into ES.
          XOR   DI,DI             ; Start at beginning of block
          XOR   AX,AX             ; and scan for a null (0H).
          CLD                     ; Scan direction forwards.

HOME1:
          REPNE SCASB             ; Now scan string.
          CMP   AL,ES:[DI]        ; Is byte past end another null?
          JNE   HOME1             ; No, more strings left, go around.
          ADD   DI,3              ; Else skip over strings count word.

          PUSH  ES                ; Save string base.
          PUSH  DI                ; Save string offset.
          MOV   BX,DI             ; and copy it to BX.
          REPNE SCASB             ; Scan string.
          MOV   AL,'\'            ; Now to strip off filename.
          STD                     ; Scan direction is backwards.
          REPNE SCASB             ; Scan string
          CLD                     ; Scan direction forwards again.
          ADD   DI,2              ; Adjust DI offset for overshoot.
          MOV   CX,DI             ; Move it into CX, and 
          SUB   CX,BX             ; Subtract BX to get string length.
          POP   SI                ; Restore string offset as source offset.
          POP   DS

          MOV   AX,DGROUP         ; Get DGROUP segment address
          MOV   ES,AX             ; and move it to ES.
          LEA   DI,ASCIIZ         ; Then get buffer offset in DI
          REP   MOVSB             ; and copy string into it.
          MOV   AL,0              ; Now stick a null terminator
          MOV   ES:[DI],AL        ; onto the end of the string.
          LEA   DI,ASCIIZ         ; Get starting offset again,
          SES   DI,STRING         ; and store pointer to buffer in STRING.
          CLRET STRING            ; Return it to Clipper

		END

