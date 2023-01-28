;
; NOCAD.ASM     a control-alt-delete interrupt handler
;
; June 23, 1992  Paul Cullum
;
; Modified by David Kirschbaum aka Toad Hall fixing problems
; relating to segment registers.  Thanks a lot David.
; Released into the public domain.
;

        .MODEL small, c

        CTRLALT  EQU  8h OR 4h
        DELSCAN  EQU  53h
        KBPORT   EQU  60h
        CONTPORT EQU  20h

        .CODE

;  Function:    disablecad
;  Parameters:  an offset to a word is passed to flag a CTRL-C/BREAK
;  Description: sets up a new keyboard handler
;

disablecad PROC near

flag    EQU     [bp+4]                  ;address of flag variable

        push  bp                        ;save BP
        mov   bp, sp                    ;set up stack frame

        mov   ax, flag                  ;mov address into AX
        mov   word ptr cs:flagptr, ax   ;save the word's offset in flag
        mov   word ptr cs:flagptr+2, ds ;
        mov   ax, 3509h                 ;save the old handler's address
        int   21h
        mov   word ptr cs:oldkbhandler, bx
        mov   word ptr cs:oldkbhandler+2, es
        push  ds
        push  cs
        pop   ds
        mov   dx, offset cs:newkbhandler
        mov   ax, 2509h                 ;set new handler as interrupt
        int   21h
        pop   ds

        pop bp
        ret

        flagptr       dd     0
        oldkbhandler  dd     0

disablecad ENDP


;
;  Function:    enablecad
;  Parameters:  none
;  Description: restores original handler
;
enablecad PROC near

        push  ds
        push  ax
        push  dx

        lds   dx, cs:oldkbhandler
        mov   ax, 2509h
        int   21h

        pop dx
        pop ax
        pop ds
        ret

enablecad ENDP


;
;  Function:    newkbhandler
;  Parameters:  none
;  Description: ctrl-break handler, sets a flag
;
newkbhandler PROC far
        push  ax

        in    al, KBPORT         ;read scancode of key that caused interrupt
        cmp   al, DELSCAN        ;check to see if it is DEL
        jne   notcad             ;if not call to old handler

        push  ds
        xor   ax,ax
        mov   ds, ax
        mov   al, ds:[417h]      ;reads keyboard flags
        pop   ds                 ;done with it
        and   al, CTRLALT        ;and it with CTRLALT flags
        cmp   al, CTRLALT        ;compare it to CTRLALT flags
        jne   notcad             ;if not call old handler

        push  bx                 ;save BX
        push  ds                 ;and DS 
        lds   bx, cs:flagptr     ;set the CTRL-ALT-DEL flag
        mov   word ptr [bx], 1   
        pop   ds                 ;restore
        pop   bx                 ;done with it

        in    al, KBPORT + 1
        mov   bl, al
        or    al, 80h
        out   KBPORT + 1, al
        mov   al, bl
        out   KBPORT + 1, al
        mov   al, 20h
        cli
        out   CONTPORT, al
        sti
        jmp   short done

notcad: pushf                ;fake an interrupt
                             ;(remember, the old handler is gonna
                             ;IRET back to us!)
        call  cs:[oldkbhandler]     ;call to old handler

done:   pop   ax               ;only reg still on the stack
        iret
newkbhandler ENDP
END
