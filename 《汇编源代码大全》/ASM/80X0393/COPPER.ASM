COMMENT |

copper.asm

This file was released into the Public Domain by Doug Mansell
(3:690/613.11) on 29jul92.

On every horizontal retrace the same DAC is changed to the next colour
in a colour table.

C callable as:

    void pascal copper(int lines, unsigned char DACnum,
                       void *colourtable, int count);

    'lines' is the number of scanlines on the screen (note that in some
    modes the number of scanlines may be twice as much as the number of
    pixel rows).  By using a value slightly higher, or slightly lower
    than the actual number of scanlines, you can get the colours to roll
    up or down the screen!

    'DACnum' is the DAC to mess with.

    'colourtable' points to an array of RGB values, something like:
        struct _rgb {
            unsigned char red, green, blue;
        } colourtable[480];

    'count' is the number of retraces to do.

IMPORTANT:
    - Interrupts are disabled for a long time: at least, your DOS clock
      will stop!
    - An 80186 instruction (OUTS) is used to help speed up the DAC
      setting, thus a 186 or better is required to call copper.
    - copper uses the CRTC status port of the _colour_ VGA.

My mileage:
    This assembles with TASM 2.01 and works under DOS on my 286 with ye
    olde VGA card.

|

        .MODEL SMALL
        .CODE

                PUBLIC COPPER

                .186

COPPER          PROC
                ARG count:WORD, colours:DATAPTR, DACnum:BYTE:2, lines:WORD = retbytes
                push bp
                mov  bp,sp

                push si                 ; save C register variables
                push di

                ; setup registers and stuff
                mov  ah,[DACnum]
                xor  ch,ch
                mov  dx,003dah          ; 3da = crtc status port
                mov  bx,0dac8h          ; 3c8 = DAC port
                cld                     ; increment si through colours
                cli                     ; keeps things stable

vsync1:         in   al,dx              ; wait for vsync to happen
                test al,8
                jz   vsync1
vsync2:         in   al,dx              ; wait for vsync to finish
                test al,8
                jnz  vsync2

ALIGN 2
colourstart:    mov  di,[lines]         ; set line counter
                mov  si,[colours]       ; point si to colour table

ALIGN 2
dolines:        mov  cl,3               ; prepare cx for OUTS
                mov  dl,bh              ; dx = CRTC status port

hsync1:         in   al,dx              ; wait for display on
                test al,1
                jnz  hsync1
hsync2:         in   al,dx              ; wait for display off
                test al,1
                jz   hsync2

                mov  dl,bl              ; dx = DAC index port (?)
                mov  al,ah              ; load DAC number
                out  dx,al              ; tell VGA which DAC we want to change
                inc  dx                 ; RGB stuff goes to next port
                rep  outsb              ; DAC is set!

                dec  di                 ; count a line
                jnz  dolines            ; do all lines

                dec  [count]            ; count another pass
                jnz  colourstart        ; do all passes

                sti                     ; enable interrupts

                pop  di                 ; restore register variables
                pop  si

                pop  bp
                ret  retbytes

COPPER          ENDP

                END
