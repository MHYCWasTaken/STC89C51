;
;  DIALER.ASM 
;  Resident dialer
;
;  Author: Inbar Raz
;  Placed in public domain


Dialres segment

        assume  cs:Dialres, ds:Dialres

        org     100h

Start:  jmp     Begin

Old15   dw      ?,?
Port    dw      0001h           ; 0000 - COM1, 0001 - COM2, etc.
NumBuf  db      22 dup (0)
NumPtr  dw      ?
AllDone db      000h
ExitFlag db     000h
Running db      000h
Wanted  db      000h
DialPre db      'ATD'
DialPre1 db     'T', 000h
XlatTbl db      037h, 038h, 039h, 0, 034h, 035h
        db      036h, 0, 031h, 032h, 033h, 030h

New15   proc    far

        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        push    sp
        push    ds
        push    es

        cmp     ax,08500h
        jnz     GoOn1

        cmp     cs:Running,000h         ; Not already running?
        jnz     GoOn1

        cli

        mov     ax,cs
        mov     ds,ax
        mov     es,ax

        mov     Running,001h

        mov     bx,offset NumBuf
        mov     NumPtr,bx
        mov     cx,022d
Loop3:  mov     byte ptr [bx],00
        inc     ax
        loop    Loop3                           ; Erase existing number

        mov     AllDone,000h

        jmp     Kbd

GoOn1:  mov     cs:ExitFlag,000h
        jmp     GoOn

GoodBye1:
        jmp     GoodBye

Kbd:    mov     ah,010h
        int     16h                             ; Extended kbd read

        cmp     al,01Bh                         ; ESCape?
        jz      GoodBye1

        cmp     al,00Dh                         ; Done?
        jz      Done1

        cmp     ax,00E08h                       ; Backspace?
        jnz     Label1

        mov     bx,offset NumBuf
        cmp     NumPtr,bx
        jbe     Kbd                             ; Beginning of line?

        dec     NumPtr
        mov     si,NumPtr
        mov     byte ptr [si],000h              ;  from buffer too

        jmp     Kbd

Done1:  jmp     Done

Label1: cmp     al,000h                         ; Extended ASCII?
        jnz     Label2

        cmp     ah,047h                         ; Keypad?
        jb      Kbd

        cmp     ah,052h                         ;  - " -
        ja      Kbd

        mov     bx,offset XlatTbl
        sub     bx,047h                         ; To make up for partiality
        mov     al,ah
        xlat                                    ; Translate key

        cmp     al,000h                         ; If illegal,
        jz      Kbd                             ;  get a new one

Echo:   mov     bx,offset NumBuf
        add     bx,020d
        cmp     NumPtr,bx
        jnz     Label3

        mov     ax,00E07h
        int     10h                             ; Too many digits

        jmp     Kbd

Label3: mov     si,NumPtr
        mov     byte ptr [si],al
        inc     NumPtr                          ;  and remember it

        cmp     AllDone,001h
        jz      Done

Kbd1:   jmp     Kbd

Label2: cmp     al,02Dh
        jz      Echo                            ; '-'

        cmp     ah,026h
        jnz     Label5

        and     al,04Fh
        cmp     al,04Ch
        jnz     Kbd1

        mov     bx,offset NumBuf
        cmp     NumPtr,bx
        jne     Kbd1                            ; Beginning of line?

        mov     AllDone,001h

        jmp     Echo

Label5: cmp     al,030h
        jb      Kbd1

        cmp     al,039h
        jna     Echo

        jmp     Kbd1

Done:   mov     si,NumPtr
        mov     di,offset NumBuf
        cmp     si,di
        jz      GoodBye

        mov     byte ptr [si],00Dh

        cmp     AllDone,001h
        jnz     Label6

        mov     DialPre1,000h                   ; If redial, delete 'T'

Label6: mov     si,offset DialPre
        call    SndStr

        cmp     AllDone,001h
        jnz     Label7

        mov     DialPre1,'T'                    ;  and then restore it

Label7: mov     si,offset NumBuf
        call    SndStr

        mov     ah,010h
        int     16h                             ; Wait for a key

        cmp     ax,0011Bh                       ; Is it ESC?
        jz      GoodBye                         ; If yes, don't hang-up

Loop1:  mov     si,offset NumBuf
        call    SndStr                          ; Make the modem hang-up

GoodBye:
        mov     Running,000h
        mov     Wanted,000h
        mov     ExitFlag,001h

GoOn:   pop     es
        pop     ds
        pop     sp
        pop     bp
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax

        cmp     cs:ExitFlag,000h
        jz      Label9
        iret

Label9: jmp     cs:dword ptr [old15]

Sndstr  proc    near

        mov     al,[si]
        or      al,al
        jz      Leavs

        mov     dx,Port
        mov     ah,01
        int     14H                             ; Serial port output

        and     ah,080h
        jnz     MdmErr

        inc     si
        jmp     Sndstr

MdmErr: jmp     GoodBye

Leavs:  ret

Sndstr  endp

New15   endp

Begin:  mov     ax,03515h
        int     21h

        mov     [old15],bx
        mov     [old15+2],es

        mov     dx,offset New15
        mov     ax,02515h
        int     21h

        mov     dx,offset Begin
        int     27h

Dialres ends
        end     start
