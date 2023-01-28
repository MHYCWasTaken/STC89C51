;
; MODEX.ASM - A Complete Mode X Library 
; 
; By Matt Pritchard 
; With considerable input from Michael Abrash 
; 
; The following information is donated to the public domain in 
; the hopes that save other programmers much frustration. 
; 
; If you do use this code in a product, it would be nice if 
; you include a line like 'Mode X routine by Matt Pritchard' 
; in the credits. '
;

    .MODEL Medium, Basic
    .286
    PAGE    255, 132

    ; ==== MACROS ====

OUT_16 MACRO Register, Value
    MOV     DX, Register        ; Select Register
    MOV     AX, Value           ; Get Out Value
    OUT     DX, AX              ; Set Register(s) 
    ENDM

OUT_8 MACRO Register, Value
    MOV     DX, Register        ; Select Register
    MOV     AL, Value           ; Get Out Value
    OUT     DX, AL              ; Set Register 
    ENDM

    ; ==== General Constants ====

    False   EQU 0
    True    EQU -1

    b       EQU BYTE PTR
    w       EQU WORD PTR
    d       EQU DWORD PTR

    ; ==== VGA Register Values ====

    VGA_Segment     EQU 0A000H  ; Vga Memory Segment

    ATTRIB_Ctrl     EQU 03C0H   ; VGA Attribute Controller
    GC_Index        EQU 03CEH   ; VGA Graphics Controller
    SC_Index        EQU 03C4H   ; VGA Sequencer Controller
    SC_Data         EQU 03C5H   ; VGA Sequencer Data Port
    CRTC_Index      EQU 03D4H   ; VGA CRT Controller
    CRTC_Data       EQU 03D5H   ; VGA CRT Controller Data
    MISC_OUTPUT     EQU 03C2H   ; VGA Misc Register
    INPUT_1         EQU 03DAH   ; Input Status #1 Register

    DAC_WRITE_ADDR  EQU 03C8H   ; VGA DAC Write Addr Register
    DAC_READ_ADDR   EQU 03C7H   ; VGA DAC Read Addr Register
    PEL_DATA_REG    EQU 03C9H   ; VGA DAC/PEL data Register R/W

    MAP_MASK        EQU 02      ; Map Register #
    READ_MAP        EQU 04      ; Read Map Register #
    MAP_MASK_PLANE1 EQU 0102h   ; Map Register + Plane 1

    START_DISP_HI   EQU 0Ch     ; CRTC Index: Display Start Addr Hi
    START_DISP_LO   EQU 0Dh     ; CRTC Index: Display Start Addr Lo

    PLANE_BITS      EQU 03h     ; Bits 0-1 of Xpos = Plane #
    ALL_PLANES      EQU 0Fh     ; All Bit Planes Selected
    ALL_PLANES_ON   EQU 0F02h   ; Map Register + All Bit Planes
    CHAIN4_OFF      EQU 0604h   ; Chain 4 mode Off
    ASYNC_RESET     EQU 0100h   ; (A)synchronous Reset
    SEQU_RESTART    EQU 0300h   ; Sequencer Restart

    ; Constants Specific for these routines

    NUM_MODES       EQU 8       ; # of Mode X Variations

    ; Specific Mode Data Table format...

    M_MiscR         EQU 0       ; Value of MISC_OUTPUT register
    M_Pages         EQU 1       ; Maximum Possible # of pages
    M_XSize         EQU 2       ; X Size Displayed on screen
    M_YSize         EQU 4       ; Y Size Displayed on screen
    M_XMax          EQU 6       ; Maximum Possible X Size
    M_YMax          EQU 8       ; Maximum Possible Y Size
    M_CRTC          EQU 10      ; Table of CRTC register values

    ; ==== DGROUP STORAGE NEEDED (36 BYTES) =====

    .DATA?

SCREEN_WIDTH    DW  0           ; Width of a line in Bytes 
SCREEN_WIDTHx4  DW  0           ; Width of a line in Pixels 
SCREEN_HEIGHT   DW  0           ; Veritcal Height in Pixels

LAST_PAGE       DW  0           ; # of Display Pages 
PAGE_ADDR       DW  0, 0, 0, 0  ; Offsets to start of each page

PAGE_SIZE       DW  0           ; Size of Page in Addr Bytes

DISPLAY_PAGE    DW  0           ; Page # currently displayed 
ACTIVE_PAGE     DW  0           ; Page # currently active

CURRENT_PAGE    DW  0           ; Offset of current Page 
CURRENT_SEGMENT DW  0           ; Segment of VGA memory

CURRENT_XOFFSET DW  0           ; Current Display X Offset 
CURRENT_YOFFSET DW  0           ; Current Display Y Offset

CURRENT_MOFFSET DW  0           ; Current Start Offset

MAX_XOFFSET     DW  0           ; Current Display X Offset 
MAX_YOFFSET     DW  0           ; Current Display Y Offset

    .CODE

    ; Data Tables, Put in Code Segement for Easy Access
    ; (Like when all the other Segment Registers are in
    ; use!!) and reduced DGROUP requirements...

LEFT_CLIP_MASK      DB  0FH, 0EH, 0CH, 08H

RIGHT_CLIP_MASK     DB  01H, 03H, 07H, 0FH

MODE_TABLE:
        DW  OFFSET MODE_320x200, OFFSET MODE_320x400
        DW  OFFSET MODE_360x200, OFFSET MODE_360x400
        DW  OFFSET MODE_320x240, OFFSET MODE_320x480
        DW  OFFSET MODE_360x240, OFFSET MODE_360x480

MODE_320x200:           ; Data for 320 by 200 Pixels

        DB  063h        ; 400 scan Lines & 25 Mhz Clock
        DB  4           ; Maximum of 4 Pages
        DW  320, 200    ; Displayed Pixels (X,Y)
        DW  1302, 816   ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  00014H      ; Dword Mode off
        DW  0E317H      ; turn on Byte Mode
        DW  00000h      ; End of CRTC Data for 320x200

MODE_320x400:           ; Data for 320 by 400 Pixels

        DB  063h        ; 400 scan Lines & 25 Mhz Clock
        DB  2           ; Maximum of 2 Pages
        DW  320, 400    ; Displayed Pixels X,Y
        DW  648, 816    ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  04009H      ; Cell Heigth (1 Scan Line)
        DW  00014H      ; Dword Mode off
        DW  0E317H      ; turn on Byte Mode
        DW  00000h      ; End of CRTC Data for 320x400

MODE_360x240:           ; Data for 360 by 240 Pixels

        DB  0E7h        ; 480 scan Lines & 28 Mhz Clock
        DB  3           ; Maximum of 3 Pages
        DW  360, 240    ; Displayed Pixels X,Y
        DW  1092, 728   ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  06B00H      ; Horz total
        DW  05901H      ; Horz Displayed
        DW  05A02H      ; Start Horz Blanking
        DW  08E03H      ; End Horz Blanking
        DW  05E04H      ; Start H Sync
        DW  08A05H      ; End H Sync
        DW  00D06H      ; Vertical Total
        DW  03E07H      ; Overflow
        DW  04109H      ; Cell Heigth (2 Scan Lines)
        DW  0EA10H      ; V Sync Start
        DW  0AC11H      ; V Sync End/Prot Cr0 Cr7
        DW  0DF12H      ; Vertical Displayed
        DW  00014H      ; Dword Mode off
        DW  0E715H      ; V Blank Start
        DW  00616H      ; V Blank End
        DW  0E317H      ; turn on Byte Mode
        DW  00000h      ; End of CRTC Data for 360x240

MODE_360x480:           ; Data for 360 by 480 Pixels

        DB  0E7h        ; 480 scan Lines & 28 Mhz Clock
        DB  1           ; Only 1 Page Possible
        DW  360, 480    ; Displayed Pixels X,Y
        DW  544, 728    ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  06B00H      ; Horz total
        DW  05901H      ; Horz Displayed
        DW  05A02H      ; Start Horz Blanking
        DW  08E03H      ; End Horz Blanking
        DW  05E04H      ; Start H Sync
        DW  08A05H      ; End H Sync
        DW  00D06H      ; Vertical Total
        DW  03E07H      ; Overflow
        DW  04009H      ; Cell Heigth (1 Scan Line)
        DW  0EA10H      ; V Sync Start
        DW  0AC11H      ; V Sync End/Prot Cr0 Cr7
        DW  0DF12H      ; Vertical Displayed
        DW  00014H      ; Dword Mode off
        DW  0E715H      ; V Blank Start
        DW  00616H      ; V Blank End
        DW  0E317H      ; turn on Byte Mode

        DW  00000h      ; End of CRTC Data for 360x480

MODE_320x240:           ; Data for 320 by 240 Pixels

        DB  0E3h        ; 480 scan Lines & 25 Mhz Clock
        DB  3           ; Maximum of 3 Pages
        DW  320, 240    ; Displayed Pixels X,Y
        DW  1088, 818   ; Max Possible X and Y Sizes

        DW  00D06H      ; Vertical Total
        DW  03E07H      ; Overflow
        DW  04109H      ; Cell Heigth (2 Scan Lines)
        DW  0EA10H      ; V Sync Start
        DW  0AC11H      ; V Sync End/Prot Cr0 Cr7
        DW  0DF12H      ; Vertical Displayed
        DW  00014H      ; Dword Mode off
        DW  0E715H      ; V Blank Start
        DW  00616H      ; V Blank End
        DW  0E317H      ; turn on Byte Mode

        DW  00000h      ; End of CRTC Data for 320x240

MODE_320x480:           ; Data for 320 by 480 Pixels

        DB  0E3h        ; 480 scan Lines & 25 Mhz Clock
        DB  1           ; Only 1 Page Possible
        DW  320, 480    ; Displayed Pixels X,Y
        DW  540, 818    ; Max Possible X and Y Sizes

        DW  00D06H      ; Vertical Total
        DW  03E07H      ; Overflow
        DW  04009H      ; Cell Heigth (1 Scan Line)
        DW  0EA10H      ; V Sync Start
        DW  0AC11H      ; V Sync End/Prot Cr0 Cr7
        DW  0DF12H      ; Vertical Displayed
        DW  00014H      ; Dword Mode off
        DW  0E715H      ; V Blank Start
        DW  00616H      ; V Blank End
        DW  0E317H      ; turn on Byte Mode

        DW  00000h      ; End of CRTC Data for 320x480

MODE_360x200:           ; Data for 360 by 200 Pixels

        DB  067h        ; 400 scan Lines & 28 Mhz Clock
        DB  3           ; Maximum of 3 Pages
        DW  360, 200    ; Displayed Pixels (X,Y)
        DW  1302, 728   ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  06B00H      ; Horz total
        DW  05901H      ; Horz Displayed
        DW  05A02H      ; Start Horz Blanking
        DW  08E03H      ; End Horz Blanking
        DW  05E04H      ; Start H Sync
        DW  08A05H      ; End H Sync
        DW  00014H      ; Dword Mode off
        DW  0E317H      ; turn on Byte Mode
        DW  00000h      ; End of CRTC Data for 360x200

MODE_360x400:           ; Data for 360 by 400 Pixels

        DB  067h        ; 400 scan Lines & 28 Mhz Clock
        DB  1           ; Maximum of 1 Pages
        DW  360, 400    ; Displayed Pixels X,Y
        DW  648, 816    ; Max Possible X and Y Sizes

        ;CRTC Setup Data....

        DW  06B00H      ; Horz total
        DW  05901H      ; Horz Displayed
        DW  05A02H      ; Start Horz Blanking
        DW  08E03H      ; End Horz Blanking
        DW  05E04H      ; Start H Sync
        DW  08A05H      ; End H Sync
        DW  04009H      ; Cell Heigth (1 Scan Line)
        DW  00014H      ; Dword Mode off
        DW  0E317H      ; turn on Byte Mode
        DW  00000h      ; End of CRTC Data for 360x400


; ===== ACTUAL ASSEMBLY CODE =====

;====================================================== 
;SET_VGA_MODEX%(ModeType%, MaxXPos%, MaxYpos%, Pages%)
;====================================================== 
; 
; Sets Up the specified version of Mode X.  Allows for 
; the setup of multiple video pages, and a virtual 
; screen which can be larger than the displayed screen 
; (which can then be scrolled a pixel at a time) 
; 
; INPUT PARAMETERS: 
; 
; ModeType%  - The Desired Version of 256 Color Mode X 
; 
;     0 =  320 x 200, 4 Pages max,  1.2:1 Aspect Ratio 
;     1 =  320 x 400, 2 Pages max,  2.4:1 Aspect Ratio 
;     2 =  360 x 200, 3 Pages max,  1.35:1 Aspect Ratio 
;     3 =  360 x 400, 1 Page  max,  2.7:1 Aspect Ratio 
;     4 =  320 x 240, 3 Pages max,  1:1 Aspect Ratio 
;     5 =  320 x 480, 1 Page  max,  2:1 Aspect Ratio 
;     6 =  360 x 240, 3 Pages max,  1.125:1 Aspect Ratio 
;     7 =  360 x 480, 1 Page  max,  2.25:1 Aspect Ratio 
; 
; MaxXpos = The Desired Virtual Screen Width 
; MaxYpos = The Desired Virtual Screen Height 
; Pages%  = The Desired # of Video Pages 
; 
; RETURNS in AX:    0 (= Failure) or ffff (= Success) 
;

SVM_STACK   STRUC
                DW  ?,?,?,? ;DI,SI,DS,BP
                DD  ?       ;Caller
    SVM_Pages   DW  ?
    SVM_Ysize   DW  ?
    SVM_Xsize   DW  ?
    SVM_Mode    DW  ?
SVM_STACK   ENDS

    PUBLIC  SET_VGA_MODEX

SET_VGA_MODEX   PROC    FAR

    PUSH    BP                  ; Preserve Important
    PUSH    DS                  ; Register Values
    PUSH    SI
    PUSH    DI

    MOV     BP,SP               ; Set up Stack Frame

    ;Check Legality of Mode Request....

    MOV     BX, [BP].SVM_Mode   ; Get Requested Mode #
    CMP     BX, NUM_MODES       ; Is it 0..7?
    JAE     SVM_BadModeSetup    ; If Not, Error out

    SHL     BX, 1                   ; Scale BX
    MOV     SI, w CS:MODE_TABLE[BX] ; CS:SI -> Mode Info
    PUSH    SI                      ; Save for later use

    ;Check # of Requested Display Pages

    MOV     CX, [Bp].SVM_Pages  ; Get # of Requested Pages
    CMP     CX, CS:[SI].M_Pages ; Check # Pages for mode
    JA      SVM_BadModeSetup    ; Report Error if too Many
    OR      CX, CX              ; # Pages = 0?
    JE      SVM_BadModeSetup    ; Report Error if 0

    ;Check Validity of X Size

    AND     [BP].SVM_XSize, 0FFF8h  ;X size Mod 8 Must = 0

    MOV     AX, [BP].SVM_XSize  ; Get Logical Screen Width
    CMP     AX, CS:[SI].M_XSize ; Check against Displayed X
    JB      SVM_BadModeSetup    ; Report Error if too small
    CMP     AX, CS:[SI].M_XMax  ; Check against Max X
    JA      SVM_BadModeSetup    ; Report Error if too big

    ;Check Validity of Y Size

    MOV     BX, [BP].SVM_YSize  ; Get Logical Screen Height
    CMP     BX, CS:[SI].M_YSize ; Check against Displayed Y
    JB      SVM_BadModeSetup    ; Report Error if too small
    CMP     BX, CS:[SI].M_YMax  ; Check against Max Y
    JA      SVM_BadModeSetup    ; Report Error if too big

    ;Enough memory to Fit it all?

    SHR     AX, 2               ;# of Bytes:Line = XSize/4
    MUL     BX                  ;# Bytes/Page = AX/4 * BX
    JO      SVM_BadModeSetup    ;Exit if Page Size > 256K

    MUL     CX                  ;Total Mem = PageSize * Pages
    JNO     SVM_Continue        ;Exit if Total Size > 256K

SVM_BadModeSetup:

    XOR     AX, AX              ; Return Value = False
    JMP     SVM_Exit            ; Normal Exit

SVM_Continue:

    MOV     AX, 13H             ; Start with Mode 13H
    INT     10H                 ; Let BIOS Set Mode

    OUT_16  SC_INDEX, CHAIN4_OFF    ; Disable Chain 4 Mode
    OUT_16  SC_INDEX, ASYNC_RESET   ; (A)synchronous Reset

    MOV     DX, MISC_OUTPUT     ; VGA Misc Register
    MOV     AL, CS:[SI].M_MiscR ; Get New Timing/Size Value
    OUT     DX, AL              ; Set VGA Misc Register

    OUT_16  SC_INDEX, SEQU_RESTART  ; Restart Sequencer ...

    MOV     DX, CRTC_INDEX      ; Vga crtc Registers
    MOV     AL, 11H             ; Vert Retrace End Register
    OUT     DX, AL              ; Load Current Value
    INC     DX                  ; Point to Data
    IN      AL, DX              ; Get Value, Bit 7 = Protect
    AND     AL, 7FH             ; Mask out Write Protect
    OUT     DX, AL              ; And send it back

    MOV     DX, CRTC_INDEX      ; Vga Crtc Registers
    ADD     SI, M_CRTC          ; SI -> CRTC Parameter Data

SVM_Setup_CRTC:

    MOV     AX, CS:[SI]         ; Get CRTC Data from Table
    ADD     SI, 2               ; Advance Pointer
    OR      AX, AX              ; At End of Data Table?
    JZ      SVM_Set_Data        ; If so, Exit Loop

    OUT     DX, AX              ; Reprogram VGA CRTC reg
    JMP     SVM_Setup_CRTC      ; Process Next Table Entry

SVM_Set_Data:

    XOR     AX, AX              ; AX = 0
    MOV     DISPLAY_PAGE, AX    ; Display Page = 0
    MOV     ACTIVE_PAGE, AX     ; Active Page = 0
    MOV     CURRENT_PAGE, AX    ; Current Page (Offset) = 0
    MOV     CURRENT_XOFFSET, AX ; Horz Scroll Index = 0
    MOV     CURRENT_YOFFSET, AX ; Vert Scroll Index = 0
    MOV     CURRENT_MOFFSET, AX ; Memory Scroll Index = 0

    MOV     AX, VGA_SEGMENT     ; Segment for VGA memory
    MOV     CURRENT_SEGMENT, AX ; Save for Future LES's

    ;Set Logical Screen Width, X Scroll and Our Data

    POP     SI                  ; Get Saved Ptr to Mode Info
    MOV     AX, [Bp].SVM_Xsize  ; Get Display Width

    MOV     CX, AX              ; CX = Logical Width
    SUB     CX, CS:[SI].M_XSize ; CX = Max X Scroll Value
    MOV     MAX_XOFFSET, CX     ; Set Maximum X Scroll

    MOV     SCREEN_WIDTHx4, AX  ; Save Width in Pixels
    SHR     AX, 2               ; Bytes = Pixels / 4
    MOV     SCREEN_WIDTH, AX    ; Save Width in Pixels

    SHR     AX, 1               ; Offset Value = Bytes / 2
    MOV     AH, 13h             ; CRTC Offset Register Index
    XCHG    AL, AH              ; Switch format for OUT
    OUT     DX, AX              ; Set VGA CRTC Offset Reg

    ;Setup Data table, Y Scroll, Misc for Other Routines

    MOV     AX, [Bp].SVM_Ysize  ; Get Logical Screen Hieght

    MOV     CX, AX              ; CX = Logical Height
    SUB     BX, CS:[SI].M_YSize ; CX = Max Y Scroll Value
    MOV     MAX_YOFFSET, CX     ; Set Maximum Y Scroll

    MOV     SCREEN_HEIGHT, AX   ; Save Hight in Pixels
    MUL     SCREEN_WIDTH        ; AX = Page Size in Bytes,
    MOV     PAGE_SIZE, AX       ; Save Page Size

    MOV     CX, [Bp].SVM_Pages  ; Get # of Pages
    MOV     LAST_PAGE, CX       ; Save # of Pages

    MOV     BX, 0               ; Page # = 0
    MOV     DX, BX              ; Page 0 Offset = 0

SVM_Set_Pages:

    MOV     PAGE_ADDR[BX], DX   ; Set Page #(BX) Offset
    ADD     BX, 2               ; Page#++
    ADD     DX, AX              ; Compute Addr of Next Page
    LOOP    SVM_Set_Pages       ; Loop until Done

    ;Clear VGA Memory

    OUT_16  SC_INDEX, ALL_PLANES_ON ; Select All Planes
    LES     DI, d CURRENT_PAGE      ; Point to Start of VGA memory

    XOR     AX, AX              ; AX = 0
    CLD                         ; Block Xfer Forwards
    MOV     CX, 8000H           ; 32K * 4 * 2 = 256K
    REP     STOSW               ; Clear dat memory!

    MOV     AX, True            ; Return Success Code

SVM_EXIT:

    POP     DI                  ; Restore Saved Registers
    POP     SI
    POP     DS
    POP     BP

    RET     8                   ;We are Done.. Outa here

SET_VGA_MODEX   ENDP


;==================
;SET_MODEX% (Mode%)
;==================
;
; Quickie Mode Set - Sets Up Mode X to Default Configuration 
; 
; INPUT PARAMETERS: 
; 
; ModeType%  - The Desired Version of 256 Color Mode X 
; 
;   (0 to 7 - See Chart under SET_VGA_MODEX) 
; 
; RETURNS in AX:    0 (= Failure) or ffff (= Success) 
;

SM_STACK    STRUC
                DW  ?   ;BP
                DD  ?   ;Caller
    SM_Mode     DW  ?
SM_STACK    ENDS

    PUBLIC  SET_MODEX

SET_MODEX   PROC    FAR

    PUSH    BP                  ; Preserve Important
    MOV     BP,SP               ; Set up Stack Frame

    XOR     AX, AX              ; Assume Failure
    MOV     BX, [BP].SM_Mode    ; Get Desired Mode #

    CMP     BX, NUM_MODES       ; Is it 0..7?
    JAE     @SX_Exit            ; If Not, don't Bother

    PUSH    BX                  ; Push Mode Parameter

    SHL     BX, 1                   ; Scale BX
    MOV     SI, w CS:MODE_TABLE[BX] ;CS:SI -> Mode Info

    MOV     AX, CS:[SI].M_XSize ; Get Default X Size
    PUSH    AX                  ; Push X Size Parameter

    MOV     AX, CS:[SI].M_Ysize ; Get Default Y size
    PUSH    AX                  ; Push Y Size Parameter

    MOV     AL, CS:[SI].M_Pages ; Get Default # of Pages
    MOV     AH, 0               ; Hi Byte = 0
    PUSH    AX                  ; Push # Pages

    CALL    FAR PTR SET_VGA_MODEX   ; Set up Mode X!

@SX_Exit:

    POP     BP                  ; Restore Register
    RET     2                   ; We are Done.. Outa here

SET_MODEX   ENDP


;============================
;CLEAR_VGA_SCREEN (ColorNum%)
;============================
;
; Clears the currently active display page to 
; all the color #ColorNum 
;

CVS_STACK   STRUC
                DW  ?,?     ;DI, BP
                DD  ?       ;Caller
    CVS_COLOR   DB  ?,?     ;Color to Set Screen to 
CVS_STACK ENDS

    PUBLIC  CLEAR_VGA_SCREEN

CLEAR_VGA_SCREEN    PROC    FAR

    PUSH    BP                  ; Preserve Registers
    PUSH    DI

    MOV     BP, SP              ; Set up Stack Frame

    OUT_16  SC_INDEX, ALL_PLANES_ON ; Select All Planes
    LES     DI, d CURRENT_PAGE      ; Point to Active VGA Page

    MOV     AL, [BP].CVS_COLOR  ; Get Color
    MOV     AH, AL              ; Copy for Word Write
    CLD                         ; Block fill Forwards

    MOV     CX, PAGE_SIZE       ; Get Size of Page
    SHR     CX, 1               ; Divied by 2 for Words
    REP     STOSW               ; Block Fill VGA memory

    POP     DI                  ; Restore Saved Registers
    POP     BP

    RET     2                   ;We are Done.. Outa here

CLEAR_VGA_SCREEN    ENDP


;===================================
;SET_POINT (Xpos%, Ypos%, ColorNum%)
;===================================
;
; Sets a single Pixel at (Xpos, Ypos) on the active 
; Display Page to color #ColorNum 
;

SP_STACK    STRUC
                DW  ?   ;BP
                DD  ?   ;Caller
    SETP_Color  DB  ?,?
    SETP_Ypos   DW  ?
    SETP_Xpos   DW  ?
SP_STACK    ENDS

        PUBLIC SET_POINT

SET_POINT   PROC    FAR

    PUSH    BP                  ; Preserve Registers
    MOV     BP, SP              ; Set up Stack Frame

    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page

    MOV     AX, [BP].SETP_Ypos  ; Get Line # of Pixel
    MUL     SCREEN_WIDTH        ; Get Offset to Start of Line

    MOV     BX, [BP].SETP_Xpos  ; Get Xpos
    MOV     CX, BX
    SHR     BX, 2               ; X offset (Bytes) = Xpos/4
    ADD     BX, AX              ; Offset = Width*Ypos + Xpos/4

    MOV     AX, MAP_MASK_PLANE1 ; Map Mask & Plane Select Register
    AND     CL, PLANE_BITS      ; Get Plane Bits
    SHL     AH, CL              ; Get Plane Select Value
    MOV     DX, SC_Index        ; Setup to select plane
    OUT     DX, AX              ; Select Plane...

    MOV     AL,[BP].SETP_Color  ; Get Pixel Color
    MOV     ES:[DI+BX], AL      ; Draw Pixel

    POP     BP                  ; Restore Saved Registers
    RET     6                   ; Exit and Clean up Stack

SET_POINT        ENDP


;==========================
;READ_POINT% (Xpos%, Ypos%)
;==========================
;
; Returns the color of a pixel from the Active Display Page 
; 
; RETURNS:  AX = Color of Pixel at (Xpos, Ypos) 
;

RP_STACK    STRUC
            DW  ?   ;Bp
            DD  ?   ;Caller
    RP_Ypos DW  ?
    RP_Xpos DW  ?
RP_STACK    ENDS

        PUBLIC  READ_POINT

READ_POINT      PROC    FAR

    PUSH    BP                  ; Preserve Registers
    MOV     BP, SP              ; Set up Stack Frame

    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page

    MOV     AX, [Bp].RP_Ypos    ; Get Line # of Pixel
    MUL     SCREEN_WIDTH        ; Get Offset to Start of Line

    MOV     BX, [BP].RP_Xpos    ; Get Xpos
    MOV     CX, BX
    SHR     BX, 2               ; X offset (Bytes) = Xpos/4
    ADD     BX, AX              ; Offset = Width*Ypos + Xpos/4

    MOV     AL, READ_MAP        ; GC Read Mask Register
    MOV     AH, CL              ; Get Xpos
    AND     AH, PLANE_BITS      ; & mask out Plane #
    MOV     DX, GC_INDEX        ; Setup to select Read Mask
    OUT     DX, AX              ; Select Read Plane...

    XOR     AX, AX              ; Clear Return Value

    MOV     AL, ES:[DI+BX]      ; Get Color of Pixel

    POP     BP                  ; Restore Saved Registers
    RET     4                   ; Exit and Clean up Stack

READ_POINT        ENDP


;====================================================== 
;FILL_BLOCK (Xpos1%, Ypos1%, Xpos2%, Ypos2%, ColorNum%) 
;====================================================== 
; 
; Fills a block on the current display Page from Point 
; (Xpos1, Ypos1) to Point (Xpos2, Ypos2) in Color #ColorNum 
;

FB_STACK    STRUC
                DW  ?,?,?,? ; DS, DI, SI, BP
                DD  ?       ; Caller
    FB_Color    DB  ?,?     ; Fill Color
    FB_Ypos2    DW  ?       ; Lower Right Pixel
    FB_Xpos2    DW  ?       ;
    FB_Ypos1    DW  ?       ; Upper Left Pixel
    FB_Xpos1    DW  ?       ;
FB_STACK    ENDS

        PUBLIC    FILL_BLOCK

FILL_BLOCK  PROC    FAR

    PUSH    BP                  ; Save Registers
    PUSH    SI
    PUSH    DI
    PUSH    DS

    MOV     BP, SP              ; Set up Stack Frame

    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page
    CLD                         ; Direction Flag = Forward

    OUT_8   SC_INDEX, MAP_MASK  ; Set up for Plane Select

    ; Validate Pixel Coordinates
    ; If necessary, Swap so X1 <= X2, Y1 <= Y2

    MOV     AX, [BP].FB_Ypos1   ; AX = Y1   is Y1< Y2?
    MOV     BX, [BP].FB_Ypos2   ; BX = Y2
    CMP     AX, BX
    JLE     @FB_NOSWAP1

    MOV     [BP].FB_Ypos1, BX   ; Swap Y1 and Y2 and save Y1
    XCHG    AX, BX              ; on stack for future use

@FB_NOSWAP1:
    SUB     BX, AX              ; Get Y width
    INC     BX                  ; Add 1 to avoid 0 value
    MOV     [BP].FB_Ypos2, BX   ; Save in Ypos2

    MUL     SCREEN_WIDTH        ; Mul Y1 by Bytes per Line
    ADD     DI, AX              ; DI = Start of Line Y1

    MOV     AX, [BP].FB_Xpos1   ; Check X1 <= X2
    MOV     BX, [BP].FB_Xpos2   ;
    CMP     AX, BX
    JLE     @FB_NOSWAP2         ; Skip Ahead if Ok

    MOV     [BP].FB_Xpos2, AX   ; Swap X1 AND X2 and save X2
    XCHG    AX, BX              ; on stack for future use

    ; All our Input Values are in order, Now determine
    ; How many full "bands" 4 pixels wide (aligned) there
    ; are, and if there are partial bands (<4 pixels) on
    ; the left and right edges.

@FB_NOSWAP2:
    MOV     DX, AX              ; DX = X1 (Pixel Position)
    SHR     DX, 2               ; DX/4 = Bytes into Line
    ADD     DI, DX              ; DI = Addr of Upper-Left Corner

    MOV     CX, BX              ; CX = X2 (Pixel Position)
    SHR     CX, 2               ; CX/4 = Bytes into Line

    CMP     DX, CX              ; Start and end in same band?
    JNE     @FB_NORMAL          ; if not, check for l & r edges
    JMP     @FB_ONE_BAND_ONLY   ; if so, then special processing

@FB_NORMAL:
    SUB     CX, DX              ; CX = # bands -1
    MOV     SI, AX              ; SI = PLANE#(X1)
    AND     SI, PLANE_BITS          ; if Left edge is aligned then ..
    JZ      @FB_LEFT_PLANE_FLUSH    ; no special processing..

    ; Draw "Left Edge" vertical strip of 1-3 pixels...

    OUT_8   SC_Data, LEFT_CLIP_MASK[SI] ; Set Left Edge Plane Mask

    MOV     SI, DI              ; SI = Copy of Start Addr (UL)

    MOV     DX, [BP].FB_Ypos2   ; Get # of Lines to draw
    MOV     AL, [BP].FB_Color   ; Get Fill Color
    MOV     BX, SCREEN_WIDTH    ; Get Vertical increment Value

@FB_LEFT_LOOP:
    MOV     ES:[SI], AL         ; Fill in Left Edge Pixels
    ADD     SI, BX              ; Point to Next Line (Below)
    DEC     DX                  ; Lines to go--
    JZ      @FB_LEFT_CONT       ; Exit loop if all Lines Drawn

    MOV     ES:[SI], AL         ; Fill in Left Edge Pixels
    ADD     SI, BX              ; Point to Next Line (Below)
    DEC     DX                  ; Lines to go--
    JNZ     @FB_LEFT_LOOP       ; loop until left strip is drawn

@FB_LEFT_CONT:

    INC     DI                  ; Point to Middle (or Right) Block
    DEC     CX                  ; Reset CX insted of JMP @FB_RIGHT

@FB_LEFT_PLANE_FLUSH:
    INC     CX                  ; Add in Left band to middle block

    ;DI = Addr of 1st middle Pixel (band) to fill
    ;CX = # of Bands to fill -1

@FB_RIGHT:
    MOV     SI, [BP].FB_Xpos2       ; Get Xpos2
    AND     SI, PLANE_BITS          ; Get Plane values
    CMP     SI, 0003                ; Plane = 3?
    JE      @FB_RIGHT_EDGE_FLUSH    ; Hey, add to middle

    ; Draw "Right Edge" vertical strip of 1-3 pixels...

    OUT_8   SC_Data, RIGHT_CLIP_MASK[SI]    ; Set Right Edge Plane Mask

    MOV     SI, DI              ; Get Addr of Left Edge
    ADD     SI, CX              ; Add Width-1 (Bands)
    DEC     SI                  ; To point to top of Right Edge

    MOV     DX, [BP].FB_Ypos2   ; Get # of Lines to draw
    MOV     AL, [BP].FB_Color   ; Get Fill Color
    MOV     BX, SCREEN_WIDTH    ; Get Vertical increment Value

@FB_RIGHT_LOOP:
    MOV     ES:[SI], AL         ; Fill in Right Edge Pixels
    ADD     SI, BX              ; Point to Next Line (Below)
    DEC     DX                  ; Lines to go--
    JZ      @FB_RIGHT_CONT      ; Exit loop if all Lines Drawn

    MOV     ES:[SI], AL         ; Fill in Right Edge Pixels
    ADD     SI, BX              ; Point to Next Line (Below)
    DEC     DX                  ; Lines to go--
    JNZ     @FB_RIGHT_LOOP      ; loop until left strip is drawn

@FB_RIGHT_CONT:

    DEC     CX                  ; Minus 1 for Middle bands
    JCXZ    @FB_EXIT            ; Uh.. no Middle bands...

@FB_RIGHT_EDGE_FLUSH:

    ;DI = Addr of Upper Left block to fill
    ;CX = # of Bands to fill in (width)

    OUT_8   SC_Data, 0FH        ; Write to All Planes

    MOV     DX, SCREEN_WIDTH    ; DX = DI Incrament
    SUB     DX, CX              ;  = Screen_Width-# Planes Filled

    MOV     BX, CX              ; BX = Quick Refill for CX
    MOV     SI, [BP].FB_Ypos2   ; SI = # of Line to Fill
    MOV     AL, [BP].FB_Color   ; Get Fill Color

@FB_MIDDLE_LOOP:
    REP     STOSB               ; Fill in entire line

    MOV     CX, BX              ; Recharge CX (Line Width)
    ADD     DI, DX              ; Point to start of Next Line

    DEC     SI                  ; Lines to go--
    JNZ     @FB_MIDDLE_LOOP     ; Loop until all lines drawn

    JMP     @FB_EXIT            ; Outa here

@FB_ONE_BAND_ONLY:
    MOV     SI, AX                  ; Get Left CLip Mask, Save X1
    AND     SI, PLANE_BITS          ; Mask out Row #
    MOV     AL, LEFT_CLIP_MASK[SI]  ; Get Left Edge Mask
    MOV     SI, BX                  ; Get Right Clip Mask, Save X2
    AND     SI, PLANE_BITS          ; Mask out Row #
    AND     AL, RIGHT_CLIP_MASK[SI] ; Get Right Edge Mask byte

    MOV     DX, SC_Data         ; Plane Selection
    OUT     DX, AL              ; Clip For Left & Right Masks

    MOV     CX, [BP].FB_Ypos2   ; Get # of Lines to draw
    MOV     AL, [BP].FB_Color   ; Get Fill Color
    MOV     BX, SCREEN_WIDTH    ; Get Vertical increment Value

@FB_ONE_LOOP:
    MOV     ES:[DI], AL         ; Fill in Pixels
    ADD     DI, BX              ; Point to Next Line (Below)
    DEC     CX                  ; Lines to go--
    JZ      @FB_EXIT            ; Exit loop if all Lines Drawn

    MOV     ES:[DI], AL         ; Fill in Pixels
    ADD     DI, BX              ; Point to Next Line (Below)
    DEC     CX                  ; Lines to go--
    JNZ     @FB_ONE_LOOP        ; loop until left strip is drawn

@FB_EXIT:
    POP     DS
    POP     DI
    POP     SI
    POP     BP                  ; Restore Registers

    RET     10                  ; Exit and Clean up Stack

FILL_BLOCK   ENDP


;===================================================== 
;DRAW_LINE (Xpos1%, Ypos1%, Xpos2%, Ypos2%, ColorNum%) 
;===================================================== 
; 
; Draws a Line from (X1,Y1) to (X2,Y2) in Color #ColorNum 
; in VGA Mode X 
;

DL_STACK    STRUC
                DW  ?,?,?   ;DI, SI , BP
                DD  ?       ;Caller
    DL_ColorF   DB  ?,?
    DL_Ypos2    DW  ?
    DL_Xpos2    DW  ?
    DL_Ypos1    DW  ?
    DL_Xpos1    DW  ?
DL_STACK    ENDS

        PUBLIC DRAW_LINE

DRAW_LINE   PROC    FAR

    PUSH    BP                  ; Save Registers
    PUSH    SI
    PUSH    DI

    MOV     BP, SP              ; Set up Stack Frame
    CLD                         ; Direction Flag = Forward

    OUT_8   SC_INDEX, MAP_MASK  ; Set up for Plane Select

    MOV     CH, [BP].DL_ColorF  ; Save Color in CH

    ;Check Line Type

    MOV     SI, [BP].DL_Xpos1   ; AX = X1   is X1< X2?
    MOV     DI, [BP].DL_Xpos2   ; DX = X2
    CMP     SI, DI              ; Is X1 < X2
    JE      @DL_VLINE           ; If X1=X2, Draw Vertical Line
    JL      @DL_NOSWAP1         ; If X1 < X2, don't swap

    XCHG    SI, DI              ; X2 IS > X1, SO SWAP THEM

@DL_NOSWAP1:

    ; SI = X1, DI = X2

    MOV     AX, [BP].DL_Ypos1   ; AX = Y1   is Y1 <> Y2?
    CMP     AX, [BP].DL_Ypos2   ; Y1 = Y2?
    JE      @DL_HORZ            ; If so, Draw a Horizontal Line

    JMP     @DL_BREZHAM         ; Diagonal line... go do it...

    ; This Code draws a Horizontal Line in Mode X where:
    ; SI = X1, DI = X2, and AX = Y1/Y2

@DL_HORZ:

    MUL     SCREEN_WIDTH        ; Offset = Ypos * Screen_Width
    MOV     DX, AX              ; CX = Line offste into Page

    MOV     AX, SI                  ; Get Left Edge CLip Mask, Save X1
    AND     SI, PLANE_BITS          ; Mask out Row #
    MOV     BL, LEFT_CLIP_MASK[SI]  ; Get Left Edge Mask
    MOV     CX, DI                  ; Get Right Edge Clip Mask, Save X2
    AND     DI, PLANE_BITS          ; Mask out Row #
    MOV     BH, RIGHT_CLIP_MASK[DI] ; Get Right Edge Mask byte

    SHR     AX, 2               ; Get X1 Byte # (=X1/4)
    SHR     CX, 2               ; Get X2 Byte # (=X2/4)

    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page
    ADD     DI, DX              ; Point to Start of Line
    ADD     DI, AX              ; Point to Pixel X1

    SUB     CX, AX              ; CX = # Of Bands (-1) to set
    JNZ     @DL_LONGLN          ; If longer than one segment, go ahead

    AND     BL, BH              ; otherwise, megre clip masks

@DL_LONGLN:

    OUT_8   SC_Data, BL         ; Set the Left Clip Mask

    MOV     AL, [BP].DL_ColorF  ; Get Line Color
    MOV     BL, AL              ; BL = Copy of Line Color
    STOSB                       ; Set Left (1-4) Pixels

    JCXZ    @DL_EXIT            ; Done if only one Line Segment

    DEC     CX                  ; CX = # of Middle Segemnts
    JZ      @DL_XRSEG           ; If no middle segments....

    ;Draw Middle Segments

    MOV     AL, ALL_PLANES      ; Write to ALL Planes
    OUT     DX, AL              ; Select Planes

    MOV     AL, BL              ; Get Color from BL
    REP     STOSB               ; Draw Middle (4 Pixel) Segements

@DL_XRSEG:
    MOV     AL, BH              ; Get Right Clip Mask
    OUT     DX, AL              ; Select Planes

    MOV     AL, BL              ; Get Color Value
    STOSB                       ; Draw Right (1-4) Pixels

    JMP SHORT @DL_EXIT          ; We Are Done...

    ; This Code Draws A Vertical Line.  On entry:
    ; CH = Line Color, SI & DI = X1

@DL_VLINE:

    MOV     AX, [BP].DL_Ypos1   ; AX = Y1
    MOV     SI, [BP].DL_Ypos2   ; SI = Y2
    CMP     AX, SI              ; Is Y1 < Y2?
    JLE     @DL_NOSWAP2         ; if so, Don't Swap them

    XCHG    AX, SI              ; Ok, NOW Y1 < Y2

@DL_NOSWAP2:

    SUB     SI, AX              ; SI = Line Heigth (Y2-Y1+1)
    INC     SI

    ; AX = Y1, DI = X1, Get offset into Page into AX

    MUL     SCREEN_WIDTH        ; Offset = Y1 (AX) * Screen Width
    MOV     DX, DI              ; Copy Xpos into DX
    SHR     DI, 2               ; DI = Xpos/4
    ADD     AX, DI              ; DI = Xpos/4 + ScreenWidth * Y1

    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page
    ADD     DI, AX              ; Point to Pixel X1, Y1

    ;Select Plane

    MOV     CL, DL              ; CL = Save X1
    AND     CL, PLANE_BITS      ; Get X1 MOD 4 (Plane #)
    MOV     AX, MAP_MASK_PLANE1 ; Code to set Plane #1
    SHL     AH, CL              ; Change to Correct Plane #

    MOV     DX, SC_Index        ; Set VGA Sequencer
    OUT     DX, AX              ; Select Plane!

    MOV     AL, CH              ; Get Saved Color
    MOV     BX, SCREEN_WIDTH    ; Get Offset to Advance Line By

@DL_VLoop:
    MOV     ES:[DI], AL         ; Draw Single Pixel
    ADD     DI, BX              ; Point to Next Line

    DEC     SI                  ; Lines to draw--
    JNZ     @DL_VLoop           ; Loop until Done

@DL_EXIT:

    JMP @DL_EXIT2               ; Done!

    ; This code Draws a diagonal line in Mode X

@DL_BREZHAM:
    LES     DI, d CURRENT_PAGE  ; Point to Active VGA Page

    MOV     AX, [BP].DL_Ypos1   ; get y1 value
    MOV     BX, [BP].DL_Ypos2   ; get y2 value
    MOV     CX, [BP].DL_Xpos1   ; Get Starting Xpos

    CMP     BX, AX              ; Y2-Y1 is?
    JNC     @DL_DeltaYOK        ; if Y2>=Y1 then goto...

    XCHG    BX, AX              ; Swap em...
    MOV     CX, [BP].DL_Xpos2   ; Get New Starting Xpos

@DL_DeltaYOK:
    MUL     SCREEN_WIDTH        ; Offset = SCREEN_WIDTH * Y1

    ADD     DI, AX              ; DI -> Start of Line Y1 on Page
    MOV     AX, CX              ; AX = Xpos (X1)
    SHR     AX, 2               ; /4 = Byte Offset into Line
    ADD     DI, AX              ; DI = Starting pos (X1,Y1)

    MOV     AL, 11h             ; Staring Mask
    AND     CL, PLANE_BITS      ; Get Plane #
    SHL     AL, CL              ; and shift into placE
    MOV     AH, [BP].DL_ColorF  ; Color in Hi Bytes

    PUSH    AX                  ; Save Mask,Color...

    MOV     DX, SC_Index
    MOV     AH, AL              ; Plane # in AH
    MOV     AL, MAP_MASK        ; Select Plane Register
    OUT     DX, AX              ; Select initial plane

    MOV     AX, [BP].DL_Xpos1   ; get x1 value
    MOV     BX, [BP].DL_Ypos1   ; get y1 value
    MOV     CX, [BP].DL_Xpos2   ; get x2 value
    MOV     DX, [BP].DL_Ypos2   ; get y2 value

    MOV     BP, SCREEN_WIDTH    ; Use BP for Line width to
                                ; to avoid extra memory access

    SUB     DX, BX              ; figure delta_y
    JNC     @DL_DeltaYOK2       ; jump if y2>=y1

    ADD     BX, DX              ; put y2 into y1
    NEG     DX                  ; abs(Delta_y)
    XCHG    AX, CX              ; and exchange x1 and x2

@DL_DeltaYOK2:
    MOV     BX, 08000H          ; seed for fraction accumulator

    SUB     CX, AX              ; figure delta_x
    JC      @DL_DrawLeft        ; if negitive, go left

    JMP     @DL_DrawRight       ; Draw Line that slopes right

@DL_DrawLeft:

    NEG     CX                  ; abs(Delta_X)

    CMP     CX, DX              ; is X < Delta_Y?
    JB      @DL_SteepLeft       ; yes, so go do steep line (Delta_Y iterations)

    ; Draw a Shallow line to the left in Mode X

@DL_ShallowLeft:
    XOR     AX, AX              ; zero low word of Delta_y * 10000h
    SUB     AX, DX              ; DX:AX <- DX * 0FFFFh
    SBB     DX, 0
    DIV     CX                  ; divide by Delta_X

    MOV     SI, BX              ; Si = Acculmulator
    MOV     BX, AX              ; Bx = Addor
    POP     AX                  ; Get Color, Bit mask
    MOV     DX, SC_Data         ; Sequence controller data register
    INC     CX                  ; Inc Delta_X so we can unroll loop

    ; Loop (x2) to Draw Pixels, Move Left, and Maybe Down...

@DL_SLLLoop:
    MOV     ES:[DI], AH         ; set first pixel, plane data set up

    DEC     CX                  ; count down Delta_X
    JZ      @DL_SLLExit         ; Exit if done..

    ADD     SI, BX              ; add numerator to accumulator
    JNC     @DL_SLLL2nc         ; move down on carry

    ADD     DI, BP              ; Move Down one line...

@DL_SLLL2nc:
    DEC     DI                  ; Left one addr
    ROR     AL, 1               ; Move Left one plane
    CMP     AL, 87h             ; Skipped around?, if AL <88 then Carry set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

    MOV     ES:[DI], AH         ; set pixel

    ADD     SI, BX              ; add numerator to accumulator,
    JNC     @DL_SLLL3nc         ; move down on carry

    ADD     DI, BP              ; Move Down one line...

@DL_SLLL3nc:                    ; Now move left a pixel...
    DEC     DI                  ; Left one addr
    ROR     AL, 1               ; Move Left one plane
    CMP     AL, 87h             ; Skipped around?, if AL <88 then Carry set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

    DEC     CX                  ; count down Delta_X
    JNZ     @DL_SLLLoop         ; loop till done

@DL_SLLExit:
    JMP     @DL_EXIT2           ; and exit

    ; Draw a steep line to the left in Mode X

@DL_SteepLeft:
    XOR     AX, AX              ; zero low word of Delta_y * 10000h
    XCHG    DX, CX              ; Delta_Y switched with Delta_x
    DIV     CX                  ; divide by Delta_Y

    MOV     SI, BX              ; Si = Acculmulator
    MOV     BX, AX              ; Bx = Addor
    POP     AX                  ; Get Color, Bit mask
    MOV     DX, SC_Data         ; Sequence controller data register
    INC     CX                  ; Inc Delta_Y so we can unroll loop

    ; Loop (x2) to Draw Pixels, Move Down, and Maybe left

@DL_STLLoop:

    MOV     ES:[DI], AH         ; set first pixel
    DEC     CX                  ; Count Down Delta_Y
    JZ      @DL_STLExit         ; Exit if done

    ADD     SI, BX              ; add numerator to accumulator
    JNC     @DL_STLnc2          ; No carry, just move down!

    ; Move Left

    DEC     DI                  ; Left one addr
    ROR     AL, 1               ; Move Left one plane
    CMP     AL, 87h             ; Skipped around?, if AL <88 then Carry set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

@DL_STLnc2:
    ADD     DI, BP              ; advance to next line.

    MOV     ES:[DI], AH         ; set pixel
    ADD     SI, BX              ; add numerator to accumulator
    JNC     @DL_STLnc3          ; No carry, just move down!

    ;Move Left

    DEC     DI                  ; Left one addr
    ROR     AL, 1               ; Move Left one plane
    CMP     AL, 87h             ; Skipped around?, if AL <88 then Carry set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

@DL_STLnc3:
    ADD     DI, BP              ; advance to next line.

    DEC     CX                  ; count down Delta_Y
    JNZ     @DL_STLLoop         ; loop till done

@DL_STLExit:
    JMP     @DL_EXIT2           ; and exit

    ; Draw A line that goes to the Right...

@DL_DrawRight:
    CMP     CX, DX              ; is X < Delta_Y?
    JB      @DL_SteepRight      ; yes, so go do steep line (Delta_Y iterations)

    ; Draw a Shallow line to the Right in Mode X

@DL_ShallowRight:
    XOR     AX, AX              ; zero low word of Delta_y * 10000h
    SUB     AX, DX              ; DX:AX <- DX * 0FFFFh
    SBB     DX, 0
    DIV     CX                  ; divide by Delta_X

    MOV     SI, BX              ; Si = Acculmulator
    MOV     BX, AX              ; Bx = Addor
    POP     AX                  ; Get Color, Bit mask
    MOV     DX, SC_Data         ; Sequence controller data register
    INC     CX                  ; Inc Delta_X so we can unroll loop

    ; Loop (x2) to Draw Pixels, Move Right, and Maybe Down...

@DL_SLRLoop:
    MOV     ES:[DI], AH         ; set first pixel, mask already set up

    DEC     CX                  ; count down Delta_X
    JZ      @DL_SLRExit         ; Exit if done..

    ADD     SI, BX              ; add numerator to accumulator
    JNC     @DL_SLR2nc          ; don't move down if carry not set....

    ADD     DI, BP              ; Move Down one line...

@DL_SLR2nc:                     ; Now move right a pixel...
    ROL     AL, 1               ; Move Right one plane
    CMP     AL, 12h             ; Skipped around?, if AL >12 then Carry not set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

    MOV     ES:[DI], AH         ; set pixel
    ADD     SI, BX              ; add numerator to accumulator
    JNC     @DL_SLR3nc          ; don't move down if carry not set....

    ADD     DI, BP              ; Move Down one line...

@DL_SLR3nc:
    ROL     AL, 1               ; Move Right one plane
    CMP     AL, 12h             ; Skipped around?, if AL >12 then Carry not set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

    DEC     CX                  ; count down Delta_X
    JNZ     @DL_SLRLoop         ; loop till done

@DL_SLRExit:
    JMP     @DL_EXIT2           ; and exit

    ; Draw a Shallow line to the Right in Mode X

@DL_SteepRight:
    XOR     AX, AX              ; zero low word of Delta_y * 10000h
    XCHG    DX, CX              ; Delta_Y switched with Delta_x
    DIV     CX                  ; divide by Delta_Y

    MOV     SI, BX              ; Si = Acculmulator
    MOV     BX, AX              ; Bx = Addor
    POP     AX                  ; Get Color, Bit mask
    MOV     DX, SC_Data         ; Sequence controller data register
    INC     CX                  ; Inc Delta_Y so we can unroll loop

    ; Loop (x2) to Draw Pixels, Move Down, and Maybe Right

@STRLoop:
    MOV     ES:[DI], AH         ; set first pixel, mask already set up...

    DEC     CX                  ; count down Delta_Y
    JZ      @DL_EXIT2           ; Exit if Finished

    ADD     SI, BX              ; add numerator to accumulator
    JNC     @STRnc2             ; if no carry then just go down...

    ROL     AL, 1               ; Move Right one plane
    CMP     AL, 12h             ; Skipped around?, if AL >12 then Carry not set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

@STRnc2:
    ADD     DI, BP              ; advance to next line.

    MOV     ES:[DI], AH         ; set pixel
    ADD     SI, BX              ; add numerator to accumulator
    JNC     @STRnc3             ; if no carry then just go down...

    ROL     AL, 1               ; Move Right one plane
    CMP     AL, 12h             ; Skipped around?, if AL >12 then Carry not set
    ADC     DI, 0               ; Adjust Address: DI = Di + Carry, ie. plane =
    OUT     DX, AL              ; Set up New Bit Plane mask

@STRnc3:
    ADD     DI, BP              ; advance to next line.

    DEC     CX                  ; count down Delta_Y
    JNZ     @STRLoop            ; loop till done

@DL_EXIT2:

    POP     DI
    POP     SI
    POP     BP                  ; Restore BP, SI, & DI

    RET     10                  ; Exit and Clean up Stack

DRAW_LINE        ENDP


;=========================
;SET_ACTIVE_PAGE (PageNo%)
;=========================
;
;Sets the Video Page to be used for future drawing 
;

SAP_STACK   STRUC
                DW  ?       ;BP
                DD  ?       ;Caller
    SAP_Page    DW  ?       ;Page # for Drawing 
SAP_STACK   ENDS

    PUBLIC  SET_ACTIVE_PAGE

SET_ACTIVE_PAGE PROC    FAR

    PUSH    BP                  ; Preserve Registers
    MOV     BP, SP              ; Set up Stack Frame

    MOV     BX, [BP].SAP_Page   ; Get Desired Page #
    CMP     BX, LAST_PAGE       ; Is Page # Valid?
    JAE     @SAP_Exit           ; IF Not, Do Nothing

    MOV     ACTIVE_PAGE, BX     ; Set Active Page #

    SHL     BX, 1               ; Scale Page # to Word
    MOV     AX, PAGE_ADDR[BX]   ; Get offset to Page

    MOV     CURRENT_PAGE, AX    ; And set for future LES's

@SAP_Exit:
    POP     BP                  ; Restore Registers

    RET     2                   ; We are Done.. Outa here

SET_ACTIVE_PAGE ENDP


;================
;GET_ACTIVE_PAGE%
;================
;
; Returns the Video Page # currently used for Drawing 
; 
; RETURNS:   AX = Current Video Page used for Drawing 
;

    PUBLIC  GET_ACTIVE_PAGE

GET_ACTIVE_PAGE PROC    FAR

    MOV     AX, ACTIVE_PAGE     ; Get Active Page #

    RET                         ; We are Done.. Outa here

GET_ACTIVE_PAGE ENDP


;==========================
;SET_DISPLAY_PAGE (PageNo%)
;==========================
;
;Sets the Video Page to be displayed on the screen 
;


SDP_STACK   STRUC
                DW  ?       ;BP
                DD  ?       ;Caller
    SDP_Page    DW  ?       ;Page # to Display... 
SDP_STACK   ENDS

    PUBLIC  SET_DISPLAY_PAGE

SET_DISPLAY_PAGE    PROC    FAR

    PUSH    BP                  ; Preserve Registers
    MOV     BP, SP              ; Set up Stack Frame

    MOV     BX, [BP].SDP_Page   ; Get Desired Page #
    CMP     BX, LAST_PAGE       ; Is Page # Valid?
    JAE     @SDP_Exit           ; IF Not, Do Nothing

    MOV     DISPLAY_PAGE, BX    ; Set Display Page #

    SHL     BX, 1               ; Scale Page # to Word
    MOV     CX, PAGE_ADDR[BX]   ; Get offset in memory to Page

    ADD     CX, CURRENT_MOFFSET ; Adjust for any scrolling

    MOV     DX, CRTC_Index      ; We Change the VGA Sequencer
    MOV     AL, START_DISP_LO   ; Display Start Low Register
    MOV     AH, CL              ; Low 8 Bits of Start Addr
    OUT     DX, AX              ; Set Display Addr Low

    MOV     AL, START_DISP_HI   ; Display Start High Register
    MOV     AH, CH              ; High 8 Bits of Start Addr
    OUT     DX, AX              ; Set Display Addr High

@SDP_Exit:
    POP     BP                  ; Restore Registers

    RET     2                   ; We are Done.. Outa here

SET_DISPLAY_PAGE    ENDP


;=================
;GET_DISPLAY_PAGE%
;=================
;
; Returns the Video Page # currently be displayed on the screen 
; 
; RETURNS: AX = Current Display Page 
;

    PUBLIC  GET_DISPLAY_PAGE

GET_DISPLAY_PAGE    PROC    FAR

    MOV     AX, DISPLAY_PAGE    ; Get Display Page #

    RET                         ; We are Done.. Outa here

GET_DISPLAY_PAGE    ENDP


;================================================= 
;SET_DAC_REGISTER (Register%, Red%, Green%, Blue%) 
;================================================= 
; 
; Sets a single (RGB) Vga Palette Register 
; to the specified Reg, Green & Blue Values 
;

SDR_STACK   STRUC
                    DW  ?   ; BP
                    DD  ?   ; Caller
    SDR_Blue        DB  ?,? ; Blue Data Value
    SDR_Green       DB  ?,? ; Green Data Value
    SDR_Red         DB  ?,? ; Red Data Value
    SDR_Register    DB  ?,? ; Palette Register # 
SDR_STACK   ENDS

    PUBLIC  SET_DAC_REGISTER

SET_DAC_REGISTER    PROC    FAR

    PUSH    BP                  ; Save Bp
    MOV     BP, SP              ; Set up Stack Frame

    ; Select which DAC Register to modify

    OUT_8   DAC_WRITE_ADDR, [BP].SDR_Register

    MOV     DX, PEL_DATA_REG    ; Dac Data Register

    MOV     AL, [Bp].SDR_Red    ; Get Red Intensity
    OUT     DX, AL              ; Set it

    MOV     AL, [Bp].SDR_Green  ; Get Green Intensity
    OUT     DX, AL              ; Set it

    MOV     AL, [Bp].SDR_Blue   ; Get Blue Intensity
    OUT     DX, AL              ; Set it

    POP     BP                  ; Restore Registers
    RET     8                   ; Exit & Clean Up Stack

SET_DAC_REGISTER    ENDP


;==================================================== 
;GET_DAC_REGISTER (Register%, &Red%, &Green%, &Blue%) 
;==================================================== 
; 
; Gets the RGB Values of a single Vga Palette Register 
; 
; INPUT:  Red%, Green% & Blue% are offsets into DGROUP 
;         that point to 16 bit integers that will hold 
;         the returned red, green, and blue values 
;

GDR_STACK   STRUC
                    DW  ?   ; BP
                    DD  ?   ; Caller
    GDR_Blue        DW  ?   ; Addr of Blue Data Value in DS
    GDR_Green       DW  ?   ; Addr of Green Data Value in DS
    GDR_Red         DW  ?   ; Addr of Red Data Value in DS
    GDR_Register    DB  ?,? ; Palette Register # 
GDR_STACK   ENDS

    PUBLIC  GET_DAC_REGISTER

GET_DAC_REGISTER    PROC    FAR

    PUSH    BP                  ; Save Bp
    MOV     BP, SP              ; Set up Stack Frame

    ; Select which DAC Register to read in

    OUT_8   DAC_READ_ADDR, [BP].GDR_Register

    MOV     DX, PEL_DATA_REG    ; Dac Data Register
    XOR     AX, AX              ; Clear AX

    IN      AL, DX              ; Read Red Value
    MOV     BX, [Bp].GDR_Red    ; Get Address of Red%
    MOV     [BX], AX            ; *Red% = AX

    IN      AL, DX              ; Read Green Value
    MOV     BX, [Bp].GDR_Green  ; Get Address of Green%
    MOV     [BX], AX            ; *Green% = AX

    IN      AL, DX              ; Read Blue Value
    MOV     BX, [Bp].GDR_Blue   ; Get Address of Blue%
    MOV     [BX], AX            ; *Blue% = AX

    POP     BP                  ; Restore Registers
    RET     8                   ; Exit & Clean Up Stack

GET_DAC_REGISTER    ENDP


;=========================
;SET_WINDOW (Xpos%, Ypos%)
;=========================
;
; Since a Logical Screen can be larger than the Physical 
; Screen, Scrolling is possible.  This routine sets the 
; Upper Left Corner of the Screen to the specified Pixel. 
; 
; Xpos & Ypos  = Coordinates of the Pixel to put in the 
;                upper left corner of the screen 
;

SW_STACK    STRUC
                DW  ?   ;BP
                DD  ?   ;Caller
    SW_Ypos     DW  ?
    SW_Xpos     DW  ?
SW_STACK    ENDS

        PUBLIC SET_WINDOW

SET_WINDOW  PROC    FAR

    PUSH    BP                  ; Preserve Registers
    MOV     BP, SP              ; Set up Stack Frame

    ; Check if our Scroll Offsets are Valid

    MOV     AX, [BP].SW_Ypos    ; Get Desired Y Offset
    CMP     AX, MAX_YOFFSET     ; Is it Within Limitss?
    JA      @SW_Exit            ; if not, exit

    MOV     BX, [BP].SW_Xpos    ; Get Desired X Offset
    CMP     BX, MAX_XOFFSET     ; Is it Within Limitss?
    JA      @SW_Exit            ; if not, exit

    ; Compute proper Display start address to use

    MUL     SCREEN_WIDTH        ; AX = YOffset * Line Width
    MOV     CX, BX              ; CX = Copy of X Offset
    SHR     BX, 2               ; BX / 4 = Bytes into Line
    ADD     AX, BX              ; AX = Offset of Upper Left Pixel

    MOV     CURRENT_MOFFSET, AX ; Save Offset Info

    MOV     BX, DISPLAY_PAGE    ; Get Display Page #
    SHL     BX, 1               ; Scale Page # to Word
    ADD     AX, PAGE_ADDR[BX]   ; Get offset in VGA to Page
    MOV     BX, AX              ; BX = Desired Display Start

    MOV     DX, INPUT_1         ; Input Status #1 Register

    ; Wait for a Vertical Retrace to smooth out the scroll

@SW_WAIT:
    IN      AL, DX              ; Get VGA stauts
    JMP     $+2                 ; Delay (why?)
    AND     AL, 8               ; Bit 3 Gone Hi Yet?
    JZ      @SW_WAIT            ; If Not, wait for it
    CLI                         ; Don't Interrupt this!

    ; Set the Start Display Address to the New window

    MOV     DX, CRTC_Index      ; We Change the VGA Sequencer
    MOV     AL, START_DISP_LO   ; Display Start Low Register
    MOV     AH, BL              ; Low 8 Bits of Start Addr
    OUT     DX, AX              ; Set Display Addr Low

    MOV     AL, START_DISP_HI   ; Display Start High Register
    MOV     AH, BH              ; High 8 Bits of Start Addr
    OUT     DX, AX              ; Set Display Addr High

    ; Now Set the Horizontal Pixel Pan values

    MOV     DX, INPUT_1         ; Input Status #1 Register
    IN      AL, DX              ; Reset Attrib Flip/Flop

    OUT_8   ATTRIB_Ctrl, 33h    ; Select Pixel Pan Register

    MOV     AL, CL              ; Get Low Bits of X Offset
    AND     AL, 03              ; Get # of Pixels to Pan (0-3)
    SHL     AL, 1               ; Shift for 256 Color Mode
    OUT     DX, AX              ; Fine tune the display!
    STI                         ; Ok, Now you CAN interrupt

@SW_Exit:
    POP     BP                  ; Restore Saved Registers
    RET     4                   ; Exit and Clean up Stack

SET_WINDOW        ENDP

    END
