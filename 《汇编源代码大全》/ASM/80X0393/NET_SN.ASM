%PAGESIZE 55,200
%TITLE "NetWare serial number routine"
; Net_SN.Asm
;
; written on Mon  06-01-1992 by Ed Beroset
; released to the public domain
;
; I've tested this code with Netware 386 version 3.11, 
; but it may also work with 2.15.  It wasn't documented 
; for 2.15, but it may still have existed.                                                             
;

  .MODEL SMALL
  IDEAL

 STACK 100h
 DATASEG

STDOUT    = 1   ; handle for stdout

    STRUC SNREQBUFF
    Length      DW  1       ; request structure length - 2
    Function    DB  12h     ; function number of GetNetworkSerialNumber
    ENDS SNREQBUFF

    STRUC SNREPLYBUFF
    Length      DW  6       ; reply structure length - 2
    NetSN       DD  0       ; network serial number in big endian packed BCD
    AppNumber   DW  0       ; Application number in same format
    ENDS SNREPLYBUFF

    U_Request SNREQBUFF    <>
    U_Reply   SNREPLYBUFF  <>
    SerialNum  DB  "00000000",0dh, 0ah
    SerialLen  = $ - SerialNum

 CODESEG
;
; Test code gets network serial number and prints it to stdout
;
Start:
  mov  ax,@data
  mov  ds,ax                ; set up the data segment
  call NetworkSN
Exit:
  mov  ah,04ch              ; return with error code preset in AL
  int  21h

;
; here's the Network stuff
;
PROC    NetworkSN
    push    ds
    push    si
    push    di
    push    es
    push    dx
    push    cx

    lea     si,[U_Request]      ; prepare to request data
    lea     di,[U_Reply  ]      ; prepare to receive data
    mov     ax,ds
    mov     es,ax
    mov     ah,0e3h             ; Get File Server Serial Number
    int     21h
    jc      @@NoMore

    lea     si,[U_Reply.NetSN]  ; point ds:si at binary data
    lea     di,[SerialNum]      ; and point es:di at target ASCII string
    mov     cx,4                ; loop four times (once for each SN digit pair)
    cld                         ; count up

@@convbyte:
    lodsb                       ; read a byte
    aam     16                  ; convert to two-digit BCD in ah,al
    xchg    ah,al               ; swap so that memory image will be correct
    or      ax,3030h            ; convert both to ASCII numbers
    stosw                       ; put 'em in our table
    loop    @@convbyte

    lea     dx,[SerialNum]      ; we're going to point ds:dx to string
    mov     cx,SerialLen        ; load the length of the string
    mov     bx,STDOUT           ; print to STDOUT
    mov     ah,40h              ; DOS function to print string
    int     21h                 ; do it
    mov     al,0                ; return with appropriate error code

@@NoMore:
    pop     cx
    pop     dx
    pop     es
    pop     di
    pop     si
    pop     ds
    ret

ENDP        NetworkSN   ;end of proc

  END Start
