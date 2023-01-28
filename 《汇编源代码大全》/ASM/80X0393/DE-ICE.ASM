;
; deice.asm by Inbar Raz
;
; Notice - this is for COM only
;

        .model  tiny
        .code

        org    0100h

CODESTART       EQU     $

Start:  jmp     Begin

NewName db      'OPENED.COM', 000h
Handle  dw      ?
Old60   dd      ?

M_Prog  db      'DeIce - A program to extract executables compressed under ICE.'
        db      00Dh, 00Ah
        db      'By Inbar Raz, 2:401/100.1 or 2:403/123.42.'
CrLf    db      00Dh, 00Ah, 024h


AlgorithmSize   EQU     00200h

; ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;
; First Breakpoint - Encryption Loop

CallerOff       EQU     word ptr ss:[bp+00002h]
RetAddress      EQU     word ptr ss:[bp+00008h]
HereCount db    2

New60   proc    far

        push    bp
        mov     bp,sp

        dec     cx                      ; When CX=1, LOOP simply proceeds.
        jnz     Wrong

; By now, the encryption has been finished. Now we will replace the second
; loop also with a CD 02, and we will self-modify the routie so it would handle
; the second loop.

; First, let's find the address for the next breakpoint

        push    ax
        push    cx
        push    dx
        push    di
        push    ds

        std
        mov     cx,AlgorithmSize
        mov     di,RetAddress
        mov     al,001h
Scan2:  repnz   scasb
        or      cx,cx
        jnz     NotDone
        jmp     NotIce

NotDone:
        cmp     word ptr es:[di+2],0FF2Eh ; The LOOP we are searching
        jne     Scan2
        cmp     word ptr es:[di+4],00A26h
        jne     Scan2

        inc     di
        inc     di
        mov     ax,060CDh               ; Replace with INT 60
        stosw

        mov     ax,02560h
        push    cs
        pop     ds
        lea     dx,New60_
        int     21h

        pop     ds
        pop     di
        pop     dx
        pop     cx
        pop     ax

        pop     bp

        iret

Wrong:  push    ax
        mov     ax,CallerOff
        sub     ax,00008h
        mov     CallerOff,ax
        pop     ax

        pop     bp
        iret

New60   endp

; ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

New60_  proc    near

        int     3

        push    ds

        push    cs
        pop     ds

        mov     ah,03Ch
        mov     cx,00020h
        lea     dx,NewName
        int     21h

        jnc     Created

        call    Print
        db      'Unable to create destination file.$'

Created:
        mov     word ptr cs:[Handle],ax

        mov     bx,ax
        mov     ah,040h
        mov     cx,di
        sub     cx,001ACh
        mov     dx,si
        pop     ds
        int     21h

        mov     ah,03Eh
        mov     bx,word ptr cs:[Handle]
        int     21h

        call    Print
        db      'Extracting file completed.$'

New60_  endp

; ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;
; Beginning of activation code

Begin:  mov     ah,009h
        lea     dx,M_Prog               ; 'DeIce - A program to extract...'
        int     21h                     ; Display $-terminated string at ds:dx

        mov     ax,03560h
        int     21h

        mov     word ptr [Old60],bx
        mov     word ptr [Old60+2],es   ; Store old vector

        mov     ax,02560h
        lea     dx,New60
        int     21h

        push    cs
        pop     es

        mov     si,00080h               ; Count of chars on CommandLine
        lodsb
        or      al,al
        jnz     FetchName

        call    Print
        db      'No file specified.$'

FetchName:
        cld
        mov     di,00082h               ; Command line start in PSP
        mov     al,00Dh                 ; Search for end of line ( 00Dh )
        mov     cx,007Fh                ; Maximum size of commandline
        repnz   scasb

        dec     di
        mov     al,000h
        stosb                           ; Repace it with 0, to make it ASCIIZ

        mov     ax,03D00h
        mov     dx,00082h
        int     21h

        jnb     Opened

        call    Print
        db      'Unable to open specified file.$'

Opened: mov     word ptr Handle,ax

        mov     ax,cs
        add     ax,((CODEEND-CODESTART) shr 4) + 16     ; Stack space
        mov     ds,ax

        lea     sp,NEWSTACK

        mov     ah,03Fh
        mov     bx,word ptr cs:[Handle]
        mov     cx,0FFFFh               ; Maximum size of COM file
        mov     dx,00100h
        int     21h                     ; Read cx bytes from handle bx to ds:dx

        mov     ah,03Eh
        mov     bx,word ptr cs:[Handle]
        int     21h                     ; Close file handle in bx

        push    ds
        pop     es

        mov     cx,AlgorithmSize
        mov     al,0D0h
        mov     di,00100h
Scan1:  repnz   scasb                   ; Search for al in es:[di]
        jcxz    NotIce
        cmp     word ptr es:[di],0F8E2h ; The LOOP we are searching
        jne     Scan1

        mov     ax,060CDh               ; Replace with INT 60
        stosw

        push    es
        mov     ax,0100h
        push    ax
        retf                            ; Jump to COM program

; ***************************************************************************


NotIce: call    Print
        db      'Program is not compressed under ICE.$'

; ***************************************************************************

Print   proc    near

        push    cs
        pop     ds

        mov     ah,009h
        pop     dx
        int     21h

        mov     ah,009h
        lea     dx,CrLf
        int     21h

        mov     ax,02560h
        lea     dx,Old60
        int     21h

        mov     ah,04Ch
        int     21h                     ; Terminate program

Print   endp

; ***************************************************************************

CODEEND         EQU     $

NEWSTACK        EQU     CODEEND+04Dh

        end     Start
