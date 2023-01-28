page 66,132


CODESEG SEGMENT BYTE PUBLIC 'CODE'

CHECKPR PROC FAR
ORG 00h
        ASSUME CS:CODESEG

START:
      jmp  begin
Copyright db 13,'CHECKPR.BIN Beta version, Copyright (c) R. R. Freeland/Synergy Corp. 12/86',
          db 13,10,'no rights reserved--Merry Christmas',26

begin:
      cmp  byte ptr DS:[bx],'0'        ;first check that param is between 0-2
      jb   wrong_param                 ;and jump to error routine if not
      cmp  byte ptr DS:[bx],'2'
      ja   wrong_param
      xor  dx,dx
      mov  dl,byte ptr DS:[bx]         ;move in param to dx (which printer)
      sub  dl,30h                      ;change it to number
      mov  ah,2                        ;service 2 of
      int  17h                         ;int 17h (check printer status)
      cmp  ah,90h                      ;not busy and selected=90h=10010000
      je   OK                          ;if it matches return "O"(K)
      cmp  ah,02                       ;I dunno, seems to give the right response
      jz   invalid                     ;when no printer there
      cmp  ah,0b8h                     ;offline?
      jz   offline
      cmp  ah,0b0h                     ;not connected?
      jz   noconnect
      test ah,00100000b                ;out of paper?
      jnz  paperout
      test ah,00010000b                ;not selected?
      jz   noselect
      test ah,00001000b                ;i/o error?
      jnz  ioerror
      mov  byte ptr DS:[bx],'U'        ;U=Unknown problem
      jmp  done

wrong_param:
      mov  byte ptr DS:[bx],'W'        ;W=wrong parameter
      jmp  done

offline:
      mov  byte ptr DS:[bx],'L'        ;L=offLine
      jmp  done

noconnect:
      mov  byte ptr DS:[bx],'C'        ;C=not Connected
      jmp  done

invalid:
      mov  byte ptr DS:[bx],'V'        ;V=inValid printer
      jmp  done

paperout:
      mov  byte ptr DS:[bx],'P'        ;P=paperout
      jmp  done

ioerror:
      mov  byte ptr DS:[bx],'I'        ;I=i/o error
      jmp  done

noselect:
      mov  byte ptr DS:[bx],'N'        ;N=not selected
      jmp  done

OK:
      mov  byte ptr DS:[bx],'O'        ;O=OK
      jmp  done

done:
      RET

CHECKPR ENDP
CODESEG ENDS
END START

