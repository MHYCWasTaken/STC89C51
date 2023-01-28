;
;  H2NAME.ASM
;  LIBRARY routine which you may either store in a    
;  library, or simply compile it to an OBJ, and state:
;  (in any way if you want to use it...)              
;
;          EXTRN   HandleToName: Far
;
;  Author: Inbar Raz
;  Placed in public domain
;

Handle2Name     Segment

                assume cs:Handle2Name

                public HandleToName

EntrySize  EQU 0004h                    ; Length of table entry
EntryCount EQU 0004h                    ; Number of table entries

;ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;
; FAR PROC HandleToName
;
; PURPOSE:  Given a handle, return pointer to ASCIIZ file name
;
; INPUTS:   BX = Handle of file to be named
;
; OUTPUTS:  ES:DX => ASCIIZ file name (exactly 11 bytes long) (FCB format)

HandleToName    proc    far

        push    ax                      ; Save all used registers on stack
        push    cx
        push    dx
        push    si
        push    bp                      ; Will be used as the indes to the
                                        ; SFTLength and SFTName fields

        push    bx                      ; Save handle

        mov     ah,030h                 ; Get DOS version
        int     21h
        xchg    al,ah                   ; Now adjust things for our compares
        mov     cx,EntryCount           ; Four entries in table
        call    @@GetOff                ; Figure out our offset

CODESTART EQU   $

@@GetOff:
        pop     bx                      ; Calculate offsets. Since this is
                                        ; a FAR Library routine, offsets
                                        ; are never as reported by the
                                        ; assembler at compilation time.
        mov     bp,bx                   ; Calculate SFT offsets
        dec     bp                      ;
        dec     bp                      ; BP now points to SFTLength
        add     bx,CODELEN              ; Calculate table's offset

@@looky:
        cmp     ax,word ptr cs:[bx]     ; Is DOS version/rev correct?
        ja      @@got_values            ; If it's greater, it's OK
        add     bx,EntrySize            ; If not, check next entry
        loop    @@looky                 ; And jump back up

; should err_exit if < DOS 2.0 here, but...

@@got_values:
        mov     ax,word ptr cs:[bx+00002h] ; Get SFTsize and NameOffs
        mov     word ptr cs:[bp],ax     ; Store them the same way (cheat!)
                                        ; Since we need to access both in the
                                        ; same time, this will be the only time
                                        ; we actually use BP and not the EQU's

        pop     bx

        mov     si,bx                   ; Save original file handle in SI
        mov     ah,062h                 ; Get program's PSP
        int     21h
        mov     es,bx
        les     bx,dword ptr es:[00034h] ; Point ES:BX to the PSP's JFT
        mov     al,byte ptr es:[bx+si]  ; Now read table entry (index into SFT)
        cbw                             ; Clear AH
        mov     si,ax                   ; Rememer the SFT index
        mov     ah,052h                 ; Get list of lists into ES:BX
        int     21h
        les     bx,dword ptr es:[bx+00004h] ; Point ES:BX to the SFT

@@NextSFT:                              ; ES:BX => SFT at this point
        cmp     si,word ptr es:[bx+00004h] ; Is this handle in this SFT?
        jb      @@SFT_ok                ; If so, we're in the correct table
        sub     si,word ptr es:[bx+00004h] ; If not, adjust index for next table
        les     bx,dword ptr es:[bx]    ; And load address of next SFT
        jmp     @@nextSFT               ; Loop back again

@@SFT_ok:
        mov     ax,si                   ; Recall our adjusted SFT index
        mov     dl,SFTlength            ; SFTlength is the length of an SFT entry
        mul     dl                      ; AX = (SFT index) * (SFT length)
        mov     dl,SFTname              ; SFTname = offset from start of SFT
        xor     dh,dh                   ; Clear out high half of DX
        add     ax,dx                   ; AX = SFTindex * SFTlength + SFTname
        add     bx,ax                   ; BX = Original offset + calc offset
        mov     di,bx                   ; Now ES:DX => File name string

        pop     bp
        pop     si
        pop     dx
        pop     cx
        pop     ax                      ; Restore all trgisters from stack

Quit:
        retf

SFTlength       EQU     byte ptr cs:[bp]
SFTname         EQU     byte ptr cs:[bp+00001h]

;SftLength
        db      ?
;SftName
        db      ?

CODELEN   EQU   $-CODESTART

; Format of data table:
;
; DosMin   DB  0                        ; Minor DOS version
; DosMaj   DB  0                        ; Major DOS version
; SFTsize  DB  ?                        ; System File Table entry size
; NameOffs DB  ?                        ; Offset from SFT start to first name

Voodoo  db    000h,004h,03Bh,026h       ; Dos version 4.0
        db    001h,003h,035h,026h       ; Dos version 3.1
        db    000h,003h,038h,027h       ; Dos version 3.0
        db    000h,002h,028h,00Ah       ; Dos version 2.0
        db    000h,000h,000h,000h       ; 000h's, to end the compare.

HandleToName    endp

Handle2Name     ends

        end
