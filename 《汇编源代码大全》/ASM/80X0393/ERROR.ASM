;
; error.asm by Rich Paul
;
; how to return an error code
; requires scroll.asm and cprint.asm
;

.model tiny
.286
.data
extrn cprint:far

msg     db 0ffh,12,"ShiftChk V2.0 By Rich Paul",10,13
        db 0ffh,14,"Released to the ",0ffh,15,"PUBLIC DOMAIN",10,13
        db 0ffh,14,"Assembled with Turbo Assembler",10,13
        db 0ffh,12,"Source should be included in package",10,13
        db 10,13
        db 0FFh,14,"Returns:",10,13
msgterm db 0
ret0    db 0ffh,12,"   0: No Shift Keys Pressed  ",10,13,0
retsiz  equ $-ret0
ret1    db 0ffh,14,"   1: Right Only             ",10,13,0
ret2    db 0ffh,14,"   2: Left Only              ",10,13,0
ret3    db 0ffh,14,"   3: Both Shift Keys        ",10,13,0
enjoy   db 0ffh,14,"Enjoy!",10,13
        db 0ffh,12,"Returning:",10,13,10

.code
main proc far
        mov     ax,@data
        mov     ds,ax

        mov     ah,3
        mov     bh,0
        mov     al,3

        mov     si,offset msg
        mov     dx,0ffffh
        call    cprint

        mov     al,0
        mov     cx,4

responceloop:
        call    presp
        inc     al
        loop    responceloop

        mov     si,offset enjoy
        call    cprint

        mov     ah,02h
        int     16h

        and     al,03h
        call    presp
        mov     ah,4ch
        int     21h
main endp

presp proc near
        pusha
        mov     si,offset ret0
        mov     bx,retsiz
        xor     ah,ah
        mul     bx
        add     si,ax
        mov     dx,0ffffh
        call    cprint
        popa
        ret
presp endp
end
