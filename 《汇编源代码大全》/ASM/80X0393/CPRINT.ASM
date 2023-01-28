;
; cprint.asm by Rich Paul
;
; This is a direct video color printing routine.  
; Use in mode three (co80). It's kinda nice.
;

.model small
.code
.286
extrn scroll:far
public cprint

;-----------------------------------------------------------------------------
; Color Printing Routine -- Direct Video Version -- Use in mode 3 
; or at your own risk
;
; Calling conventions: DS:SI - string to print
;    DX - location, or FFFF for currant cursor pos
; String:  Z-Term, 255 preceeds attribute
; Return:  DX - ending cursor pos
;    if called with 0 in DX, sets new cursor pos as well

cprint proc far
        push    es              ; save ES first, to use as temp
        pusha                   ; save the regs
        push    dx              ; and an extra DX for return

        mov     ax,0b800h       ; set the ES to screen
        mov     es,ax

        cmp     dx,0FFFFH       ; if =, use current bois pos
        jne     sent            ; otherwise, it came with control
        mov     ah,3            ; Lookin' for the cursor pos.
        mov     bx,0            ; For the currant page
        int     10h             ; OK, so gotta use a bois routine

sent:
        push    dx              ; Save the full position
        shr     dx,8            ; and set DX to the row #
        mov     ax,160          ; ax is # of bytes/row
        mul     dl              ; * rows is offset of first byte of row
        pop     dx
        add     al,dl           ; set to 1/2 offset of column
        add     al,dl           ; set to offset of column
        mov     di,ax
        mov     bl,es:[di+1]    ; get currant attribute

printl:
        lodsb
        cmp     al,0
        je      printld
        cmp     al,255          ; is this attribute flag?
        jne     printl2         ; nope, print it

        lodsb                   ; get the attribute
        mov     bl,al           ; save the attribute
        jmp     printl          ; and continue

printl2:
        cmp     al,13           ; how 'bout <CR>?
        je      docr
        cmp     al,10           ; and <LF>?
        je      dolf

        inc     dl
        cmp     dl,51h
        jne     printl4
        xor     dl,dl
        inc     dh
        cmp     dh,19h
        jne     printl3
        dec     dh
        call    scroll
        sub     di,160

printl3:
printl4:
        stosb
        mov     al,bl
        stosb

        jmp     printl

printld:
        mov     es,dx           ; save the cursor pos in es
        pop     dx              ; restore extra DX
        cmp     dx,0FFFFH       ; was it called to print at current position?
        jne     sent2           ; if NOT, skip setting
        mov     dx,es           ; get pos back
        mov     ah,02           ; service to set
        xor     bh,bh           ; zero bx
        int     10h             ; and set new cursor pos

sent2:
        popa
        mov     dx,es           ; return the cursor pos
        pop     es
        ret

docr:
        push    ax
        push    cx
        mov     ax,di
        mov     cl,160
        div     cl
        shr     ax,8            ; move remainder to AX (from ah)
        neg     ax
        add     ax,di
        mov     di,ax           ; set marker to start of currant line

        xor     dl,dl           ; set column to 0
        pop     cx
        pop     ax
        jmp     printl

dolf:
        inc     dh              ; next line
        cmp     dh,19h          ; off the edge?
        jne     dolf2
        dec     dh
        call    scroll
        jmp     printl

dolf2:
        add     di,160          ; move to same pos on next line
        jmp     printl

cprint endp
end
