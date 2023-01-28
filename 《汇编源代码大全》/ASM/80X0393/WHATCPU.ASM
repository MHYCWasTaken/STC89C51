;***************************************
;* WHATCPU.ASM 
;* Written by Dave M. Walker 
;* For use with TASM, but shouldn't 
;* be too hard to change.  
;* Version 1.1
;***************************************
;* Thanks to the author(s) of INFOPLUS, 
;* Yousuf Khan and David Kirschbaum for 
;* their contributions. This is released 
;* into the public domain with no 
;* reservations, of course.                          
;***************************************
;* Returns: 00 for 8088
;*          01 for 8086
;*          10 for NEC V20
;*          11 for NEC V30
;*          12 for 80188
;*          13 for 80186
;*          20 for 80286
;*          30 for 80386sx
;*          31 for 80386dx
;*          40 for 80486 (sx or dx)
;*          FF for Unknown CPU type
;***************************************

                IDEAL
                MODEL   tiny
                CODESEG
                ORG     0100h

f8088           EQU     00h
f8086           EQU     01h
fV20            EQU     10h
fV30            EQU     11h
f80188          EQU     12h
f80186          EQU     13h
f80286          EQU     20h
f80386SX        EQU     30h
f80386DX        EQU     31h
f80486          EQU     40h
UnkCPU          EQU     0FFh

;*** Test interrupt of multi-prefix string instruction
Start:          sti
                mov     cx,0FFFFh
                rep lods [BYTE es:si]
                jcxz    Test02                  ;If CX == 0, not 8086/8088

                call    PiqCalc                 ;Determine type by PIQ len.
                cmp     dx,4
                jne     Test01a
                mov     bl,f8088
                jmp     TestDone
Test01a:        cmp     dx,6
                jne     Test01b
                mov     bl,f8086
                jmp     TestDone
Test01b:        mov     bl,UnkCPU
                jmp     TestDone

;*** Test number of recognized bits used by shift
Test02:         mov     al,0FFh
                mov     cl,20h
                shl     al,cl
                or      al,al
                jnz     Test03                  ;If AL != 0, not V20/V30

                call    PiqCalc                 ;Determine type by PIQ len.
                cmp     dx,4
                jne     Test02a
                mov     bl,fV20
                jmp     TestDone
Test02a:        cmp     dx,6
                jne     Test02b
                mov     bl,fV30
                jmp     TestDone
Test02b:        mov     bl,UnkCPU
                jmp     TestDone

;*** Test order of write/decrement by PUSH SP
Test03:         push    sp
                pop     ax
                cmp     ax,sp
                je      SHORT Test04            ;If AX == SP, not 80186/80188

                call    PiqCalc                 ;Determine type by PIQ len.
                cmp     dx,4
                jne     Test03a
                mov     bl,f80188
                jmp     TestDone
Test03a:        cmp     dx,6
                jne     Test03b
                mov     bl,f80186
                jmp     TestDone
Test03b:        mov     bl,UnkCPU
                jmp     TestDone

;*** Test high nybble of FLAGS register (80286 remains 0's)
Test04:         pushf
                pop     ax
                or      ah,0F0h
                push    ax
                popf
                pushf
                pop     ax
                test    ah,0F0h
                jnz     Test05                  ;If FLAGS changes, not 80286

                mov     bl,f80286
                jmp     TestDone

;*** Test CR0 register bit 4 (386SX won't allow 1)
Test05:         P386
                mov     eax,cr0
                mov     ebx,eax                 ;Original CR0 into EBX
                or      al,10h                  ;Set bit
                mov     cr0,eax                 ;Store it
                mov     eax,cr0                 ;Read it back
                mov     cr0,ebx                 ;Restore CR0
                test    al,10h                  ;Did it set?
                jnz     Test06                  ;Go if not 386SX

                mov     bl,f80386SX
                jmp     TestDone

;*** Test AC bit in EFLAGS (386DX won't change)
Test06:         mov     ecx,esp                 ;Original ESP in ECX
                pushfd                          ;Original EFLAGS in EBX
                pop     ebx
                and     esp,not 3               ;Align stack to prevent 486
                                                ;  fault when AC is flipped
                mov     eax,ebx                 ;EFLAGS => EAX
                xor     eax,40000h              ;Flip AC flag
                push    eax                     ;Store it
                popfd
                pushfd                          ;Read it back
                pop     eax
                push    ebx                     ;Restore EFLAGS
                popfd
                mov     esp,ecx                 ;Restore ESP
                cmp     eax,ebx                 ;Compare old/new AC bits
                jne     Test07                  ;If AC changes, not 386

                mov     bl,f80386DX
                jmp     SHORT TestDone

Test07:         mov     bl,f80486               ;Until the Pentium appears...

TestDone:       call    PrintCPU
                call    PrintPiq

;*** At this point, BL=CPU type, DL=PIQlen
                mov     al,bl
                mov     ah,4Ch
                int     21h

;***************************************
;* On exit:
;*   DX = Length of PIQ (0..64)
;*   AL, CX, DI destroyed
;*   Assumes ES = CS
;***************************************
PROC            PiqCalc

MaxPIQ          EQU     64                      ;Maximum PIQ to check
OpIncDX         EQU     42h
OpNop           EQU     90h

                jmp     PiqCalcEntry

                ALIGN   16                      ;PIQ must start on para.

PiqStart:       rep stosb
Piq:            db      MaxPIQ dup (OpIncDX)    ;Original code is INC DX

                ALIGN   2
                sti
                add     dx,2                    ;Adjust for 'REP STOSB'
                ret

PiqCalcEntry:   cld
                xor     dx,dx                   ;Reset PIQ counter
                mov     cx,MaxPIQ               ;Reset PIQ to "INC DX"s
                mov     di,OFFSET Piq
                mov     al,OpIncDX
                rep stosb
                mov     cx,MaxPIQ
                mov     di,OFFSET Piq
                mov     al,OpNop                ;Opcode for NOP
                cli
                jmp     PiqStart

ENDP

PROC            PrintCPU
                mov     dx,OFFSET CPUa$         ;Print start of message
                mov     ah,9
                int     21h

                mov     si,OFFSET CPUb$         ;Search for proper string
PrintCPU_10:    cmp     bl,[si]
                je      PrintCPU_90
                add     si,9
                jmp     PrintCPU_10

PrintCPU_90:    inc     si                      ;Print string
                mov     dx,si
                mov     ah,9
                int     21h
                mov     dl,13                   ;Print CR/LF
                mov     ah,2
                int     21h
                mov     dl,10
                mov     ah,2
                int     21h
                ret

CPUa$           db      'CPU type: $'
CPUb$           db      00h,'8088$   '
                db      01h,'8086$   '
                db      10h,'NEC V20$'
                db      11h,'NEC V30$'
                db      12h,'80188$  '
                db      13h,'80186$  '
                db      20h,'80286$  '
                db      30h,'80386SX$'
                db      31h,'80386DX$'
                db      40h,'80486$  '
                db      0FFh,'Unknown$'
ENDP

PROC            PrintPiq
                call    PiqCalc
                push    dx
                mov     ax,dx                   ;Ensure PIQlen is valid
                cmp     ax,MaxPIQ
                ja      PiqErr
                mov     dl,10                   ;Convert PIQlen to ASCII
                div     dl
                add     ax,3030h
                mov     [PiqStore],al           ;Store result
                mov     [PiqStore+1],ah
                mov     dx,OFFSET Piq$          ;Print result
                mov     ah,9
                int     21h
                pop     dx
                ret

PiqErr:         mov     dx,OFFSET PiqErr$       ;Print error message
                mov     ah,9
                int     21h
                ret

Piq$            db      'PIQ = '
PiqStore        db      '?? Bytes',13,10,'$'
PiqErr$         db      'PIQ length invalid!',13,10,'$'
ENDP

                END     Start
