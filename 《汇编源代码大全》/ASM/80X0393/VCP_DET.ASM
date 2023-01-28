comment *
    VCP_DET.ASM
    Purpose:
    To detect if a VCPI host is is currently installed in machine.

    Author:
    Yousuf Khan, released to public domain

   To run VCP_DET you have link it with EXTFILE.ASM !
   (i.e. tlink /t vcp_det+extfile for Borland)       
*

        .model tiny

        .data
        cr_lf   equ     13,10
        yes     db      "VCPI driver found, exiting errorlevel 0",cr_lf,"$"
        no      db      "No VCPI driver, exiting errorlevel 255",cr_lf,"$"

        .code
extrn   vcpi_detect:near

        org     100h

start:
        call    vcpi_detect
        push    ax
        cmp     ax, 0
        jne     no_vcpi
        lea     dx, yes
        mov     ah, 9
        int     21h
        pop     ax
        jmp     short eop

no_vcpi:
        lea     dx, no
        mov     ah, 9
        int     21h
        pop     ax

eop:
        mov     ah, 4ch
        int     21h
        end     start

; EOF VCP_DET.ASM
