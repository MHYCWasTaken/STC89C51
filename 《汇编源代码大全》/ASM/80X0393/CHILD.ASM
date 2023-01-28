;
;  CHILD.ASM
;
;  Author:  Dave Walker
;  Placed in the public domain
;

            IDEAL
            MODEL   SMALL
            STACK   100h

            DATASEG
keep_ss     dw      ?                   ;For saving our stack
keep_sp     dw      ?
PSPseg      dw      ?
childspec   db      'd:\subdir\child.exe',0
parmblk     dw      0                   ;Use our environment
            dd      0                   ;Command tail
            dd      0                   ;Pointer to first FCB
            dd      0                   ;Pointer to second FCB

            CODESEG
start:      mov     ax,@data            ;Setup DS
            mov     ds,ax
            mov     [PSPseg],es         ;Save PSP segment for later use

;*** Reallocate memory
            mov     bx,zseg             ;Our size is calculated by
            mov     ax,es               ;  subtracting the address
            sub     bx,ax               ;  of a "end marker" segment
            mov     ah,4ah              ;  from our PSP segment.
            int     21h

;[...go on about our business for a while...]

;*** Setup parmblk for executed program.  For simplicity, we are using
;***   the parent's environment block and PSP.

;*** The parameter block contains four items:
;***   +00  WORD   Segment of child process' environment block (this
;***                 will normally be the same as the parent's)
;***   +02  DWORD  Address of command tail for child process (seg:ofs)
;***   +06  DWORD  Address of first FCB (normally PSP:005Ch)
;***   +0A  DWORD  Address of second FCB (normally PSP:006Ch)

            mov     es,[PSPseg]         ;ES = Our PSP seg.
            mov     ax,[es:002ch]       ;AX = [PSP:002Ch] = Our
            mov     [WORD parmblk],ax   ;  environment seg.

            mov     ax,0080h            ;Command tail is at PSP:0080h
            mov     [WORD parmblk+2],ax
            mov     [WORD parmblk+4],es

            mov     ax,005ch            ;FCB1 is at PSP:005Ch
            mov     [WORD parmblk+6],ax
            mov     [WORD parmblk+8],es

            mov     ax,006ch            ;FCB2 is at PSP:006Ch
            mov     [WORD parmblk+10],ax
            mov     [WORD parmblk+12],es

;*** Load and execute child process
            push    ds                  ;Point ES:BX to parmblk
            pop     es
            mov     bx,OFFSET parmblk
            mov     dx,OFFSET childspec ;Point DS:DX to ASCIIZ filespec
            mov     [keep_ss],ss        ;Save our stack pointer
            mov     [keep_sp],sp
            mov     ax,4b00h            ;Execute child process
            int     21h

;*** After the child process returns, we can't rely on *any* registers
;*** to be intact.
            mov     ax,@data            ;Reload DS
            mov     ds,ax
            mov     ss,[keep_ss]        ;Reload stack
            mov     sp,[keep_sp]

;*** Get return code via 21/4D (if desired)
            mov     ah,4dh
            int     21h

;*** Return to DOS
            mov     ax,4c00h
            int     21h

SEGMENT     zseg
ENDS        zseg

            END     start
