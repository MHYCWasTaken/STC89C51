comment *
    SERTYPE.ASM
    Purpose:
    Determines the type of UART in each serial port

    Author:
    Douglas Boling, in PcMag. With some modifications by Yousuf Khan.
*

dosseg

bdseg   segment at 40h  ;bios data segment
        com1    dw      ?
        com2    dw      ?
        com3    dw      ?
        com4    dw      ?
        ends

_data   segment para 'data'
        uart1   db      0
        uart2   db      0
        uart3   db      0
        uart4   db      0
        portmsg db      "COMx: $"
        x8250   db      "NS 8250 (non-FIFO)",13,10,"$"
        x16450  db      "NS 8250A/16450/16550 (non-FIFO)",13,10,"$"
        xfifo   db      "NS 16550A/Intel 82510 (FIFO)",13,10,"$"
        xdma    db      "IBM type 3 (FIFO/DMA)",13,10,"$"
        pas_de_ports    db      "No serial ports detected",13,10,"$"
        ends

_stack  segment para stack 'stack'
        db      200h dup (?)
        ends

cseg    segment para 'code'
        mov     ax, bdseg
        mov     es, ax          ;ES := Bdseg
        mov     ax, _data
        mov     ds, ax          ;DS := _data
        assume  cs:cseg, ds:_data, es:bdseg, ss:_stack
        xor     al, al          ;# of serial ports = 0
        mov     dx, [com1]
        cmp     dx, 0
        je      eoproc
        inc     al              ;# of serial ports = 1
        push    ax
        call    uartdet
        mov     [uart1], al     ;remember the type of this UART
        pop     ax
        mov     dx, [com2]
        cmp     dx, 0
        je      eoproc
        inc     al              ;# of serial ports = 2
        push    ax
        call    uartdet
        mov     [uart2], al
        pop     ax
        mov     dx, [com3]
        cmp     dx, 0
        je      eoproc
        inc     al              ;# of serial ports = 3
        push    ax
        call    uartdet
        mov     [uart3], al
        pop     ax
        mov     dx, [com4]
        cmp     dx, 0
        je      eoproc
        inc     al              ;# of serial ports = 4
        push    ax
        call    uartdet
        mov     [uart4], al
        pop     ax

eoproc:
        call    disp
        mov     ah, 4ch
        int     21h

delay   macro
        jmp     $+2
        endm

uartdet proc    near
        push    bx
        push    cx
        mov     bx, dx          ;save starting i/o addr
        add     dx, 4           ;point to modem ctrl reg
        in      al, dx          ;disable interrupts
        push    ax
        and     al, 11111011b
        out     dx, al
        mov     ch, 0           ;assume type 0
        mov     dx, bx
        add     dx, 7           ;see if scratch reg exists
        mov     al, 55h
        cli
        out     dx, al          ;write to scratch reg
        delay
        in      al, dx          ;read back
        cmp     al, 55h
        jne     endudet
        mov     al, 0AAh
        out     dx, al          ;write to scratch reg
        delay
        in      al, dx          ;read back
        sti
        cmp     al, 0AAh
        jne     endudet
        inc     ch              ;assume type 1
        mov     dx, bx
        add     dx, 2           ;point to FIFO ctrl reg
        mov     al, 7           ;attempt to enable FIFOs
        cli
        out     dx, al
        delay
        in      al, dx          ;read interrupt ID reg
        sti
        and     al, 0c0h        ;strip all but FIFO bits
        jz      endudet         ;if bits 0, 16450/16550
        inc     ch              ;assume type 2
        mov     dx, bx
        add     dx, 8003h       ;point to enhanced reg 1
        cli
        in      al, dx
        push    ax
        or      al, 01000000b   ;enable DMA transmission
        out     dx, al
        push    dx
        mov     dx, bx
        add     dx, 2
        in      al, dx
        mov     cl, al
        pop     dx              ;restore enhanced reg 1
        pop     ax
        out     dx, al
        sti
        and     cl, 0c0h        ;again mask all but FIFO ID
        cmp     cl, 40h
        jne     endudet         ;must be type 2 (FIFO)
        inc     ch              ;must be type 3 (DMA)

endudet:
        pop     ax
        mov     dx, bx
        add     dx, 4           ;point to modem ctrl reg
        out     dx, al          ;restore initial condition
        xor     ax, ax
        mov     al, ch
        mov     dx, bx
        pop     cx
        pop     bx
        ret
uartdet endp

disp    proc    near
        push    ax      ;save AX
        cmp     al, 0   ;no serial ports?
        je      noports
        mov     bx, offset ds:[uart1]   ;offset of UART type field
        mov     di, offset ds:[portmsg][3] ;offset of 4th char in message
        mov     cx, 0
        mov     cl, al  ;number of iterations

@l1:
        ;
        ;write "COMx: " message substituting "x" for proper comport #
        ;
        mov     dl, 4
        sub     dl, cl
        add     dl, 30h ;convert to ASCII number
        mov     [di], dl
        lea     dx, portmsg
        mov     ah, 9
        int     21h
        ;
        ;write the UART type now
        ;
        mov     dl, [bx]
        cmp     dl, 0
        jne     @t2
        lea     dx, x8250
        mov     ah, 9
        int     21h
        jmp     @eot

@t2:    cmp     dl, 1
        jne     @t3
        lea     dx, x16450
        mov     ah, 9
        int     21h
        jmp     @eot

@t3:    cmp     dl, 2
        jne     @t4
        lea     dx, xfifo
        mov     ah, 9
        int     21h
        jmp     @eot

@t4:    lea     dx, xdma
        mov     ah, 9
        int     21h

@eot:   inc     bx
        loop    @l1
        jmp     eoproc2

noports:
        lea     dx, pas_de_ports
        mov     ah, 9
        int     21h

eoproc2:
        pop     ax
        ret
disp    endp

        ends
        end

; EOF SERTYPE.ASM