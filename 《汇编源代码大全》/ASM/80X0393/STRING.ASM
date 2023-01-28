comment #

   STRING.ASM
   Finding strings in memory

   Author: Dave Walker
   placed in public domain
   
  The actual search is quite easy, due to the magical "rep cmpsb"
  instruction. The biggest problem is normalizing the addresses. 
  If you are starting with an arbitrary address, just do this:

        While (offset >= 10h)
          offset = offset-10h
          segment = segment+1
#

            IDEAL
            MODEL   TINY

            CODESEG
            ORG     100h

start:      mov     dx,OFFSET askmsg         ;Ask user for string
            mov     ah,9
            int     21h

            mov     dx,OFFSET inbuf          ;Get input
            mov     ah,0ah
            int     21h

            xor     ax,ax                    ;Init target to 0:0
            mov     es,ax
            mov     di,ax
            cld

mainloop:   mov     ch,0                     ;Init length
            mov     cl,[inbuf+1]
            mov     si,OFFSET string         ;Init source

            push    es                       ;Perform search
            push    di
            rep cmpsb
            pop     di
            pop     es
            jz      foundit

bump:       inc     di                       ;Next mem location
            cmp     di,16                    ;Past paragraph boundary?
            jb      mainloop
            sub     di,16                    ;If so, adjust DI and ES

            push    es                       ;INC ES (grrr...)
            pop     ax
            inc     ax
            push    ax
            pop     es

            cmp     ax,0a000h                ;Did we pass 640k?
            jb      mainloop

            mov     ax,4c00h                 ;Bye-bye
            int     21h

foundit:    mov     ax,es                    ;Convert segment to ASCII
            mov     dx,OFFSET foundaddr
            call    binhex
            mov     ax,di                    ;Convert offset to ASCII
            mov     dx,OFFSET foundaddr+5
            call    binhex
            mov     dx,OFFSET foundmsg       ;Tell user where we found
            mov     ah,9                     ;  the string
            int     21h
            jmp     bump

PROC        binhex                           ;Convert AX to ASCII and
            push    ax                       ;  store result at [DS:DX].
            push    bx                       ;  All regs (including
            push    cx                       ;  DX) are preserved.
            push    dx
            mov     bx,dx
            mov     dl,ah
            mov     cl,4
            shr     dl,cl
            call    bh10
            mov     dl,ah
            and     dl,15
            call    bh10
            mov     dl,al
            shr     dl,cl
            call    bh10
            mov     dl,al
            and     dl,15
            call    bh10
            pop     dx
            pop     cx
            pop     bx
            pop     ax
            ret

bh10:       cmp     dl,10
            jb      bh11
            add     dl,7
bh11:       add     dl,'0'
            mov     [bx],dl
            inc     bx
            ret
ENDP

askmsg      db      'Enter search string : $'
foundmsg    db      13,10,'String found at '
foundaddr   db      'ssss:oooo','$'
inbuf       db      253,?
string      db      253 dup (?)

            END     start
