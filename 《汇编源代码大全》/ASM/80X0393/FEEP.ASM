;
; FEEP  Timer interrupt diagnostic checker.
;       Beeps while timer interrupt (int 08) is being serviced.
;       Released to the public domain by Sam Warden 1992/10/13.
;       TSR.  Source compatible with MS MASM 5.1.
;
COUNT           EQU     91                  ; = 18.2 x 5 (5 sec. interval)
;
_TEXT           SEGMENT PARA    PUBLIC  'CODE'
                ASSUME  cs:_TEXT, ds:_TEXT, es:_TEXT, ss:_TEXT
                ORG     100h
;
main:           jmp     SHORT init
;
counter         DB      COUNT
feeping         DB      0
oldvec          DD      0
;
handler:                                    ; INT 08 handler
                pushf                       ; 1st call original handler
                call    dword ptr cs:oldvec
;
                cmp     cs:feeping,1        ; if already feeping
                jne     feep
                push    ax
                in      al,61h              ;   stop
                and     al,0FCh
                jmp     SHORT   $ + 2
                out     61h,al
                pop     ax
                mov     cs:feeping,0
feep:
                dec     cs:counter
                jnz     go_on               ; if counter = 0
                mov     cs:counter,COUNT    ;   reset
                push    ax
                in      al,61h              ;   and feep
                or      al,3
                jmp     SHORT $ + 2
                out     61h,al
                pop     ax
                mov     cs:feeping,1
go_on:          iret
;
tsr_end         EQU     $
;
init:           mov     es,ds:[002Ch]       ; get pointer to env. from PSP
                mov     ah,49h              ; release environment memory
                int     21h
                mov     ax,3508h            ; get timer interrupt vector
                int     21h                 ;   in es:bx
                mov     word ptr oldvec,bx  ;   and store it
                mov     word ptr oldvec+2,es
                mov     ax,2508h            ; set new timer vector
                mov     dx,OFFSET handler
                int     21h
                mov     dx,OFFSET tsr_end   ; and go resident
                int     27h
;
_TEXT           ENDS
                END     main
