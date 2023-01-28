TITLE   CPUID
;v5.93  Toad Hall Tweak, 1 Jul 91
;       - Tightened up code a little, tabified.
;         I have *no* idea what he did to make the original
;         CPUID.EXE 6Kb long!  debug enabled?
;       - Rewritten to .COM format to reduce size further.
;       We *really* should be returning a magic number as ERRORLEVEL
;       so a batch file could process accordingly.
;       David Kirschbaum
;       kirschd@sesi.com

;v5.94  DD Tweak, 8 Jan 92, to return errorlevels
;       This version assembles with Eric Isaacson's A86
;       Fixed the problem with "period" not always being printed
;       Doug Dougherty, valley@gsbsun.uchicago.edu

CSEG    SEGMENT
        ASSUME  CS:CSEG,DS:CSEG,ES:CSEG

        org     100H                    ;.COM file

cpuid   proc    near
        jmp     start                   ;skip over data

exitcode        db      0               ;v5.94

fp_status       dw      0
id_mess         db      "CPUID v5.94 (Toad Hall, DD tweaks)"            ;v5.94
                db      13,10,13,10,"This system has a$"                ;v5.94
fp_8087         db      " and an 8087 math coprocessor$"
fp_80287        db      " and an i287tm math coprocessor$"
fp_80387        db      " and an i387tm math coprocessor$"
c8086           db      "n 8086/8088 microprocessor$"
c286            db      "n 80286 microprocessor$"
c386            db      " i386tm microprocessor$"
c486            db      " i486tm DX microprocessor or i487tm SX math coprocessor$"
c486nfp         db      " i486tm SX microprocessor$"
period          db      ".",13,10,"$"
present_86      db      0       ;v5.92
present_286     db      0       ;v5.92
present_386     db      0       ;v5.92
present_486     db      0       ;v5.92

cpuid   endp

;
; The purpose of this code is to allow the user the ability to identify the
; processor and coprocessor that is currently in the system.  The algorithm
; of the program is to first determine the processor id.
; When that is accomplished, the program continues to then identify whether
; a coprocessor exists in the system.  If a coprocessor or integrated
; coprocessor exists, the program will identify the coprocessor id.  If one
; does not exist, the program then terminates.
;
start   proc    near

        mov     dx,offset id_mess               ; print header message
        mov     ah,9h
        int     21h

;
;       8086 CPU check
;       Bits 12-15 are always set on the 8086 processor.
;
        mov     cx,0F000H               ;handy constant                 v5.93
        pushf                           ; save EFLAGS
        pop     bx                      ; store EFLAGS in BX
        mov     ax,0fffh                ; clear bits 12-15
        and     ax,bx                   ;       in EFLAGS
        push    ax                      ; store new EFLAGS value on stack
        popf                            ; replace current EFLAGS value
        pushf                           ; set new EFLAGS
        pop     ax                      ; store new EFLAGS in AX
        and     ax,cx   ;0f000h         ; if bits 12-15 are set,        v5.93
        cmp     ax,cx   ;0f000h         ; then CPU is an 8086/8088      v5.93
        mov     dx,offset c8086         ; store 8086/8088 message
        mov     present_86,1            ; turn on 8086/8088 flag
        mov     exitcode,1              ; Setup exitcode for 8086/8088  v5.94
        je      check_fpu               ; if CPU is 8086/8088, check for 8087

;
;       80286 CPU check
;       Bits 12-15 are always clear on the 80286 processor.
;

        or      bx,cx   ;0f000h         ; try to set bits 12-15         v5.93
        push    bx
        popf
        pushf
        pop     ax
        and     ax,cx   ;0f000h         ; if bits 12-15 are cleared     v5.93
        mov     dx,offset c286          ; then CPU is an 80286
        mov     present_86,0            ; turn off 8086/8088 flag
        mov     present_286,1           ; turn on 80286 flag
        mov     exitcode,2              ; Setup exitcode for 80286      v5.94
        jz      check_fpu               ; if CPU is 80286, check for 80287

;
; i386 CPU check
; The AC bit, bit #18, is a new bit introduced in the EFLAGS register on the
;  i486 DX CPU to generate alignment faults.  This bit can be set on the
;  i486 DX CPU, but not on the i386 CPU.


        mov     bx,sp                   ; save current stack pointer to align it
        and     sp,not 3                ; align stack to avoid AC fault
        db      66h
        pushf                           ; push original EFLAGS
        db      66h
        pop     ax                      ; get original EFLAGS
        db      66h
        mov     cx,ax                   ; save original EFLAGS
        db      66h                     ; xor EAX,40000h
        xor     ax,0                    ; flip AC bit in EFLAGS
        dw      4                       ; upper 16-bits of xor constant
        db      66h
        push    ax                      ; save for EFLAGS
        db      66h
        popf                            ; copy to EFLAGS
        db      66h
        pushf                           ; push EFLAGS
        db      66h
        pop     ax                      ; get new EFLAGS value
        db      66h
        xor     ax,cx                   ; if AC bit cannot be changed, CPU is
        mov     dx,offset c386          ; store i386 message
        mov     present_286,0           ; turn off 80286 flag
        mov     present_386,1           ; turn on i386 flag
        mov     exitcode,3              ; Setup exitcode for 80386      v5.94
        je      check_fpu               ; if CPU is i386, now check for
                                        ;       80287/80387 MCP

;
;       i486 DX CPU / i487 SX MCP and i486 SX CPU checking
;
        mov     dx,offset c486nfp       ; store 486NFP message
        mov     present_386,0           ; turn off i386 flag
        mov     present_486,1           ; turn on i486 flag
        mov     exitcode,4              ; Setup exitcode for 80486      v5.94

;
; Co-processor checking begins here for the 8086/80286/i386 CPUs.
; The algorithm is to determine whether or not the floating-point status
; and control words can be written to.  If they are not, no coprocessor exists.
; If the status and control words can be written to, the correct coprocessor
; is then determined depending on the processor id.  Coprocessor checks are
; first performed for an 8086, 80286 and a i486 DX CPU.  If the coprocessor id
; is still undetermined, the system must contain a i386 CPU.  The i386 CPU may
; work with either an 80287 or an 80387.  The infinity of the coprocessor must
; be checked to determine the correct coprocessor id.
;

check_fpu:                              ; check for 8087/80287/80387
        fninit                          ; reset FP status word
        mov     fp_status,5a5ah         ; initialize temp word to non-zero value
        fnstsw  fp_status               ; save FP status word
        mov     ax,fp_status            ; check FP status word
;v5.93  cmp     al,0
        or      al,al                   ; see if correct status with written v5.93
        jne     print_one               ; jump if not Valid, no NPX installed

        fnstcw  fp_status               ; save FP control word
        mov     ax,fp_status            ; check FP control word
        and     ax,103fh                ; see if selected parts looks OK
        cmp     ax,3fh                  ; check that ones and zeroes correctly
                                        ; read
        jne     msgterm                 ; not valid, no NPX installed   v5.93

        cmp     present_486,1           ; check if i486 flag is on
        jne     not_486                 ;nope, continue 386 checking    v5.93
                                        ;yep, display i486 msg          v5.93
         mov    dx,offset c486          ; store i486 message
         jmp    msgterm                 ;display i486 message, terminate v5.93

not_486:
        cmp     present_386,1           ; check if i386 flag is on
        jne     print_87_287            ; if i386 flag not on, check NPX for
                                        ;       8086/8088/80286
        mov     ah,9h                   ; print out i386 CPU ID first
        int     21h

;
;   80287/80387 check for the i386 CPU
;
        fld1                            ; must use default control from FNINIT
        fldz                            ; form infinity
        fdiv                            ; 8087/80287 says +inf = -inf
        fld     st                      ; form negative infinity
        fchs                            ; 80387 says +inf <> -inf
        fcompp                          ; see if they are the same
                                        ; and remove them
        fstsw   fp_status               ; look at status from FCOMPP
        mov     ax,fp_status
        mov     dx,offset fp_80287      ; store 80287 message
        sahf                            ; see if infinities matched
        jz      restore_EFLAGS          ; jump if 8087/80287 is present
         mov    dx,offset fp_80387      ; store 80387 message

restore_EFLAGS:
        mov     ah,9h                   ; print NPX message
        int     21h
        db      66h
        push    cx                      ; push ECX
        db      66h
        popf                            ; restore original EFLAGS register
        mov     sp,bx                   ; restore original stack pointer
        jmp     short exit

print_one:
        jmp     short msgterm           ;print out CPU ID with no NPX   v5.93

print_87_287:
        mov     ah,9h                   ; print out 8086/8088/80286 first
        int     21h
        cmp     present_86,1            ; if 8086/8088 flag is on
        mov     dx,offset fp_8087       ; store 8087 message
        je      print_fpu
        mov     dx,offset fp_80287      ; else CPU=80286, store 80287 message

print_fpu:
        or      exitcode,080h           ; High bit => NPX present       v5.94
        jmp     short msgterm           ;print out NPX                  v5.93

exit:
        mov     dx,offset period        ; print out a period to end message
msgterm:
        mov     ah,9h
        int     21h

        cmp     dx,offset period        ; print out a period to end message v5.94
        jne     exit                    ; (if we have not already done so)

        mov     ah,4ch                  ; terminate program
        mov     al,exitcode             ;v5.94
        int     21h
start   endp

CSEG    ENDS
        end     cpuid
