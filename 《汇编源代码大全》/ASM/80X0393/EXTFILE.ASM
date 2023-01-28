comment *
   EXTFILE.ASM

   EXTERNAL FILE for use with:
   IRQVECT.ASM
   VCP_DERT.ASM

   Author:
   Yousuf Khan, released to the public domain
*

        .model tiny

public  vcpi_detect

        .data
        emm_name        db      'EMMXXXX0', 0
        org 100h
        .code

vcpi_detect     proc    near

comment *
    Purpose:
    Detects whether a VCPI host is installed.

    Entry:
    nothing

    Destroys:
    BX, DX

    Returns:
    AX=0    if VCPI installed
    AX=-1   if VCPI not installed
*

        pushf
        xor     ax, ax
        push    ax
        popf
        pushf
        pop     ax
        test    ax, 0f000h
        je      short not_386   ;upper four bits of FLAG register not
                                ;settable, must be less than a 386
        popf
        jmp     short ems_check

not_386:
        popf                    ;No 386? Can't have VCPI either
        jmp     no_vcpi

ems_check:
        lea     dx, emm_name    ;"EMMXXXX0" name of handle to open
        mov     ax, 3d00h
        int     21h             ;open file handle
        jc      short no_vcpi   ;handle not found, must be no EMS
        mov     bx, ax          ;store file handle # in BX
        mov     ax, 4400h       ;IOCTL: get device info
        int     21h
        jc      short no_vcpi   ;error occurred, must be no EMS
        test    dx, 80h         ;if 7th bit clear, file handle must
        jz      short no_vcpi   ; be disk file not character device
        mov     ax, 4407h
        int     21h
        push    ax              ;store result of fnct.
        mov     ah, 3eh
        int     21h             ;close file handle
        pop     ax
        cmp     al, 0ffh        ;EMS device ready? If yes, AL=0FFh.
        jne     short no_vcpi
        ;
        ;Now we found an EMS driver is installed. But is it VCPI?
        ;
        mov     bx, 1           ;# of pages to allocate
        mov     ah, 43h         ;get EMS handle and allocate
        int     67h             ; memory
        cmp     ah, 0           ;function completed successfully
        jne     short no_vcpi
        push    dx              ;save EMS handle
        mov     ax, 0de00h      ;VCPI installation check fnct.
        int     67h
        pop     dx              ;retrieve EMS handle
        push    ax              ;save VCPI detection flag
        mov     ah, 45h
        int     67h             ;release EMS handle
        pop     ax
        cmp     ah, 0
        jne     short no_vcpi   ;if AH<>0, no VCPI
        xor     ax, ax          ;set AX=0 if VCPI present
        jmp     short exit_tests

no_vcpi:
        mov     ax, -1          ;set AX=1 if VCPI not present

exit_tests:
        ret
        endp
        end

; EOF EXTFILE.ASM
