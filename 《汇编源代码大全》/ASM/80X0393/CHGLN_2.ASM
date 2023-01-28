comment	*

        Author: Yousuf J. Khan

        Purpose: A TSR which will detect if a diskette has been changed,
        even if that drive's diskette change detection line is out of
        commission. Normally only used by 5.25" HD drives which have
        trouble recognizing a different density diskette.

	*

.model tiny

bds     segment at 40h

        comment {

        The BIOS Data Segment, segment 40h. All diskette related status
        bytes are listed below.

                {

        org     3Eh
disk_recal_stat db      ?       ;diskette recalibrate status
disk_motor_stat db      ?       ;diskette motor status
motor_timeout   db      ?       ;diskette motor turn-off timeout count
last_op         db      ?       ;last operation's error message
statcombytes    db      7 dup(?);status and command bytes
        org     8Bh
media_control   db      ?       ;diskette media control byte
        org     8Fh
ctrlr_info      db      ?       ;diskette controller info
drive0          db      ?       ;diskette 0 (A:) media state
drive1          db      ?       ;diskette 1 (B:) media state
last_drive0     db      ?       ;prior diskette 0 media state
last_drive1     db      ?       ;prior diskette 1 media state
cur_trak0       db      ?       ;current track, drive 0
cur_trak1       db      ?       ;current track, drive 1
        ends

.code

        comment {

        Look in the first byte of the first FCB field of PSP to get
        drive letter. This field takes any command-line argument that
        looks like a drive letter and converts it into an integer (eg.
        1=A:, 2=B:, etc.).

                {

        org     5Ch
FCB     db      ?       ;first FCB field of PSP
        org     100h

.startup
        jmp     tsr_init        ;jump to transient portion

        comment {

        Save data that is required for the resident portion of TSR here.
        Any other data not required to be resident can go into regular
        old '.data' segment.

                {

old_vect        dw      ?,?     ;address of previous level ISR
old_ax          dw      ?
old_bx          dw      ?
old_ds          dw      ?
old_es          dw      ?
old_si          dw      ?

new_int13:

        comment {

        When Int 13h called, can only assume CS:IP is saved therefore
        must manually save all other segments and registers.

                {

        ;
        ;let DS:=CS
        ;
        push    ds              ;save DS to be restored upon exit
        push    ax
        mov     ax, cs
        mov     ds, ax          ;make DS equal to CS
        pop     ax
        ;
        ;See if an access was made to our diskette drive? If not, then
        ;let it pass through.
        ;
        push    dx
        mov     dh, [fcb]
        dec     dh              ;BIOS_drive=FCB_drive-1
        cmp     dl, dh          ;is it drive we're interested in?
        pop     dx
        jne     passthru        ;not drive we're interested in
        ;
        ;See if an access to the read, write, or verify functions were
        ;made (ie. subfuncs 2 to 4)? If it was then intercept. Otherwise
        ;let it pass through.
        ;
        cmp     ah, 4
        ja      passthru        ;greater than subfnct 4
        cmp     ah, 2
        jb      passthru        ;less than subfnct 2
        ;
        ;save AX in case fnct has to be recalled
        ;
        mov     [old_ax], ax    
        ;
        ;call old INT 13h:
        ;simulate an INT by pushing FLAGS before CALLing
        ;
        pushf           
        call    dword ptr [old_vect]
        ;
        ;If no errors were found, then there is no need to redo this
        ;function. Just jump to the end.
        ;
        pushf   ;save the flag for exit from ISR
        jnc     simulated_iret

        comment {

        If an "address mark not found" error (ie. error #2) was
        returned, then modify various diskette parameters in BIOS data
        segment byte and redo function again.

                {

        cmp     ah, 2
        jne     simulated_iret  ;it wasn't an error #2
        popf                    ;pop exit ISR flag & discard
        ;
        ;ES:=BIOS Data Segment
        ;
        int     3               ;debugger breakpoint
        mov     [old_bx], bx
        mov     [old_es], es
        mov     [old_si], si
        mov     bx, BDS
        mov     es, bx
        assume  es:BDS
        ;
        ;SI+BX will be equal to either 90h or 91h, the offset within the
        ;BIOS data segment for either the drive 0 or drive 1 media
        ;state. The FCB number determines whether we want drive 0 or 1.
        ;
        xor     bx, bx  ;make sure BX is zero
        mov     si, offset ctrlr_info
        mov     bl, [fcb]
        ;
        ;To indicate an undetermined drive, set the proper media state
        ;byte to the value 2.
        ;
        mov     byte ptr es:[si][bx], 2
        mov     si, [old_si]
        mov     es, [old_es]
        mov     bx, [old_bx]
        mov     ax, [old_ax]    ;restore AX before recalling function

passthru:
        ;
        ;call old INT 13h:
        ;simulate an INT by pushing FLAGS before CALLing
        ;
        pushf           
        call    dword ptr [old_vect]
        pushf           ;save the flag for exit from ISR
simulated_iret:
;        popf            ;pop the flag for exit from ISR
;        pop     ds      ;restore original DS
;        retf    2       ;pop CS,IP, but discard saved entry flag
;
;  BINGO! this should be an iret, the old flags never get popped
;  from the stack.  On my machine with DOS 5.0 and about 20 or so
;  files on the floppy, it didnt give me any errors after 100 tries.
;  Why?  DOS resets the stack frame for each call, but for another
;  parent it will cause a problem.
;
;  when at  'retf  2'  the stack looks like this:
;    ss:[sp+00] = callers cs
;    ss:[sp+02] = callers ip
;    ss:[sp+04] = callers flags  ( flags get pushed 1st with int instruction )
;
;  What has been happening is that the flags being returned had the correct
;  return codes, and we returned to the correct cs:ip, however the stack
;  frame is off by one word on each call by not popping off the flags,
;  thus screwing with the callers stack frame.  That's why we do a pushf
;  before a far call to the original interrupt vector.  So, changing the
;  flags is okay because for this interrupt it is the return code,  however
;  we must exit in a way to perserve/restore the callers stack frame.
;
;  To do the least amount of changes to your code, try replacing with:
;
;    simulated_iret:
          pop     ds          ; pop saved flags into DS, DS is still stacked
          push    bp          ; save BP, we need it as an index into the stack
          mov     bp,sp       ; bp = stack pointer,  so  [bp+00] = old bp
          mov     [bp+08],ds  ; edit return flags, cs=[sp+04] so flags=[sp+08]
          pop     bp          ; restore original BP
          pop     ds          ; restore original DS, now [sp+00] = cs
          iret                ; int return

; I compiled this as is, and ran it in a continous loop from a batch file
; for about 30 minutes with no errors.  Now I have to go clean my floppy
; drive heads.

tsr_init:

        comment {

        This is the transient portion of the TSR. It initializes the
        resident portion by parsing the command-line, and revectoring
        the interrupts. This entire portion will disappear after
        residency.

                {

        .data
CR_LF   equ     13,10
copyright       db      "CHNGLINE (c) 1993, Yousuf J. Khan",cr_lf,"$"
installed       db      "installed",cr_lf,"$"
        .code
        ;
        ;write copyright message
        ;
        mov     ah, 9
        mov     dx, offset copyright
        int     21h
        ;
        ;get command-line
        ;
        cmp     [fcb], 0        ;any command-line argument at all?
        je      install_error   ; no arguments provided
        cmp     [fcb], 2        ;Drive A: or B:?
        ja      install_error   ; not a floppy drive
        ;
        ;redirect interrupts
        ;
	mov	ax, 3513h
	int	21h			;save old Int 13h vector
	mov	[old_vect], bx		;offset
        mov     [old_vect][2], es       ;segment
	push	ds			;save current DS
	mov	ax, cs
        mov     ds, ax                  ;DS := CS
        mov     dx, offset new_int13    ;DS:DX -> new Int 13h
	mov	ax, 2513h
	int	21h			;set new Int 13h address
        pop     ds                      ;restore DS
        ;
        ;state that TSR has been successfully installed
        ;
        mov     ah, 9
        mov     dx, offset installed
        int     21h
        ;
        ;start making resident
        ;
        mov     dx, offset tsr_init     ;# of bytes to keep res
	mov	cl, 4
	sar	dx, cl			;convert bytes to paragraphs
        inc     dx                      ;add 1
	mov	ax, 3100h
	int	21h			;term &	stay res
install_error:

        comment {

        If any errors were detected during initialization process then
        abort operation.

                {

        .data
errmsg  db      "Syntax:",CR_LF,"CHNGLINE {drive}",10,CR_LF
        db      "Where {drive} can only be either A: or B:",CR_LF,"$"
        .code
        mov     ah, 9
        mov     dx, offset errmsg
        int     21h
        mov     ax, 4C01h       ;errorlevel = 1
        int     21h             ;terminate w/o resident
        ends

        end
