;*** MYPATH.ASM - Written by Dave M. Walker @ 1:396/1
;*** Released into the public domain with no reservations.

                IDEAL
                MODEL   TINY

                DATASEG
PathBuffer      db      128 dup (?)

                CODESEG
                ORG     0100h

EnvSeg          EQU     002Ch

Start:          mov     bx,OFFSET PathBuffer    ;Call our routine
                call    GetPathName
                mov     si,bx                   ;Print the path
                call    DispString
                mov     ax,4C00h                ;Exit
                int     21h

PROC            DispString
DispStringLoop: mov     dl,[si]
                inc     si
                cmp     dl,0
                je      DispStringExit
                mov     ah,2
                int     21h
                jmp     DispStringLoop

DispStringExit: ret
ENDP

;***************************************
;* Find pathname at end of environment
;* Entry: DS:BX = Buffer
;* Exit : Pathname copied in the format
;*        "D:\PATH\"
;* Regs : None
;* Note : Requires DOS 3 or higher
;***************************************
PROC            GetPathName
                push    ax cx si di ds es       ;Save modified regs

                mov     es,[cs:EnvSeg]          ;Point ES:DI to EnvSeg:0000
                xor     di,di
FindEnvEnd:     inc     di
                cmp     [WORD es:di],0          ;Two NULLs means end of env.
                jne     FindEnvEnd
                add     di,4                    ;Skip NULLs & trail counter
                mov     si,di                   ;Save pointer

;*** Find end of pathname (null byte)
                mov     cx,0FFFFh               ;Set CX to a safe limit
                cld                             ;Search forward
                repne scasb
                sub     di,2                    ;Backup to end of string

;*** Find last backslash to strip filename.ext
                mov     al,'\'
                std                             ;Search backward
                repne scasb
                inc     di                      ;Include last backslash

;*** Copy pathname into buffer
                mov     cx,di                   ;Count = (end - start) + 1
                sub     cx,si
                inc     cx
                mov     di,bx
                push    ds                      ;Swap ES & DS for block move
                push    es
                pop     ds
                pop     es
                cld
                rep movsb
                xor     ax,ax                   ;Terminate with NULL
                stosb

                pop     es ds di si cx ax       ;Recover caller's regs
                ret
ENDP

                END     Start
