comment *
   IRQVECT.ASM

   Purpose:
   Demonstration of use of the VCPI function 0DE0Ah, which tells
   you which vectors the 8259 PIC is mapping IRQs to.

   Author:
   Yousuf Khan, released to the public domain

   To run IRQVECT you have link it with EXTFILE.ASM !
   (i.e. tlink /t irqvect+extfile for Borland)
*

        .model tiny
        .data
        no_vcpi         db      "VCPI driver not found",13,10,"$"
        success         db      "Successful query of VCPI IRQ mappings"
                        db      13,10,"Master PIC: "
        master_vect     db      3 dup(20h),"d",13,10,"Slave PIC: "
        slave_vect      db      3 dup(20h),"d",13,10,"$"
        no_slave        db      "none"
        failure         db      "Couldn't get VCPI IRQ mappings",13,10,"$"

        .code

extrn   vcpi_detect:near        ;custom external module
        org     100h

start:
        call    vcpi_detect     ;Is VCPI host installed?
        cmp     ax, 0
        je      vcpi_present    ;if AX=0, then VCPI present
        lea     dx, no_vcpi     ;print message that VCPI
        mov     ah, 9           ; host is not present
        int     21h
        jmp     short eop

vcpi_present:
        mov     ax, 0DE0Ah      ;VCPI Get 8259 state function
        int     67h
        cmp     ah, 0
        jne     short failed    ;function failed if AH<>0
        ;
        ;Ascii'ize master PIC's vector
        ;
        mov     al, bl          ;use master PIC's vector as errorlevel
        push    ax
        mov     di, offset master_vect
        call    ascii_convert   ;Ascii'ize master PIC's vector
        ;
        ;determine if slave 8259 PIC exists
        ;
        mov     ah, 0c0h
        int     15h
        ;If CF set, then Int 15h, AH=C0h not supported by machine
        ;must be an old XT
        jnc     short supported ;is function supported? yes
        call    print_none
        pop     ax
        jmp     short display_string

supported:
        mov     al, es:[bx]     ;read feature byte #1 into AL
        test    al, 01000000b   ;if bit 6 set then slave PIC present
        je      short slave_present
        call    print_none
        jmp     short display_string

slave_present:
        ;
        ;Ascii'ize slave PIC's vector
        ;
        xor     ax, ax          ;zero AX
        mov     al, cl
        mov     di, offset slave_vect
        call    ascii_convert   ;Ascii'ize slave PIC's vector

display_string:
        mov     dx, offset success
        mov     ah, 9
        int     21h
        pop     ax
        jmp     short eop

failed:
        mov     dx, offset failure
        mov     ah, 9
        int     21h
        mov     al, 0

eop:
        mov     ah, 4ch
        int     21h

ascii_convert   proc
;
;       -Put byte to be converted into AL
;       -Point DI to start of location to store ascii'ized AL value
;
        push    bx
        mov     bl, 100d
        div     bl              ;divide by 100
        mov     bl, al
        add     bl, 30h         ;convert to ascii
        mov     [di], bl        ;store ascii
        xchg    ah, al          ;work on remainder
        xor     ah, ah          ;zero upper byte
        mov     bl, 10d
        div     bl              ;divide by 10
        mov     bl, al
        add     bl, 30h
        mov     [di][1], bl
        xchg    bl, ah          ;store remainder directly into BL
        add     bl, 30h
        mov     [di][2], bl
        pop     bx
        ret
        endp

print_none      proc
;
; puts the word "none" right after slave PIC status
;
        mov     ax, ds
        mov     es, ax          ;make sure ES = DS
        lea     si, no_slave    ;source of MOVSW
        lea     di, slave_vect  ;destination of MOVSW
        mov     cx, 2
        rep movsw
        ret
        endp

        end     start

; EOF IRQVECT.ASM
