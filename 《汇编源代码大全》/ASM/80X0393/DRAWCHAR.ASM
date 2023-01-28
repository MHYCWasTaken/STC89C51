;
;  DRAWCHAR.ASM
;
;  Author: Matt Pritchard
;  Released to public domain
;
;Assumes Character set of 128 x 8 Bytes 
;  Character Set has a 16 byte Scratch Buffer at end.... 
; 
;GPRINTC (CharSet, CharNum, Xpos, Ypos, ColorF, ColorB) 
; 
; in "C"
;void GPRINTC(far char* SetData, int CharNum, int Xpos, 
;              int Ypos, int Color_Fg, int Color_Bg) 
; 
; in "BASIC" 
;TYPE CharSET 
;    BitMaps AS STRING * 1024 
;    Scratch AS STRING * 16 ;END TYPE 
; 
;DIM SetData AS CharSET 
; 
;DECLARE SUB GPRINTC(SEG SetData AS ANY, BYVAL CharNum%, BYVAL Xpos%,_ 
;                    BYVAL Ypos%, BYVAL ColorF%, BYVAL ColorB%) 
; 
; 
;Draws Text Character (8x8) in Graphics Mode 13 at Pixel (Xpos,Ypos) 
;In Color: COLORF with background of COLORB 
;

        .MODEL  Medium
        .CODE

SCRATCH_OFS     EQU     1024+2  ;Character Set Working Area Offset 
VGA_SEGMENT     EQU     0A000H  ;Vga Memory Segment 
SCREEN_WIDTH    EQU     320     ;Width of screen in Pixels/Bytes

GPC_STACK       STRUC
                        DW      ?       ;DI
                        DW      ?       ;SI
                        DW      ?       ;DS
                        DW      ?       ;BP
                        DD      ?       ;Caller
        GPC_COLORB      DB      ?,?     ;Background Color
        GPC_COLORF      DB      ?,?     ;Forground Color
        GPC_YPOS        DW      ?       ;Y position
        GPC_XPOS        DW      ?       ;X position
        GPC_CHAR_NUM    DB      ?,?     ;Character #
        GPC_SET_OFS     DW      ?       ;Offset of Set Data
        GPC_SET_SEG     DW      ?       ;Segment of Set Data GPC_STACK
ENDS

        PUBLIC GPRINTC

GPRINTC         PROC    FAR

        PUSH    BP
        PUSH    DS                      ;Save Data Seg
        PUSH    SI                      ;BC7 - Save Si,DI, Dir Flag
        PUSH    DI

        MOV     BP,SP                   ;Set to address of bp


        ;Compute Screen Addr to Write to in ES:DI

        MOV     AX,[BP].GPC_YPOS        ;Get Value of Ypos (mult * 320)
        MOV     DH,AL                   ;Save 256*Y
        MOV     CL,6                    ;mult * 2^6 (Y * 64)
        SHL     AX,CL                   ;Shift Left
        ADD     AH,DH                   ;Add in 256 * Ypos

        ADD     AX,[BP].GPC_XPOS        ;Add X position to addr

        MOV     DI,AX                   ;Save in DI

        ;Get Source ADDR of Character Bit Map in DS:SI
        ;But first, save colors in lookup table


        MOV     BX,[BP].GPC_SET_OFS     ;Get OFFSET of Character Set
        MOV     SI,BX                   ;Save in SI for now..

        ;Get colors

        MOV     AX,[BP].GPC_SET_SEG     ;Segment of Charset
        MOV     ES,AX                   ;Use ES to point to it...

        MOV     AL,[BP].GPC_COLORB      ;Get Backgorund color
        MOV     ES:[SI+SCRATCH_OFS],AL  ;Save in Table at Pos (0)

        MOV     AL,[BP].GPC_COLORF      ;Forground Color (1)
        MOV     ES:[SI+SCRATCH_OFS+1],AL   ;Save in Table

        ;Get addr of character map

        MOV     AL,[BP].GPC_CHAR_NUM    ;Get Character #
        AND     AX,007FH                ;Valid #'s are 0..127
        SHL     AX,1                    ;* 2
        SHL     AX,1                    ;* 4
        SHL     AX,1                    ;* 8

        MOV     BX,SI                   ;BX = Start of Char Set
        ADD     BX,SCRATCH_OFS          ;BX points to lookup table

        ADD     SI,AX                   ;Add Start of Character Set

        ;Get segment of character map

        MOV     DS,[BP].GPC_SET_SEG     ;Get Segment of Character Set
                                        ;Cant use DS (DGROUP) anymore

        MOV     AX,VGA_SEGMENT          ;VGA Graphics Segment
        MOV     ES,AX
        CLD                             ;Forwards....

        MOV     CX,8                    ;8 Bytes to Decode

DECODE_CHAR_BYTE:

        LODSB                           ;Get Byte to decode in DS:SI
        MOV     DL,AL                   ;Save in DL

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #1
        MOV     AL,DH                   ;Put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #2
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #3
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #4
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #5
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #6
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #7
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        XOR     DH,DH                   ;Clear DH
        SHL     DX,1                    ;Get Bit #8
        MOV     AL,DH                   ;put in AL
        XLAT    AL                      ;Translate into DAC value

        STOSB                           ;Write to Video Buffer

        ADD     DI,SCREEN_WIDTH-8       ;Point to Next Line

        LOOP    DECODE_CHAR_BYTE        ;Process all Lines

        POP     DI
        POP     SI
        POP     DS                      ;Restore Data Segment
        POP     BP                      ;Restore Bp

        RET     14                      ;Exit and Clean up Stack


GPRINTC  ENDP
