;TOADICE Unscrambles a .COM file scrambled by ICE v1.0
;       Usage:  TOADICE program.com
;       Warning:  Overwrites the original "in situ", so be sure
;       to have it saved somewhere in case there's a problem.
;
;       Released to the public domain.
;       Please do not distribute without documentation and source.
;       David Kirschbaum
;       Toad Hall
;       Fayetteville NC         (919) 868-3471
;       kirsch@sesi.com

CR      EQU     0DH
LF      EQU     0AH
PROGCODE EQU    79H             ;encrypted program code starting offset

;This structure replicates the beginning of an ICE'd program
icehdr  STRUC
        db      ?,?     ;EB,0A          ;'JMP Start'                    ;100
wxor    dw      ?                       ;magic XOR word                 ;102
ptr127  dw      ?       ;0127H          ;Ptr to 127H, just past loader  ;104
                                        ;appears to be unused
proglen dw      ?                       ;nr bytes in encrypted program  ;106
decodlen dw     ?                       ;nr bytes in encrypted decoder  ;108
setup   dw      ?                       ;Address of runtime setup code  ;10A
                                        ;(just beyond encrypted code)
Start   dw      ?       ;BE 26 01       ;MOV SI,126H                    ;10C
        ENDS

CSEG    SEGMENT
        ASSUME DS:CSEG, SS:CSEG ,CS:CSEG ,ES:CSEG
        ORG     100H

ToadIce PROC    FAR
handle  label   word                    ;Waste not, want not, eh?
        call    OpenFile                ;open, set up to read target ICEd file
        int     21H
        jnb     ReadOk                  ;all went ok
         mov    dx,offset readerr       ;'File read error'
         jmp    MsgTerm                 ;(DOS'll close the open file)

logo    db      'TOADICE: $'
readerr db      'File read error$'
createrr db     'File create error$'
writerr db      'File write error$'
noticed db      'File is not ICEd$'
okmsg   db      'deICEd$'

ReadOk:
;BX = file handle
;DX = input buffer (b)
;SI = AsciiZ filename

        mov     ah,3EH                  ;close file
        int     21H
;Check to see if the program was in fact ICE'd.
        mov     bx,dx                   ;b and header structure
        cmp     [bx],0AEBH              ;EB 0A, JMP 010CH
        jnz     NoMatch                 ;nope, not ICEd
        cmp     [bx].ptr127,127H        ;constant in header
        jnz     NoMatch                 ;nope
        cmp     [bx].Start,26BEH        ;BE 26 01, MOV SI,126
        jz      Iced                    ;Enough already
NoMatch:
        mov     dx,offset noticed       ;'Not iced'
        mov     al,0FFH                 ;ERRORLEVEL
        jmp     MsgTerm

Iced:                                   ;Ok, so it's an ICEd .COM file.
;Now we "create" a file with the same name (saved in SI)
;just so we can write back out to it.
;(This is the only way to "truncate" it cleanly.)

        mov     dx,si                   ;AsciiZ file name from PSP cmdline
        xor     cx,cx                   ;normal file attributes
        mov     ah,3CH                  ;create a file
        int     21H
        jnb     CreateOk
         mov    dx,offset createrr      ;'Create error'
         jmp    MsgTerm                 ;AL=ERRORLEVEL

CreateOk:
        mov     handle,ax               ;save file handle
        mov     si,offset b + 26H       ;start decoding the mover/decoder,
                                        ;plus the encoded program
                                        ;(yes, we must do it all)
        mov     di,si                   ;put decoded words right back
        mov     cx,b.decodlen           ;nr of words to decode
        mov     dx,b.wxor               ;magic XOR value for decoding
        cld                             ;insure fwd
DecodeLup:
        lodsw
        xor     ax,dx                   ;decode
        stosw                           ;put it back
        mov     dx,ax                   ;new XOR value
        loop    DecodeLup               ;do the entire mover/decoder

;This portion (a tweaked copy of the original decoder) moves decoded but
;possibly compressed program code/data to the stack, and then copies it
;back (possibly decompressing it) to the same place.

;This is all very odd, because if it *had* been truly compressed,
;it should now overwrite ICE's setup code that lies beyond
;the compressed program code.
;Methinks they lie to us ...  ICE'd programs will *always* be larger
;than the original, because that program area will *always* be as
;large as the original (uncompressed) program.

        mov     si,offset b + PROGCODE  ;from start of encrypted code
        push    si                      ;save for DX (write to disk)
        push    si                      ;save for DI later (putback)
        mov     cx,b.proglen            ;nr bytes to move
        inc     cx                      ;adjust
        push    cx                      ;save nr bytes
        mov     di,0FEFFH               ;to temporary work area in stack
        add     si,cx                   ;plus length
        dec     si                      ;adjust start point
        std                             ;backwards
        repz    movsb                   ;copy encrypted code to himem
        mov     si,di                   ;where we stopped
        inc     si                      ;from
        pop     cx                      ;same nr bytes
        pop     di                      ;b+PROGCODE: same place
        cld                             ;forward
ExpandLup:
        lodsb                           ;next encrypted byte
        cmp     cx,0                    ;sign set (e.g., big value)
        jl      Chk_RLC                 ;yep, check for compression
         cmp     cx,2
         jng     Relup                  ;too close to end, no expansion
Chk_RLC:
        cmp     al,0D5H                 ;special flag for RLC (run-length
compression)
        jz      GotRlc                  ;yep, that's it
         stosb                          ;regular char, stuff it
         jmp    short Relup

GotRlc: lodsw                           ;repeated char count (AH), char (AL)
        dec     cx                      ;adjust loop counter
        dec     cx                      ;by 2
        push    cx                      ;save it
        xor     ch,ch                   ;clear msb
        mov     cl,ah                   ;here's the repeat value
        inc     cx                      ;new count of bytes to stuff
        repz    stosb                   ;stuff x repeated chars
        pop     cx                      ;restore loop counter
Relup:  loop    ExpandLup
;Code is decrypted, uncompressed, etc.
        pop     dx                      ;b+PROGCODE: decoded program code start
        mov     cx,b.setup              ;ICE's code beyond program code
        sub     cx,100H+PROGCODE        ;minus where ICE thinks the program
code
                                        ;would start = program length
        mov     bx,handle               ;output file handle
        mov     ah,40H                  ;write to file/device
        int     21H
        mov     dx,offset writerr       ;'Write fail'
        jb      MsgTerm                 ;error msg, return AL
        mov     ah,3EH                  ;close file, just to be neat
        int     21H                     ;(although DOS would do it
                                        ;when we terminate)
        mov     dx,offset okmsg         ;'deICEd'
        xor     ax,ax                   ;ERRORLEVEL 0
MsgTerm:
        push    ax                      ;save ERRORLEVEL
        push    dx                      ;save DX terminal msg a sec
        mov     dx,offset logo          ;'TOADICE: '
        mov     ah,9                    ;display msg
        int     21H
        pop     dx                      ;original msg
MsgTerm2:
        mov     ah,9                    ;display msg
        int     21H
        pop     ax                      ;original ERRORLEVEL in AL
        mov     ah,4CH                  ;terminate process
        int     21H
ToadIce ENDP

;This data and code will be overwritten when we read in
;the ICE'd program as data.
b       label   icehdr
usage   db      'Usage:  TOADICE progname.com',CR,LF
        db      "Unscrambles an ICE'd .COM program, overwriting the original."
        db      CR,LF
        db      'Author:  David Kirschbaum, Toad Hall, v1.0 Dec 92',CR,LF
        db      'Donated to the Public Domain (with .ASM src, natch)'
        db      CR,LF,'$'
openerr db      'File open error$'

OpenFile        PROC    NEAR
        cld
        mov     si,80H                  ;PSP cmdline
        lodsb                           ;cmdline length
        or      al,al
        jnz     Spaces                  ; If 0, no command line
         mov    dx,offset usage         ;'Usage: ...'
         mov    al,0FFH                 ;ERRORLEVEL
         jmp    short Die               ;display, terminate

Spaces: lodsb
        cmp     al,20h                  ;Space?
        jz      Spaces                  ;Gobble leading spaces

        dec     si                      ;back up to first char
        mov     dx,si                   ;DX -> filename start

;Now we must Asciize the filename with a terminating 0

GetCr:  lodsb
        cmp     al,CR                   ;Terminating CR?
        jnz     GetCr                   ;nope, loop
        dec     si
        mov     byte ptr [si],0         ;Asciize the filename

        mov     si,dx                   ;remember filename start in SI
        mov     ax,3D00H                ;open file for read only
        int     21H
        jnb     OpenOk                  ;fine
         mov    dx,offset openerr       ;'File open error'
Die:     push   ax                      ;for upcoming POP
         jmp    MsgTerm2                ;ERRLEVEL in AL

OpenOk: mov     bx,ax                   ;file handle into BX
        mov     cx,0FE00H               ;max read (leaving room for stack)
        mov     dx,offset b             ;our buffer
        sub     cx,dx                   ;CX = max available space
        mov     ah,3FH                  ;read from file/device
        ret                             ;we'll read after the return
                                        ;since it'll overwrite this code
OpenFile        ENDP

CSEG    ENDS
        END     ToadIce
