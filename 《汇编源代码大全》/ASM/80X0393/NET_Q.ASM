%PAGESIZE 55,200
%SUBTTL "Get List of Queue Servers under Netware 3.11"
; Net_Q.Asm
;
; written on Tue  12-08-1992  by Ed Beroset
;

  .MODEL SMALL

  IDEAL

 STACK 100h

MACRO DOSint function
  mov ah,function
  int 21h
ENDM

 DATASEG
  STDOUT = 1       ;  the stdout device handle

  DOS_WRITE_TO_HANDLE = 040h  ; Write to File Handle
  DOS_TERMINATE_EXE   = 04Ch  ; Terminate Program

  NOVELL_FUNCTION     = 0E3h
;
; Object Types
;   note that they're all big endian
;
  OT_USER             = 0100h
  OT_USER_GROUP       = 0200h
  OT_PRINT_QUEUE      = 0300h ; Print Queue object type
  OT_FILE_SERVER      = 0400h


BragMsg DB     0dh,0ah,"NET_Q.EXE",9,"  written by Ed Beroset on Tue 12-08-1992"
        DB     9,"Version 1.00",0dh,0ah
        DB     9,9,"released to the public domain by the author",0dh,0ah,0dh,0ah
BragLen =  $ - BragMsg

Crlf   DB      0dh,0ah,0

    STRUC SCAN_REQ          ; bindery ScanObject request packet structure
    Length     DW  55       ; the length of this buffer
    Function   DB  37h      ; scan object subfunction number
    ObjectID   DD  -1       ; all ones for initial object search
    ObjectType DW  -1       ; wild card -- looks for all objects
    ObjNameLen DB  1        ; at least one character
    ObjName    DB  47 DUP ('*') ; fill with wildcards to start
    ENDS  SCAN_REQ

    STRUC SCAN_REP          ; bindery ScanObject request packet structure
    Length     DW  57
    ObjectID   DD  0       ; all ones for initial object search
    ObjectType DW  0       ; wild card -- looks for all objects
    ObjName    DB  48 DUP (0) ; fill with wildcards to start
    ObjFlag    DB  0
    ObjSecurty DB  0
    ObjHasProp DB  0
    ENDS  SCAN_REP

    ScanObjReq SCAN_REQ <>
    ScanObjRep SCAN_REP <>

 CODESEG

;
; This is the main part of the code
;
; Test code gets and prints the name of all print queues from the
; logged server -- NO ERROR CHECKING IS DONE, so be careful!
;

Start:
  mov  ax,@data
  mov  ds,ax                ; set up the data segment
  mov  dx,OFFSET BragMsg    ; prepare to print out brag line(s)
  mov  cx,BragLen
  mov  bx,STDOUT            ; print to STDOUT
  DOSint DOS_WRITE_TO_HANDLE
  jc   Exit                 ; if carry is set, there was an error

  mov     [ScanObjReq.ObjectType],OT_PRINT_QUEUE
  ;
  ;  in this case the name is already set up, (a wildcard) but if a
  ;  specific name were desired, it would be moved to
  ;  ScanObjReq.ObjName, with the appropriate length (not including
  ;  optional terminating NULL char set up in ScanObjReq.ObjNameLen.
  ;
@@MoreQueues:
  call    BindScan
  jc      Exit

  lea     dx,[ScanObjRep.ObjName]
  call    Puts
  lea     dx,[Crlf]
  call    Puts
  jmp     @@MoreQueues

Exit:
  DOSint DOS_TERMINATE_EXE  ; return with error code preset in AL

;
; BindScan
;
; scans the bindery for the object name set in the request buffer
;
PROC    BindScan
    push    ds si di es dx ax

    lea     si,[ScanObjReq]     ; point DS:DI to request buffer
    mov     dx,ds
    mov     es,dx
    lea     di,[ScanObjRep]     ; point ES:SI to reply buffer
    DOSint  NOVELL_FUNCTION
    jb      @@Exit

    cld                         ; make sure to count up
    mov     si,OFFSET ScanObjRep.ObjectID
    mov     di,OFFSET ScanObjReq.ObjectID
    movsw
    movsw

    clc

@@Exit:
    pop     ax dx es di si ds
    ret

ENDP    BindScan     ;end of proc

; Puts
;
; prints a NUL terminated string to stdout
;
; INPUTS: ds:dx  points to ASCIIZ string
;
; OUTPUTS: prints string to stdout
;
; RETURNS: ax = number of bytes actually printed
;          carry set on error
;
; DESTROYED: ax
;
PROC Puts
  push bx cx di es

  push ds
  pop  es
  mov  cx,0ffffh    ; maximum length of string
  mov  di,dx
  cld
  mov  al,0         ; we're looking for NUL
  repne scasb
  dec  di
  mov  cx,di
  sub  cx,dx
  mov  bx,STDOUT    ; write to this device
  DOSint DOS_WRITE_TO_HANDLE

  pop  es di cx bx
  ret
ENDP Puts

  END Start
