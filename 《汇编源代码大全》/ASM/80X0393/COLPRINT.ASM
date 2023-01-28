; Routine by Rob Green.  
; Released to the Public Domain on Feb. 20th 1993.
;

                .MODEL   small
                .code
;==============================================================
; This routine displays a string at the current cursor location
; DS:DX point to the string to display.  If a #0 is found, then
; the next byte is the attribute.  the string is '$' terminated
;

Attrib  Db  07
X       Db  00
Y       Db  00

Print  proc near
        push ax            ; Save registers.. Dont want to
        push bx            ; be like Microsoft
        push cx
        push si
        push dx
        push ds
        mov ah,3           ; Gets the current cursor
        mov bh,0           ; Location, so we know were
        int 010h           ; to print
        mov x,dl           ; and save
        mov y,dh
        pop ds
        pop dx
        push dx
        cld                ; Not needed but habit ;)
        mov si,dx          ; and setup our counter


GetChr: mov al,ds:[si]     ; Get character,  LODSB wouldnt
                           ; work for some reason
        inc si             ; Inc pointer
        cmp al,0           ; Is it an attribute?
        jz Attr            ; Yes, change the Attrib.
        cmp al,'$'         ; Is it end of String
        jz endStr          ; Yes, do cleanup
        cmp al,0dh         ; Is it Enter?
        jne @@1            ; No, check for LF
        mov x,0            ; Enter puts the cursor at begining of
                           ; line
        jmp short getchr   ; Dont print, 010h/09h prints literals only
   @@1: cmp al,0ah         ; Is it a LF
        jne @@2            ; No, go print
        inc y              ; Linefeeds advances the cursor row
        cmp y,26  ;decimal ; Check to see if time for a scroll
        jne getchr         ; I omitted this code..
        mov y,25           ; This takes into account of
                           ; scrolling (decimal)
                           ; Scrolling routine omitted
        jmp short getchr   ; And dont print lf, literals only
   @@2: push si            ; This was causing me trouble!!
        push ax            ; I kept getting trash on the screen
        push bx            ; and i couldnt figure out why
        push dx            ; amazing what can happen by just saving
        mov ah,02h         ; the registers..  like work ;)
        mov bh,0h          ; This sets the cursor location before print
        mov dh,y
        mov dl,x
        int 010h
        pop dx             ;I wish Dos wouldnt trash the registers!!!
        pop bx             ;
        pop ax             ;
        push dx            ;
        push bx            ;
        push ax            ;
        mov ah,09h         ; And this prints a character to the
        mov bh,0           ; screen
        mov bl,attrib
        mov cx,1
        int 010h
        pop ax
        pop bx
        pop dx
        inc x              ; Increments the Column
        pop si
        jmp short getchr   ; And do it all over again

; Changes the attribute
attr:   mov al,ds:[si]
        inc si
        mov attrib,al
        jmp short getchr

; got a '$' and ready to RET
endstr:
        pop dx
        pop si
        pop cx
        pop bx
        pop ax
        ret
print endp

start proc far
   push ds
   xor ax,ax
   push ax
   mov ax,@data
   mov ds,ax
   mov es,ax
   mov dx,offset testmsg
   call print
   mov ax,04c00h
   int 021h
start endp

.data

testMsg db 0,0eh,'Yellow ',0,1eh,'Yellow/Blue ',0,08eh,'Blinking Yellow'
        db 0,04h,0dh,0ah,'This is in Red',0dh,0ah
        db 0,0fh,'This is in White'
        db '$'

.stack 0200h
     end start
