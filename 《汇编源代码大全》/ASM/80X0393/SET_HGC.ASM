; ********************************************************************
; Set_HGC.    Switch Hercules in Graphics mode
; Code hereby placed into the PublicDomain
; by Oliver Reubens
; (nc) No Copyright 1993, No rights reserved, Nopetent Pending.
; ********************************************************************

CTRC_Parms DB   00h,35h    ; Hor. Total = 54 characters
           DB   01h,2Dh    ; Hor. Displayed = 45 characters
           DB   02h,2Eh    ; Hor. Sync Pos. = on 46 character
           DB   03h,07h    ; Hor. Sync Width = 7 character clocks
           DB   04h,5Bh    ; Vert. Total = 92 characters (368 Lines)
           DB   05h,02h    ; Vert. Adjust = 2 Scanlines
           DB   06h,57h    ; Vert. Displayed = 87 character rows
           DB   07h,57h    ; Vert. Sync Pos. = At 87 character rows
           DB   09h,03h    ; Max Scan Line = 4 scanlines/character

Set_HGC    PROC NEAR
           CLD             ; increment on string instructions
           MOV  AX,0B000h  ; \
           MOV  ES,AX      ;  \
           MOV  DI,0       ;   \  Clear screen
           MOV  CX,16000   ;   /
           XOR  AX,AX      ;  /
           REP  STOSW      ; /

           MOV  DX,003BFh  ; Configuration switch port
           MOV  AL,1       ; \ BIT 0 = 1 : Make Graphmode possible
           OUT  DX,AL      ; / BIT 1 = 0 : Exclude second 32K of video
           MOV  DX,003B8h  ; Mode control port of CRTC
           XOR  AL,AL      ; Bit 3 = 0 -> DISABLE VIDEO SIGNAL
           OUT  DX,AL
     ; PROGRAM THE CRTC
           SUB  DL,4       ; DX=3B4h = ADDRES REGISTER OF CRTC
           MOV  SI,OFFSET CRTC_Parms
           MOV  CX,9       ; Change 9 CRTC parameters NEXT:      LODSW
           OUT  DX,AX
           LOOP NEXT
      ; INITIALIZE GRAPHICS }
           ADD  DL,4       ; DX=3B8h = Mode control port of CRTC
           MOV  AL,0Ah     ; \ BIT 1 = 1 -> Graphics on
           OUT  DX,AL      ; / BIT 3 = 1 -> Video on
           RET
Set_HGC    ENDP
