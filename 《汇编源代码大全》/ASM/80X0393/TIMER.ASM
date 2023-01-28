;***************************************
;* Sample code to change the timer value
;* Written by Dave M. Walker
;*
;* Frequency for 8253 = 4.77MHz/4 = 1192500
;* Default BIOS countdown = 65536 = 18.2/sec
;***************************************
                IDEAL
                MODEL   TINY

                CODESEG
                ORG     0100h
Start:          jmp     Begin

PSPlen          equ     0100h                   ;Used to calc. TSR length

VideoSeg        equ     0b800h                  ;For color
;VideoSeg       equ     0b000h                  ;For mono

NewCount        equ     582                     ;2048/sec

CmdReg8253      equ     43h                     ;8253 Command Register port
TickChannel     equ     40h                     ;8253 System Timer channel
TickInt         equ     08h                     ;IRQ0 = INT 08

Warning$        db      13,10
                db      'WARNING!  This program reprograms the 8253'
                db      ' timer chip.  If the system is running under a'
                db      ' multitasker, the system may hang or behave'
                db      ' abnormally.',13,10,'Continue (Y/N)? $'

Begin:          mov     dx,OFFSET Warning$      ;Warn user that we're going
                mov     ah,9                    ;  to mess with the timer
                int     21h
KeyLoop:        mov     ah,8                    ;Get Y/N response
                int     21h
                xor     ah,ah
                or      al,al
                jne     GotKey
                mov     ah,8                    ;Handle extended keycode
                int     21h
                xor     ah,ah                   ;Make it negative
                neg     ax

GotKey:         cmp     ax,3                    ; ^C = Abort
                je      Abort
                cmp     ax,27                   ;ESC = Abort
                je      Abort
                cmp     ax,78                   ;"N" = Abort
                je      Abort
                cmp     ax,110                  ;"n" = Abort
                je      Abort
                cmp     ax,89                   ;"Y" = Continue
                je      Continue
                cmp     ax,121                  ;"y" = Continue
                je      Continue
                mov     dl,7                    ;Invalid response - beep!
                mov     ah,2
                int     21h
                jmp     KeyLoop                 ;Try again

Abort:          mov     ax,4c00h                ;Back to DOS
                int     21h

Continue:       mov     al,TickInt              ;Get old timer vector
                mov     ah,35h
                int     21h
                mov     [OldSeg],es             ;Save it
                mov     [OldOfs],bx

                mov     dx,OFFSET NewISR        ;Set new timer vector
                mov     ah,25h                  ;  (DS already correct)
                int     21h
                mov     al,36h                  ;Reprogram 8253 countdown
                out     CmdReg8253,al           ;  register
                mov     ax,NewCount
                out     TickChannel,al          ;Send LSB
                mov     al,ah
                out     TickChannel,al          ;Send MSB

                mov     dx,Paragraphs           ;Calculate size of routine
                mov     ax,3100h                ;TSR with errorlevel 0
                int     21h

;* New timer ISR and data

SlowCounter     db      4                       ;Counter for generating
                                                ;  1/512 sec interval
BIOSCounter     dw      0                       ;Counter for BIOS's handler

LABEL           OldVector DWORD                 ;BIOS's timer vector
OldOfs          dw      ?                       ;  Offset portion
OldSeg          dw      ?                       ;  Segment portion

NewISR:         call    FastHook                ;Fast routine always called
                dec     [cs:SlowCounter]        ;If --counter != 0
                jne     SkipSlow                ;  not time yet...
                call    SlowHook                ;else call the slow routine
                mov     [cs:SlowCounter],4      ;  and reset counter
SkipSlow:       add     [cs:BIOSCounter],NewCount ;Update BIOS counter
                jno     SkipBIOS                ;If no overflow, skip
                pushf                           ;else simulate a clock tick
                call    [DWORD cs:OldVector]    ;  interrupt
                iret                            ;BIOS already ACK'ed the INT

SkipBIOS:       push    ax
                mov     al,20h                  ;Acknowledge the INT by
                out     20h,al                  ;  resetting the 8259 PIC
                pop     ax
                iret

;* This is the fast interrupt procedure.  All registers except the flags
;* MUST be preserved, including segment registers.  The ONLY register that
;* has a known value at the beginning of this procedure is CS.

FastNum         dw      0                       ;Counter

FastHook:       push    dx di                   ;Save regs

                mov     dx,[cs:FastNum]         ;Get our counter
                inc     [cs:FastNum]            ;Update it
                mov     di,(0)*160+(79)*2       ;Right edge at (0,79)
                call    DispNum                 ;Display it

                pop     di dx
                ret

;* This is the slow interrupt procedure.  Same rules apply.

SlowNum         dw      0                       ;Counter

SlowHook:       push    dx di                   ;Save regs

                mov     dx,[cs:SlowNum]         ;Get our counter
                inc     [cs:SlowNum]            ;Update it
                mov     di,(0)*160+(74)*2       ;Right edge at (0,74)
                call    DispNum                 ;Display it

                pop     di dx
                ret

DispNum:        push    ax cx es

                mov     ax,VideoSeg             ;Point ES to video
                mov     es,ax
                mov     ah,0fh                  ;Attribute for display
                mov     cx,0404h                ;Set CL=4 for shifts, and
                                                ;  CH=4 for loop counter
                std                             ;Going backwards!
DispNumLoop:    call    DispDigit               ;Display a digit
                shr     dx,cl                   ;Rotate for next digit
                dec     ch
                jnz     DispNumLoop             ;Do it four times

                pop     es cx ax
                ret

DispDigit:      mov     al,dl                   ;Get low byte
                and     al,0fh                  ;Keep only low nybble
                add     al,'0'                  ;Convert to hex ASCII
                cmp     al,'9'
                jbe     NotLetter
                add     al,7                    ;Adjust for letter
NotLetter:      stosw                           ;Store digit/attribute
                ret

Paragraphs      =       ($-Start+PSPlen+15) SHR 4

                END     Start
