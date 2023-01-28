;
; Tasm module, Call from C
;
; pos_xy( int x, int y );
;
; Written by Craig Allsop, Public Domain.
; Assumes 16 colour mode, and screen width of 640 pixels.
;
; I only typed this from memory, but it should work.
;

ideal
model   small,c
codeseg

proc    pos_xy
        arg     x:word, y:word

        mov     ax,[y]
        mov     bl,80                   ; width of screen / 8 (bytes)
        mul     bl
        mov     bx,[x]
        mov     cl,bl
        shr     bx,3                    ; Convert to bytes from pixels
        add     bx,ax
        and     cl,7                    ; Keep pixel offset
        mov     dx,3d4h
        mov     ah,0ch
        mov     al,bh
        out     dx,al                   ; Set high order of address
        mov     ah,0dh
        mov     al,bl
        out     dx,al                   ; Set low order of address
        mov     dh,0dah
        in      al,dx
        mov     dh,0c0h
        mov     al,33h                  ; Register 13 OR 20
        out     dx,al
        mov     al,cl
        out     dx,al                   ; Output pixel panning offset
        ret
endp
end pos_xy
