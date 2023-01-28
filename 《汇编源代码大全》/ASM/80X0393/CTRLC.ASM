; CTRLC.ASM
; Trap Ctrl-C, Ctrl-Break, and PrintScreen
; Public domain by Matthew Hildebrand
; This module may not be overlaid

IDEAL
P386                                  ; no 386 code; just for alignment
MODEL LARGE

        CODESEG

        LABEL old9Vect DWORD
old9Off        dw        ?
old9Seg        dw        ?
old1BOff       dw        ?
old1BSeg       dw        ?


        PUBLIC C disableCtrlC
PROC C disableCtrlC
  push ds

  mov ax,3509h                        ; get the old 09h vector
  int 21h
  mov [old9Off],bx
  mov [old9Seg],es
  mov ax,251Bh                        ; get the old 1Bh vector
  int 21h
  mov [old1BOff],bx
  mov [old1BSeg],es

  mov ax,2509h                        ; set the new 09h vector
  mov dx,SEG isr
  mov ds,dx
  mov dx,OFFSET isr
  int 21h
  mov ax,251Bh                        ; set the new 1Bh vector
  mov dx,SEG ctrlBreak
  mov ds,dx
  mov dx,OFFSET ctrlBreak
  int 21h

  pop ds
  retcode
ENDP

        PUBLIC C enableCtrlC
PROC C enableCtrlC
  push ds

  mov ax,2509h                        ; restore the old 09h vector
  mov ds,[old9Seg]
  mov dx,[old9Off]
  int 21h
  mov ax,251Bh                        ; restore the old 1Bh vector
  mov ds,[old1BSeg]
  mov dx,[old1BOff]
  int 21h

  pop ds
  retcode
ENDP

PROC isr FAR
  push ax dx es

  in al,60h                           ; read keyboard input
  mov ah,al                           ; store it
  test al,80h                         ; was key pressed or released?
  jz @@L1
  xor dl,dl                           ; released
  jmp short @@L2
        @@L1:
  mov dl,1                            ; pressed

        @@L2:
  ; Trap unwanted keystrokes
  and al,01111111b                    ; strip high bit

  cmp al,55                           ; PrintScreen
  je @@Exit

  cmp al,46                           ; C
  jne @@oldHandler
  mov ah,12h                          ; Get enhanced keyboard flags
  int 16h
  test al,00000100b                   ; bit for either Ctrl key pressed
  jne @@Exit

        @@oldHandler:                 ; Pass control to old handler
  pushf                               ; so old9Vect's iret will work
  call [old9Vect]
  pop es dx ax
  iret

        @@Exit:                       ; Ignore a keystroke
  in al,61h                           ; acknowledge keystroke
  mov ah,al
  or al,80h
  out 61h,al
  mov al,ah
  out 61h,al
  mov al,20h
  out 20h,al
  pop es dx ax                        ; clean up and leave
  iret
ENDP

PROC ctrlBreak FAR
  iret
ENDP

        ENDS
END
