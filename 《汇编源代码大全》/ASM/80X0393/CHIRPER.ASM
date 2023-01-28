;-----------------------------------------------------------------------
; Chirper.asm - A TSR that gives each key a musical tone for audio feedback.
; This program is useful for those with visual handicaps as well as those
; that are just learning to type. After learning what each character typed
; sounds like the user will be able to audibly detect if the wrong key has
; been pressed.
;
; Released to the public domain by Garry Freemyer on 02/04/93
; Send me a note if You like it.   5829 Sawmill Road.
;                                  Paradise, CA. 95969
;                                  (916) 877-7015
;-----------------------------------------------------------------------


cseg    segment    para    public 'code'
        assume cs:cseg
        org 100h

start:  jmp initialize

  old9_hndlr    label dword     ;old int 9h handler
  old9_off      dw      ?
  old9_seg      dw      ?
  notes         dw      0
                dw      18356
                dw      17292
                dw      16344
                dw      15495
                dw      14550
                dw      13714
                dw      12969
                dw      12175
                dw      11472
                dw      10847
                dw      10198
                dw       9700
                dw       9108
                dw       8584
                dw       8116
                dw       7648
                dw       7231
                dw       6818
                dw       6449

                dw       6087
                dw       5736
                dw       5423
                dw       5120
                dw       4830
                dw       4554
                dw       4307
                dw       4058
                dw       3836
                dw       3615
                dw       3418
                dw       3224
                dw       3043
                dw       2875
                dw       2711
                dw       2560
                dw       2415
                dw       2281
                dw       2153
                dw       2032
                dw       1918
                dw       1810
                dw       1709
                dw       1612
                dw       1521
                dw       1435
                dw       1355
                dw       1280
                dw       1207
                dw       1140
                dw       1075
                dw       1015
                dw        959
                dw        898
                dw       1709
                dw        854
                dw        806
                dw        760
                dw      65535   ;Caps Lock,
                dw        718
                dw        677
                dw        639
                dw        604
                dw        570
                dw        538
                dw        507
                dw        479
                dw        452
                dw        427
                dw        403
                dw        380
                dw        359
                dw        338
                dw        319
                dw        301
                dw        285
                dw        268
                dw        253
                dw        239
                dw        225
                dw        213
                dw        201
                dw        189
                dw        179
                dw        169
                dw        160
                dw        151
                dw        403
                dw        380

  ax_           dw      ?
  bx_           dw      ?
  scancode      db      ?
  toggle        db      1       ;turns program on or off. 1 = on.
  load_msg      db      0ah,0dh,"Chirper installed! Toggle: SysRq or Alt-SysRq for 101 Keyboard.",0ah,0dh
                db      "Toggle = SysRq.",10,13
                db      "By Garry Freemyer-916-877-7015 U.S. (Freeware)",0ah,0dh,"$"

  ;*************************************************************************
  ;New handler for int 9h (keyboard hardware interrupt)
  ;*************************************************************************

  new9_hndlr    proc
    mov ax_,ax                  ;save ax register.
    xor ax,ax                   ;zero ax register.
    in al,60h                   ;get scancode.
    mov scancode,al             ;store scancode.
    mov ax,ax_
    pushf                       ;simulate int
    call  old9_hndlr
    xor ax,ax

    mov al,scancode            ;ignore caps lock key.
    cmp al,3ah
    je exit_9

    cmp al,54h
                ;has sys req been pressed?
    jne continue

    mov al,toggle
    xor al,1                   ;toggle toggle
    mov toggle,al
    cmp al,0
    je nosound                 ;turn sound off too when turning program off.

continue:
    mov al,toggle              ;test status of toggle.
    cmp al,0
    je  exit_9                 ;if program off skip sound routines.

    mov al,scancode            ;get scancode.
    cmp al,58h
    ja nosound                 ;turn sound off when key is released.

    mov bx_,bx                 ;save bx
    xor bx,bx                  ;zero bx
    add al,al
    mov bx,notes
    add ax,bx                  ;ax = vector address into notes array.
    mov bx,ax
    mov al,0b6h                ;turn on sound
    out 43h,al
    mov ax,notes[bx];
    out 42h,al
    mov al ,ah
    out 42h,al
    in al,61h
    or al,3
    out 61h,al
    mov bx,bx_
    jmp exit_9

nosound:                        ;turn off sound, key is released.
     in al,61h
     and al,252
     out 61h,al

exit_9:
    mov ax,ax_
    iret
  new9_hndlr    endp

last_byte   db   "$"

;------------------------------------------------------------------------
; Initialization procedure
;------------------------------------------------------------------------

  initialize    proc    near
  assume        ds:cseg         ;variables in this segment


  ;Insert new handlers into interrupt chain
  ;
    mov ax,cs
    mov ds,ax

    mov ax,3509h                ;get interrupt 9 vector
    int  21h
    mov  old9_off,bx
    mov  old9_seg,es

    mov  ax,2509h
    mov  dx,offset new9_hndlr
    int  21h

  ;Display message, then terminate but stay resident
  ;
    mov  dx,offset load_msg
    mov  ah,9
    int  21h

  ;amount of memory to retain in dx
    mov  dx,(offset last_byte - offset cseg + 15)
    mov  cl,4
    shr  dx,cl                  ;convert to paragraphs
    mov  ax,3100h               ;TSR function
    int  21h
  initialize    endp
  ;
  cseg          ends
    end         start           ;end of program
