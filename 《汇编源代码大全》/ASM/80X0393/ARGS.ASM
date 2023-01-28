;---- args.asm ----------------------------------------------------------
; This file contains both an argument grabber and a simple heap.
;
; By: David Kirschbaum
;
; Args parses the command line into a unix-style parameter array.
; To use it, you should be a .COM program: that is,
; you must start your program as follows:
;       code    segment public 'CODE'
;       assume  CS:code,DS:code,ES:code
;       extrn   _Args:near,argc:word,argv:word          ;argc & argv in CS:
;
; and end the file as follows:
;       code    ends
;       end
; Link to your program with LINK YOURFILE,args; then EXE2BIN YOURFILE.
; Your program should start with an ORG 100H statement, and should
; CALL _Args to have it parse the cmdline.
; Argc and Argv are just as in C; argc = # of params on cmd line,
; argv is an array of pointers to strings (null terminated, of course).
; Because MS-DOS doesn't pass us the name used to invoke the program,
; argv[0] always points to a null string.
; _Shift updates ARGC.
;
; See the file \bin\asm\exist.asm for an example.
; Do not call NEW before calling _Args.
;
; Changes:
;   16 July 1984   DRK    Copy args out of default location; fixed argv[0].
;   1 Aug 1984     DRK    Added _Shift.
;   16 Oct 1984    MCN    _Shift updates argc
;   21 Mar 1985    DRK    oh yeah?
;Toad Hall Tweak
;--------------------------------------------------------------------------

        PUBLIC  _Args,ARGC,ARGV,_Shift, New, Dispose

; Predeclared file handles
STDIN   equ     0
STDOUT  equ     1
STDERR  equ     2
STDSIO  equ     3
STDPRN  equ     4

; Maximum number of parameters
MAXPARMS        equ     32

; Declarations for accessing command line
NCHAR   equ     80h             ; address of number of chars in argument line
PARAMS  equ     81h             ; address of argument line

;---- code starts here --------------------------------------------------------
CSEG    segment public  'CODE'
        assume CS:CSEG,ds:CSEG

_Args   proc    near
        call    near ptr newinit
        ; initialize
        cld                     ; clear decrement mode
        mov     bx, 2           ; argc = 0 (will sub 2 from bx at end)
        mov     argv[0], offset null    ; prog name unknown; set to null.
        mov     si, PARAMS      ; i = 0
        mov     di, NCHAR
        mov     cl, [di]        ; cx = # of chars in command line
        xor     ch,ch
        jcxz    Done            ; no arg chars -> we're done.
        ; Allocate room for arguments; returns pointer in BX.
        push    bx
        push    cx
        mov     cx, 128         ; worst case
        call    New
        mov     di, bx
        pop     cx
        pop     bx
        ; Move arguments out of default DTA.
        push    cx
        push    di
        rep     movsb
        pop     si
        pop     cx

;Toad Hall:  When he's using redirection
;StdIn alone ("<") produces a command line length of 1 (space, CR)
;StdOut alone (">" : same result.
;StdIn and StdOut ("<wherever >wherever") produces a command line
;length of 2 (space, space, CR).
;A question mark alone would produce a command line length of 2
;(space, "?", CR).
        ; Big loop- find arguments...
ParmL:
        ; Little loop #1: strip leading blanks from argument.
        ; while (i<NCHAR && PARAMS[i] = ' ') do i++;
StripL: lodsb                   ; al = [si++]
        cmp     al, ' '         ; space?
        loopz   StripL          ; if so, keep skipping.
        jne     GotOne          ; If we found a nonblank, skip zero test.
        jcxz    Done            ; found no unblank chars -> we're done.
GotOne: dec     si              ; bump SI back to start of nonblank.
        inc     cx
        mov     argv[bx],si     ; save pointer to this string
        ; Little loop #2: skip nonblank chars.
        ; while (i<NCHAR && PARAMS[i] <> ' ') do i++;
SkipL:  lodsb
        cmp     al, ' '
        loopnz  SkipL
        jz      OkY
                ; Last char of line was not blank.
         inc    si      ; make next statement put null AFTER arg.
OkY:    mov     byte ptr [si][-1], 0    ; put null at end of arg
        add     bx,2            ; argc++;
        jcxz    Done            ; if we ran off end of cmdline, no more args.
         cmp    bx, MAXPARMS*2
         jb     ParmL           ; loop if argc < MAXPARMS.
Done:   ; All done finding parms; now share argc with caller.
        sub     bx, 2
        shr     bx, 1
        mov     argc, bx
        ret
_Args   endp

;---- _Shift: --------------------------------------------
; Shifts %2 to %1, %3 to %2, etc.  Leaves %0 alone.
; Works by shuffling argv[*].
_Shift  proc    near
        cld
        mov     si, offset argv[4]
        mov     di, offset argv[2]
        mov     cx, MAXPARMS
        rep     movsw
; wrongo!  Caller should do this...
;       sub argc, 1             ; there is now one less argument
        ret
_Shift  endp

;----- Newinit- initialize heap

Newinit proc    near
        mov     CS:firstfree, offset lastcode
        ret
Newinit endp

; Given length in CX, returns pointer in BX.
; Does not affect CX.
New     proc    near
        push    cx
        mov     bx, CS:firstfree
        add     cx, CS:firstfree
        mov     CS:firstfree, cx
        cmp     sp, cx
        pop     cx
        jbe     Newerror
        ret

stackmsg        db      '?New: heap overflow', 7, 13, 10
Newerror:
SML     equ     Newerror - stackmsg
        push    cs
        pop     ds
        mov     dx, offset stackmsg
        mov     cx, SML
        mov     bx, STDERR
        mov     ah, 40h
        int     21h
        mov     ax, 4c02h               ; terminate- error code 2
        int     21h
New     endp

;---- Call Dispose with ptr in BX, length in CX.
Dispose proc    near
        ret
Dispose endp

;------ Heap variables ------
firstfree       dw      ?

;---- parameter count, array ------------------------------------
; Pointers to up to MAXPARMS parameter strings are held here.

null    dw      0               ; the null string
argc    dw      ?
argv    dw      MAXPARMS dup (?)
lastcode        label   near

CSEG    ends
        end
