;
; scroll.asm by Rich Paul
; This is a scroll routine, required by the color printing routine 
; cprint.asm. It takes 1/3 time of the bios routine.
;

.model small
.286
public scroll
.code

;-------------------------------------------------------------------------
;  Scroll the screen one line.
;  Calling conventions:  Don't worry 'bout it
;  All Registers Saved

scroll proc far
        pusha                   ; Save std regs
        push    ds              ; Save seg regs
        push    es

        mov     ax,0b800h       ; Set seg regs --
        mov     ds,ax           ; They're the same, but both for string
        mov     es,ax           ; Commands

        mov     si,160          ; Set si to 160
        xor     di,di           ; And di to 0

        mov     cx,(80*24)      ; cx is number to words on screen
                                ; - one row

        rep     movsw           ; move everything

        mov     ax,0720h        ; set up for spaces in white on black
        mov     cx,80           ; and one line (of words)

        rep     stosw           ; store the words
        pop     es
        pop     ds
        popa
        ret

scroll endp
end
