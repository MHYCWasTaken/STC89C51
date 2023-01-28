comment *

        Author:
        Yousuf J. Khan

        Purpose:
        To detect what type of video adapter is installed.

        Note:
        Work was based on source code inside Richard Wilton's
        "Programmer's Guide to PC & PS/2 Video Systems" book. However
        it's been heavily reworked to a more (dare I say?)
        understandable format. Also Super VGA tests were put in,
        something completely missing from Wilton's book.

        *

.model tiny

;.stack 200h

.data
std0type        db      0       ;first video subsystem type
ext0type        db      0       ;extended info on first subsystem
std1type        db      0       ;second video subsystem type
ext1type        db      0       ;extended info on second subsystem
vidflag         record  monofound:1,colorfound:1
egatype         vidflag <0,0>   ;Was a mono or color EGA found?
;
;video adapter codes (based on codes returned by VGA BIOS DC codes):
;-these codes are used when a VGA BIOS is not found and other tests
; need to be performed to determine video type
;-it was decided to use the VGA BIOS DC codes for simplicity
;-see findVGA procedure for list of VGA DCCs.
;
NoDISP  equ     0       ;no display
MONO    equ     1
CGA     equ     2
cEGA    equ     4       ;color EGA
mEGA    equ     5       ;mono EGA

.code
        org     100h            ;*.COM start offset
start:
        call    findVGA         ;look for a VGA or MCGA
        comment \

        After executing VGA BIOS routines, the VGA BIOS itself will have
        done all the work of determining what is installed so no need to
        do any further tests. The "findVGA" subprocedure will set the CF
        upon returning, if a VGA BIOS was found.

                \
        jc      endtests        ;found VGA/MCGA, end of tests
        call    findEGA         ;look for an EGA
        call    findCGA         ;look for a CGA
        call    findMono        ;look for an MDA or HGC
endtests:
        call    write_msg       ;write found info to screen, set
                                ; errorlevel
        mov     ah, 4ch         ;end program
        int     21h

findVGA         proc
comment *

        Purpose:
        determine if a MCGA/VGA/SVGA display is installed

        Entry:
        nothing

        Return:
        CF set if found, clear if not

        *
        mov     ax, 1A00h       ;get Displ Combo Code info
        int     10h
        cmp     al, 1Ah         ;VGA-only BIOS function
        jne     noVGA           ; not supported => no VGA
comment \

        Upon Return from Int 10h, AX=1A00h:
        BL=primary adapter DCC
        BH=secondary adapter DCC

        Values for display combination code:
        00h no display
        01h monochrome adapter w/ monochrome display
        02h CGA w/ color display
        03h reserved
        04h EGA w/ color display
        05h EGA w/ monochrome display
        06h PGA w/ color display
        07h VGA w/ monochrome analog display
        08h VGA w/ color analog display
        09h reserved
        0Ah MCGA w/ digital color display
        0Bh MCGA w/ monochrome analog display
        0Ch MCGA w/ color analog display
        FFh unknown display type

        \
        push    bx              ;interpretDCC destroys contents of BH
        call    interpretDCC    ;this proc looks for extended info
        mov     [std0type], bl  ;save normal info
        mov     [ext0type], bh  ;save extended info
        pop     bx
        push    bx
        ;Since proc interpretDCC only works on info in BL, we must
        ; exchange BL with BH.
        xchg    bh, bl          
        call    interpretDCC
        mov     [std1type], bl  ;save normal info
        mov     [ext1type], bh  ;save extended info
        pop     bx
        stc             ;found VGA/MCGA, so set CF before ret
        jmp     endVGAtest
noVGA:
        clc
endVGAtest:
        ret

        interpretDCC    proc
        comment *

                Purpose:
                Interprets information returned by the DCC

                Upon Entry:
                BL=normal DCC of desired video subsystem
                BH=will be destroyed, replaced with ext code

                Upon Return:
                BL=normal DCC returned (MDA,CGA,EGA,PGA,MCGA,VGA,etc)
                BH=extended code (HGC,SVGA), 0 if none

                *
                mov     bh, 0   ;Assume no extended attribs
                cmp     bl, 0   ;If 0, then no display card
                je      end_interpret
                cmp     bl, 1   ;Mono
                je      monotests
                jmp     CGAtests
        monotests:
        ;
        ;If we find a monochrome adapter, then we need to do special
        ; further tests to see if we have just an MDA or one of the
        ; Hercules cards.
        ;
                mov     bh, bl          ;save normal DCC in BH
                call    findHGC         ;MDA or HGC? value returned in
                                        ; BL
                xchg    bh, bl          ;BL=normal,BH=extended
                jmp     end_interpret
        CGAtests:
                cmp     bl, 2           ;CGA
                je      end_interpret
                cmp     bl, 6           ;EGA or PGA
                jbe     end_interpret
                cmp     bl, 8           ;BL=7,8: VGA
                ja      end_interpret
        ;
        ;If we find a VGA, then we need to do extra tests to see whose
        ; VGA chipset it is.
        ;
                mov     bh, bl          ;save BL in BH
                call    findSVGA        ;search for SVGA adapter
                xchg    bh, bl          ;BH=extended, BL=normal
        end_interpret:
                ret
                endp
        endp

findSVGA        proc
comment *

        Purpose:
        To search for specific SVGA types, including VESA compatibility.

        Entry:
        Nothing

        Return:
        BL=SVGA type

        *
comment \

        List of SVGA types:
        Brand           Type
        -----           ----
        ATI             1Ä¿
        Ahead           2 ³ These chipsets all have
        Paradise        3 ³ easily identifiable BIOS based
        Oak             4 ³ identification methods.
        Genoa           5ÄÙ
        Cirrus          6Ä¿
        Chips&Tech      7 ³ These chipsets require complicated
        Tseng           8 ³ register twiddlings to identify
        ZyMos           9 ³ their presense.
        Trident        10ÄÙ
        8514/A         11   -checks for HDILOAD.EXE only

        If a VESA BIOS is present, then add 128 to errorlevels.

        Note: The BIOS based id strings were obtained from Ralf Brown's
        PD DOS Interrupts list (INTER32).

        The register level chipset tests were obtained through various
        sources, including John Bridges' VGAKIT 5.0, and Andrew
        Rossmann's INFOPLUS 1.55 source code. However, both gentlemen
        appear to have gotten their info through the book "Advanced
        Programmer's Guide to Super VGAs", by George Sutty and Steve
        Blair. So lay credit wherever you like.

        \
        noSVGA  equ     0
        mov     bl, noSVGA      ;initially assume no SVGA
        push    es              ;save current ES segment
        mov     ax, 0C000h      ;set ES to VBIOS
        mov     es, ax          ; segment (C000h)
        ;
        ;now testing for ATI chipset
        ;
        .data
        ATIid   db      "761295520"
        idlen   =       $-ATIid
        ATI     equ     1
        .code
        mov     di, 31h         ;id string offset in ATI VBIOS
        mov     si, offset ATIid
        mov     cx, idlen
        repe    cmpsb   ;comp each byte, exit if not matched
        jne     noATI
        mov     bl, ATI         ;ATI was found
        jmp     endSVGAtests
noATI:
        ;
        ;now testing for Ahead chipset
        ;
        .data
        AHEADid db      "AHEAD"
        idlen   =       $-AHEADid
        AHEAD   equ     2
        .code
        mov     di, 25h ;id string offset in AHEAD VBIOS
        mov     si, offset AHEADid
        mov     cx, idlen
        repe    cmpsb   ;comp each byte, exit if not matched
        jne     noAHEAD
        mov     bl, AHEAD       ;AHEAD was found
        jmp     endSVGAtests
noAHEAD:
        ;
        ;now testing for Paradise/WD chipset
        ;
        .data
        PDISEid db      "VGA="
        idlen   =       $-PDISEid
        PDISE   equ     3
        .code
        mov     di, 7dh         ;id string offset in Pdise VBIOS
        mov     si, offset PDISEid
        mov     cx, idlen
        repe    cmpsb
        jne     noPDISE
        mov     bl, PDISE       ;Paradise was found
        jmp     endSVGAtests
noPDISE:
        ;
        ;now testing for OAK chipset
        ;
        .data
        OAKid   db      "OAK VGA"
        idlen   =       $-PDISEid
        OAK     equ     4
        .code
        mov     di, 8           ;id string offset in OAK VBIOS
        mov     si, offset OAKid
        mov     cx, idlen
        repe    cmpsb
        jne     noOAK
        mov     bl, OAK         ;OAK was found
        jmp     endSVGAtests
noOAK:
        ;
        ;now testing for Genoa chipset
        ;
        ;the installation check for Genoa video adapters is the
        ;signature 77h XXh 99h 66h at C000h:0037h, where XXh is
        ;        00h for Genoa 6200/6300
        ;        11h for Genoa 6400/6600
        ;        22h for Genoa 6100
        ;        33h for Genoa 5100/5200
        ;        55h for Genoa 5300/5400
        ;
        .data
        GENOAid db      77h,00h,99h,66h
        idlen   =       $-GENOAid
        GENOA   equ     5
        .code
        mov     di, 37h         ;id string offset in OAK VBIOS
        mov     si, offset GENOAid
        mov     cx, idlen
        repe    cmpsb
        pushf                   ;save flags
        cmp     cx, idlen-2     ;did mismatch occur after 2nd byte?
        je      contGENOA       ;yes? continue test at next byte
        popf                    ;restore flags
        jmp     discontGENOA
contGENOA:
        popf                    ;restore flags
        repe    cmpsb           ;continue tests where it left off
discontGENOA:
        jne     noGENOA
        mov     bl, GENOA
        jmp     endSVGAtests
noGENOA:
        ;
        ;The following chipsets require register tests, and cannot be
        ; identified through the BIOS:
        ;
        ;test for CIRRUS chipset (register)
        ;
        call    CIRRUStest
        cmp     bl, 0           ;if BL=0, Cirrus wasn't found
        jne     endSVGAtests
        ;
        ;test for ZyMos chipset (register)
        ;
        call    ZYMOStest
        cmp     bl, 0           ;C&T was not found
        jne     endSVGAtests
        ;
        ;test for TSENG chipset (register)
        ;
        call    TSENGtest
        cmp     bl, 0           ;Tseng was not found
        jne     endSVGAtests
        ;
        ;test for Trident chipset (register)
        ;
        call    TridentTest
        cmp     bl, 0           ;Trident wasn't found
        jne     endSVGAtests
        call    Test8514
        cmp     bl, 0
        jne     endSVGAtests    ;8514/A wasn't found
        ;
        ;test for C&T chipset (register)
        ;
        ;Note: the C&T test routine has been moved to the end, because
        ; it seems to cause the most lockups.
        ;
        call    CHIPStest
endSVGAtests:
        ;
        ;test to see if VESA BIOS is installed
        ;
        .data
        VESAbuf db      256 dup(?)      ;VESA information buffer
        .code
        pop     es
        mov     ax, 4f00h
        mov     di, offset VESAbuf
        int     10h             ;VESA: get SVGA info
        cmp     ax, 004fh       ;if VESA installed this should work
        jne     noVESA
        ;
        ; If VESABIT of the record, SVGATYPE, is set, then a VESA
        ; BIOS was found. The rest of the record just records what
        ; type of SVGA was found, regardless of VESA.
        ;
        svgatype        record  vesabit:1,svgabits:7

        or      bl, mask vesabit        ;turn on VESAbit, in ext code
noVESA:
        ret

        CIRRUStest      proc
        comment *

                Purpose:
                Register-level test for CIRRUS chipset

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code
                *
                CIRRUS  equ     6
                push    bx      ;save BH
                mov     dx, 3B4h;assume DX=mono VGA selection port
                cmp     bh, 8   ;if BH=8, color; BH=7, mono
                je      color_cirrus
                add     dx, 020h;if color, add 20h to select port addr
        color_cirrus:
                mov     al, 0Ch ;Start Addr High register
                out     dx, al
                inc     dx      ;DX=data port
                in      al, dx  ;read register
                mov     ah, al  ;save
                xchg    al, ah  ;swap back
                push    ax      ;save port value
                push    dx      ;save port addr
                xor     al, al
                out     dx, al  ;zero Start Addr High reg
                dec     dx      ;back to selection port
                mov     al, 1Fh
                out     dx, al  ;select special Cirrus ID reg
                inc     dx      ;back to data port
                in      al, dx  ;read ID reg value
                mov     ah, al  ;save it in AH
                mov     bh, al  ;save again in BH
                mov     dx, 3c4h;addr of special seq'ncr/ext'ns select port
                mov     al, 6   ;extensions control reg
                mov     cl, 4   ;get ready to swap the nibbles in AX
                ror     bh, cl  ;nibble swap computes ext reg disable val
                mov     ax, bx
                out     dx, ax  ;disable extensions
                inc     dx      ;seq/ext data port
                in      al, dx  ;read back enable flag
                or      al, al  ;disabled?
                jnz     noCIRRUS
                dec     dx      ;seq/ext select port
                mov     bh, al  ;save AL
                mov     al, 6   ;extensions control reg
                out     dx, al
                inc     dx
                mov     al, ah  ;restore ID reg value
                out     dx, al  ;reenable ext regs
                in      al, dx  ;read back ext reg
                cmp     al, 1   ;enabled?
                jne     noCIRRUS
                ror     bh, cl  ;nibble swap to compute enable ext reg val
                dec     dx
                mov     ax, bx
                out     dx, ax  ;ext enable
                inc     dx
                in      al, dx
                cmp     al, 1
                jne     noCIRRUS
                mov     bl, CIRRUS      ;CIRRUS was found
        noCIRRUS:
                pop     dx
                dec     dx      ;point select reg
                pop     ax
                out     dx, ax  ;restore to orig state
                pop     ax      ;restore BH
                mov     bh, ah
                ret
                endp

        CHIPStest       proc
        comment *

                Purpose:
                Register-level test for C&T VGA chipset

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code

                Note:
                This particular test appears to lockup my system, with
                an ATI VGAWonder card installed, after the first port
                write. I have no idea if this lockup is due to the
                presence of the ATI or some other component in my
                motherboard. So be forewarned: always do the ATI test
                first, which is safely BIOS-based, before you do this
                test. In fact you might want to save this test for the
                last of all the tests you do.

                *

        CHIPS   equ     7
        ;place VGA in setup mode
                cli
                mov     dx,46e8h        ;address of setup control reg
                in      al,dx
                or      al,10h          ;turn on setup bit
                out     dx,al           ;go to setup mode
        ;enable extended register bank
                mov     dx,103h         ;extended reg address
                in      al,dx
                or      al,80h          ;turn enable bit on
                out     dx,al
        ;read global ID
                mov     dx,104h         ;global ID reg
                in      al,dx
                mov     ah,al           ;save
        ;place vga in normal mode
                mov     dx,46e8h
                in      al,dx
                and     al,03fh
                out     dx,al
                sti
        ;read version extended register
                mov     dx,3d6h
                mov     al,0
                out     dx,al           ;select version register
                inc     dx
                in      al,dx
                cmp     ah,5ah          ;check for CTI ID
                jne     notcti
                and     al,0f0h         ;adjust chip id
                shr     al,1
                shr     al,1
                shr     al,1
                shr     al,1
                cmp     al,2            ;only 0, 1 and 3 are good
                je      notcti
                cmp     al,4
                jge     notcti
                cmp     al,3
                je      notcti
                mov     bl, CHIPS       ;return SVGA info
        notcti:
                ret
                endp

        TSENGtest       proc
        comment *

                Purpose:
                Register-level test for Tseng VGA chipset

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code
                *
        TSENG   equ     8
                mov     dx,3cdh         ;page select reg
                in      al,dx
                mov     ah,al           ;save
                and     al,0c0h         ;save some bits
                or      al,55h          ;test value one
                out     dx,al           ;write it
                in      al,dx
                cmp     al,55h          ;same?
                jne     nottseng
                mov     al,0aah         ;test value two
                out     dx,al
                in      al,dx
                cmp     al,0aah         ;same
                jne     nottseng
                mov     al,ah           ;restore original settings
                out     dx,al
                mov     al,1
        end_tsengck:
                mov     bl, TSENG       ;found a TSENG
        nottseng:
                ret
                endp

        ZYMOStest       proc
        comment *

                Purpose:
                Register-level test for ZyMos VGA chipset

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code

                *
        ZyMos   equ     9
                mov     dx,3c4h         ;extended reg bank
                mov     al,0bh          ;version reg
                out     dx,al
                inc     dx
                in      al,dx           ;get version
                and     al,0fh
                cmp     al,2
                jne     end_zymosck     ;not found
                mov     bl, ZyMos       ;found one
        end_zymosck:
                ret
                endp

        TridentTest     proc
        comment *

                Purpose:
                Register level test for Trident VGA chipset

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code

                Note:
                -Thanks to John Olson, of Fidonet 1:106/888, of the
                Fidonet 80XXX assembly language echo, who supplied this
                routine from "Programmer's Guide to the EGA and VGA
                Cards", by Richard F. Ferraro.

                *
                Trident equ     10
                MOV     DX,3C4H
                MOV     AL,0EH          ; Read mode control #1 Register
                OUT     DX,AL
                INC     DX
                IN      AL,DX           ; Read old value
                XOR     AL,2
                PUSH    AX
                MOV     AL,0            ; Write new value bit 1=0, all
                                        ;other bits =0
                OUT     DX,AL
                IN      AL,DX           ; Read new value
                AND     AL,0FH
                CMP     AL,2            ; Check for Trident
                POP     AX              ; Old value
                OUT     DX,AL           ; Restore old value
                JNZ     NOT_T
                mov     bl, Trident     ;found one
        not_t:
                ret
                endp

        Test8514        proc
        comment *

                Purpose:
                Installation check for 8514/A's HDILOAD.EXE

                Entry:
                BH=current normal DCC

                Return:
                BL=SVGA code
                AX,CX,DX destroyed

                *
                _8514   equ     11
                ;
                ;If HDILOAD installed then CX and/or DX will be modified
                ; as CX:DX->function entry point array
                ;
                mov     ax, 105h
                mov     cx, 0
                mov     dx, 0
                int     7Fh             ;installation check
                cmp     cx, 0           ;if CX didn't change
                je      no8514
                mov     bl, _8514
        no8514:
                ret
                endp

        endp

findEGA proc
comment *

        Purpose:
        To find an EGA

        *
        mov     ah, 12h
        mov     bx, 0FF10h
        int     10h             ;for EGA BIOS+ only
        cmp     bh, 0FFh        ;if BH remains unchanged
        je      endEGAtest      ; then EGA BIOS not installed
        cmp     bh, 0           ;BH=0, if color EGA
        jne     EGAmono
        mov     [egatype], mask colorfound
        mov     bl, cEGA        ;EGA w/ color disp
        mov     dx, 3D4h        ;color port addr
        call    determine_subsystem
        jmp     endEGAtest
EGAmono:
        mov     [egatype], mask monofound
        mov     bl, mEGA        ;EGA w/ mono disp
        mov     dx, 3B4h        ;mono port addr
        call    determine_subsystem
endEGAtest:
        ret
        endp

findCGA         proc
comment *

        Purpose:
        To find a CGA

        *
        test    [egatype], mask colorfound      ;is color EGA present?
        jne     endCGAtest      ;yes, then skip test
        mov     dx, 3D4h        ;DX=CGA 6845 CRTC port addr
        call    find6845
        jc      endCGAtest      ;didn't find CGA
        mov     bl, CGA         ;ie. found CGA
        call    determine_subsystem
endCGAtest:
        ret
        endp

findMono        proc
comment *

        Purpose:
        To find a Mono adapter

        *
        test    [egatype], mask monofound       ;is mono EGA present?
        jne     EndMonotest     ;yes, then skip test
        mov     dx, 3B4h        ;DX=Mono 6845 CRTC port addr
        call    find6845
        jc      EndMonoTest     ;didn't find MDA
        ;
        ;We've found a mono adapter, now it's time to find out
        ; if it's a Hercules-compatible monographics adapter.
        ;
        mov     bl, MONO        ;found MDA
        call    determine_subsystem
        jc      mono_is_2nd     ;MDA was 2nd system
        call    findHGC         ;is 1st a Herc?
        mov     [ext0type], bl
        jmp     EndMonoTest
mono_is_2nd:
        call    findHGC         ;is 2nd a Herc?
        mov     [ext1type], bl
EndMonoTest:
        ret
        endp

findHGC proc
comment *

        Purpose:
        To determine if MDA found is also a Herc of some sort

        Enter:
        nothing

        Return:
        BL=Herc type

        *
        NoHerc  equ     0       ;no Hercules
        Herc    equ     1       ;regular Hercules
        HercP   equ     2       ;Hercules Plus
        HercIC  equ     3       ;Hercules InColor
        comment \
                Hercs are identified from normal MDA by bit 7, vertical
                sync, on 6845 Status Port which is changeable only on
                Hercs.

                Hercs are identified from each other with bits 4-6 in
                Status Port. On an regular Herc, they are 000b, on
                Herc+, they are 001b, and on InColor, they are 101b.

                Each bit is kept track of in the record, HGCStatus.
                \
        HGCStatus       record  vsync:1,HGCid:3,miscbits:4
        mov     dx, 3BAh        ;status port
        in      al, dx
        and     al, mask vsync
        mov     ah, al          ;store current vsync status in AH
        mov     cx, 8000h       ;loop 32768 times
teststatusbit:
        in      al, dx
        and     al, mask vsync
        cmp     ah, al
        loope   teststatusbit   ;keep looping as long as they are equal
        jne     HercIDs         ;if vsync changed, is a Herc
        mov     bl, NoHerc      ;was not a Herc
        jmp     EndHercTests
HercIDs:
        in      al, dx          ;read status port again
        and     al, mask HGCid  ;mask all but bits 4-6
        cmp     al, HGCStatus <0,001b,0>        ;indicates HGC+
        je      isPlus
        mov     bl, Herc        ;is a regular Herc, for now
        cmp     al, HGCStatus <0,101b,0>        ;indicates InColor
        jne     EndHercTests    ;didn't find InColor
        mov     bl, HercIC      ;is an InColor
        jmp     EndHercTests
isPlus:
        mov     bl, HercP       ;is a Herc Plus (Ramfonts)
EndHercTests:
        ret
        endp

determine_subsystem     proc
comment *
        Purpose:
        To determine if currently tested subsystem is primary or secondary

        Entry:
        BL=subsystem DCC value to input in memory
        DX=port address to compare against

        Return:
        CF clear if primary system, set if secondary
        *
        push    ax              ;save AX
        push    es              ;save ES
        mov     ax, 40h         ;set ES to
        mov     es, ax          ; BIOS data segment
        mov     ax, es:[63h]    ;AX=active Vid CRTC base addr
        cmp     ax, dx          ;is current = default?
        pop     es              ;restore ES
        pop     ax              ;restore AX
        jne     _2ndsystem      ;if not default, must be 2nd subsystem
        mov     [std0type], bl  ;store as 1st system
        clc                     ;is primary, so clear CF
        jmp     end_det_sys
_2ndsystem:
        mov     [std1type], bl  ;store as 2nd system
        stc                     ;is secondary, so set CF
end_det_sys:
        ret
        endp

find6845        proc
comment *
        Purpose:
        Detect if a 6845-compatible CRTC controller is present at port
        address given in DX

        Entry:
        DX=CRTC base port address to test

        Return:
        CF clear if successful, set if not
        *
        push    dx
        push    ax
        mov     al, 0Fh
        out     dx, al  ;select 6845 reg 0Fh (cursor low)
        inc     dx
        in      al, dx  ;AL=current cursor low val
        mov     ah, al  ;save AL in AH
        mov     al, 66h ;AL=set some arbitrary val
        out     dx, al  ;write to 6845
        mov     cx, 100h
        loop    $       ;loop 100h times
        in      al, dx
        xchg    ah, al  ;AH=returned value
                        ;AL=original value
        out     dx, al  ;restore original value
        cmp     ah, 66h ;did 6845 respond?
        je      end6845
        stc             ;set CF if 6845 not there
end6845:
        pop     ax
        pop     dx
        ret
        endp

write_msg       proc
comment *

        Purpose:
        To display video adapter information to Stdout, and return an
        errorlevel based on command line parameters.

        *
.data
CRLF    equ     13,10,"$"
copyright       db      "VIDTYPE v1.1 (c) 1992, Yousuf J. Khan",CRLF
std_msg db      "Standard video information:",CRLF
ext_msg db      "Extended video information:",CRLF
vid1msg db      "Primary video subsystem:",CRLF
vid2msg db      "Secondary video subsystem:",CRLF
;Standard video adapter types (based on VGA BIOS DCCs)
DCC00   db      "no display",CRLF
DCC01   db      "monochrome adapter w/ monochrome display",CRLF
DCC02   db      "CGA w/ color display",CRLF
DCC03   db      "reserved",CRLF
DCC04   db      "EGA w/ color display",CRLF
DCC05   db      "EGA w/ monochrome display",CRLF
DCC06   db      "PGA w/ color display",CRLF
DCC07   db      "VGA w/ monochrome analog display",CRLF
DCC08   db      "VGA w/ color analog display",CRLF
DCC09   db      "reserved",CRLF
DCC0A   db      "MCGA w/ digital color display",CRLF
DCC0B   db      "MCGA w/ monochrome analog display",CRLF
DCC0C   db      "MCGA w/ color analog display",CRLF
DCCFF   db      "unknown display type",CRLF
stdmsgtbl       dw      DCC00
                dw      DCC01
                dw      DCC02
                dw      DCC03
                dw      DCC04
                dw      DCC05
                dw      DCC06
                dw      DCC07
                dw      DCC08
                dw      DCC09
                dw      DCC0A
                dw      DCC0B
                dw      DCC0C
                dw      DCCFF
;Extended video adapter types (various Hercs & Super VGAs)
HG      equ     "Hercules "
SV      equ     "Super VGA: "
EXT00   db      "no extended info available",CRLF
HGC00   db      HG,"standard",CRLF
HGC01   db      HG,"Plus (Ramfonts)",CRLF
HGC02   db      HG,"InColor",CRLF
HGCmsgtbl       dw      EXT00   ;no Herc
                dw      HGC00
                dw      HGC01
                dw      HGC02
VGA00   db      SV,"ATI",CRLF
VGA01   db      SV,"AHEAD",CRLF
VGA02   db      SV,"Paradise",CRLF
VGA03   db      SV,"Oak",CRLF
VGA04   db      SV,"Genoa",CRLF
VGA05   db      SV,"Cirrus",CRLF
VGA06   db      SV,"Chips & Tech",CRLF
VGA07   db      SV,"TSENG",CRLF
VGA08   db      SV,"ZyMos",CRLF
VGA09   db      SV,"Trident TVGA",CRLF
VGA10   db      SV,"8514/A",CRLF
VGAmsgtbl       dw      EXT00   ;no SVGA
                dw      VGA00
                dw      VGA01
                dw      VGA02
                dw      VGA03
                dw      VGA04
                dw      VGA05
                dw      VGA06
                dw      VGA07
                dw      VGA08
                dw      VGA09
                dw      VGA10
spaces  db      "    $"   ;three space indentation
VESAmsg db      "VESA BIOS present",CRLF

.code

DOSMSG  macro   msg_offset
        mov     ah, 9
        mov     dx, offset msg_offset
        int     21h
        endm

TYPEmsg macro   base, index
        xor     bx, bx
        add     bl, [index]
        ;
        ;mask out VESA bit in "svgatype" record
        ; both forms of the AND below are the same
        ;
;        and     bl, mask svgabits      ;mask bottom 7 bits, or
        and     bl, not mask vesabit    ; don't mask 8th bit
        mov     si, bx
        shl     si, 1                   ;mult offsets by 2
        mov     bx, offset base
        mov     dx, [bx][si]
        mov     ah, 9
        int     21h
        endm

INDENT  macro   num
        ;
        ;puts indentations in text printed to screen
        ;
        rept    num
                mov     dx, offset spaces
                mov     ah, 9
                int     21h
                endm
        endm

        DOSMSG  copyright
        INDENT  1
        DOSMSG  vid1msg
        INDENT  2
        DOSMSG  std_msg
        INDENT  3
        TYPEmsg stdmsgtbl, std0type
        INDENT  2
        DOSMSG  ext_msg
        INDENT  3
        cmp     [std0type], 1           ;is it MDA?
        je      extmono1                ;yes, it's MDA
        TYPEmsg vgamsgtbl, ext0type
        mov     bl, ext0type
        and     bl, mask vesabit        ;was VESA bit set?
        je      _2nd_vid_adapter        ;no VESA
        INDENT  3
        DOSMSG  VESAmsg                 ;display VESA message
        jmp     _2nd_vid_adapter
extmono1:
        TYPEmsg hgcmsgtbl, ext0type
_2nd_vid_adapter:
        INDENT  1
        DOSMSG  vid2msg
        INDENT  2
        DOSMSG  std_msg
        INDENT  3
        TYPEmsg stdmsgtbl, std1type
        INDENT  2
        DOSMSG  ext_msg
        INDENT  3
        cmp     [std1type], 1           ;is it MDA or VGA?
        je      extmono2                ;it's MDA
        TYPEmsg vgamsgtbl, ext1type
        mov     bl, ext1type
        and     bl, mask vesabit        ;was VESA bit set?
        je      end_display             ;no VESA
        INDENT  3
        DOSMSG  VESAmsg                 ;display VESA message
        jmp     end_display
extmono2:
        TYPEmsg hgcmsgtbl, ext1type
end_display:
        call    return_errorlevel;return errorlevel
;        mov     al, [std0type]  ;return errorlevel
        ret
        endp

return_errorlevel       proc
comment *

        Purpose:
        To scan command-line for errorlevel return parameters

        eg: vidtype
            vidtype e
            vidtype 2
            vidtype e2
            vidtype 2E

                Command-line parameters:
                none    errorlevel based on prim video subsys
                "e"     errorlevel based on prim ext video code
                "2"     errorlevel based on sec video subsys
                both    errorlevel based on sec ext video code
        *
        .data
        cmdln_params    record  secondary:1, extended:1
        cmdflags        cmdln_params    <0,0>
        .code
        ;
        ;start parsing the command line
        ;
lookfor macro   char
        mov     al, char
        mov     di, 81h
        repne   scasb
        endm

        mov     si, 80h ;point to command-line in PSP
        lodsb           ;get length of cmd-line in AL
        ;
        ;looking for little "e"
        ;
        push    ds
        pop     es      ;ES=DS
        xor     cx, cx
        mov     cl, al  ;number of interations
        push    cx      ;save CX
        lookfor "e"
        jne     SeekBigE
        or      [cmdflags], mask extended       ;set the flag
        jmp     SeekTwo ;start looking for "2"
SeekBigE:
        ;
        ;looking for big "E"
        ;
        pop     cx      ;restore CX
        push    cx      ;save it again
        lookfor "E"
        jne     SeekTwo
        or      [cmdflags], mask extended       ;set the flag
SeekTwo:
        ;
        ;looking for "2"
        ;
        pop     cx      ;restore CX
        lookfor "2"
        jne     send_err00
        or      [cmdflags], mask secondary      ;set the flag
send_err00:
        cmp     [cmdflags], cmdln_params <0,0>  ;neither flag set
        jne     send_err01
        mov     al, [std0type]
        jmp     endret_err
send_err01:
        cmp     [cmdflags], cmdln_params <0,1>  ;ext flag set
        jne     send_err10
        mov     al, [ext0type]
        jmp     endret_err
send_err10:
        cmp     [cmdflags], cmdln_params <1,0>  ;sec flag set
        jne     send_err11
        mov     al, [std1type]
        jmp     endret_err
send_err11:     ;both ext and sec flags set
        mov     al, [ext1type]
endret_err:
        ret
        endp

        end     start
