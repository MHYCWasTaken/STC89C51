; Written by Chris Barrett (3:690/660.25)                        
; The following non table-based routine produces the same CRC16's
; as required by the EMSI standard so I assume it's correct.     
; Released into the Public Domain                                

; Pass    - DS:SI = pointer to the buffer
;         - CX    = length of the buffer 
; Returns - DX    = CRC16 of the buffer  

CRC16    PROC NEAR

         PUSH AX
         PUSH BX
         PUSHF
         CLD                      ; Move forward through the buffer

         SUB  DX,DX               ; CRC := 0000h

C1:      LODSB                    ; AL := byte at DS:SI
         SUB  AH,AH

         XCHG AH,AL               ; AX := 256 * AL
         XOR  DX,AX               ; CRC := CRC xor AX

         PUSH CX
         MOV  CX,8

C2:      MOV  BX,DX
         SHL  DX,1

         AND  BX,8000h
         JZ   C3

         XOR  DX,1021h
C3:      LOOP C2
         POP  CX

         LOOP C1

         POPF
         POP  BX
         POP  AX
         RET
CRC16    ENDP
