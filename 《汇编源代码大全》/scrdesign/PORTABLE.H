/*============================================================================

    portable.h   v1.00      Written by Scott Robert Ladd.

    _MSC_VER        Microsoft C 6.0 and later
    _QC             Microsoft Quick C 2.51 and later
    __TURBOC__      Borland Turbo C and Turbo C++
    __ZTC__         Zortech C and C++
    __WATCOM__      WATCOM C
============================================================================*/


/* prevent multiple inclusions of this header file */

#if !defined(PORTABLE_H)
#define PORTABLE_H

/*----------------------------------------------------------------------------
    Pointer-related macros

    MK_FP   creates a far pointer from segment and offset values
----------------------------------------------------------------------------*/

#if !defined(MK_FP)
    #define MK_FP(seg,off) ((void far *)(((long)(seg) << 16)|(unsigned)(off)))
#endif

/*----------------------------------------------------------------------------
    Directory search macros and data structures

    DOSFileData         MS-DOS file data structure
    FIND_FIRST          MS-DOS function 0x4E -- find first file matchine spec
    FIND_NEXT           MS-DOS function 0x4F -- find subsequent files
----------------------------------------------------------------------------*/

/* make sure the structure is packed on byte boundary */

#if defined(_MSC_VER) || defined(_QC) || defined(__WATCOM__)
    #pragma pack(1)
#elif defined(__ZTC__)
    #pragma ZTC align 1
#elif defined(__TURBOC__)
    #pragma option -a-
#endif

/* use this structure in place of compiler-defined file structure */

typedef struct {
      char        reserved[21];
      char        attrib;
      unsigned    time;
      unsigned    date;
      long        size;
      char        name[13];
      } DOSFileData;

/* set structure alignment to default */

#if defined (_MSC_VER) || defined(_QC) || defined(__WATCOMC__)
 #pragma pack()
#elif defined (__ZTC__)
 #pragma ZTC align
#elif defined (__TURBOC__)
 #pragma option -a.
#endif

/* include proper header files and create macros */

#if defined (_MSC_VER) || defined(_QC) || defined(__WATCOMC)
 #include "direct.h"
 #define FIND_FIRST(spec,attr,buf) _dos_findfirst(spec,attr,\
       (struct find_t *)buf)
 #define FIND_NEXT(buf) _dos_findnext((struct find_t *)buf)
#elif defined (__TURBOC__)
 #include "dir.h"
 #define FIND_FIRST(spec,attr,buf) findfirst(spec,(struct ffblk *)buf,attr)
 #define FIND_NEXT(buf) findnext((struct ffblk *)buf)
#elif defined (__ZTC__)
 #include "dos.h"
 #define FIND_FIRST(spec,attr,buf) dos_findfirst(spec,attr,\
       (struct DOS_FIND *)buf)
 #define FIND_NEXT(buf) dos_findnext((struct DOS_FIND *)buf)
#endif

/*----------------------------------------------------------------------------
    I/O Port Macros

    IN_PORT     read byte from I/O port
    IN_PORTW    read word from I/O port
    OUT_PORT    write byte to I/O port
    OUT_PORTW   write word to I/O port
----------------------------------------------------------------------------*/

#if defined(__TURBOC__)
 #include "dos.h"
 #define IN_PORT(port)           inportb(port)
 #define IN_PORTW(port)          inport(port)
 #define OUT_PORT(port, val)     outportb(port, val)
 #define OUT_PORTW(port, val)    outport(port, val)
#else
 #include "conio.h"

 #define IN_PORT(port)           inp(port)
 #define IN_PORTW(port)          inpw(port)
 #define OUT_PORT(port, val)     outp(port, val)
 #define OUT_PORTW(port, val)    outpw(port, val)

/*----------------------------------------------------------------------------
    Borland pseudo register macros

    These macros replace references to Borland's pseudo register
    variables and geninterrup() funciton with traditional struct
    REGS/int86 references.
----------------------------------------------------------------------------*/

#if !defined(__TURBOC__)
 #include "dos.h"

 union REGS CPURegs;

 #define _AX CPURegs.x.ax
 #define _BX CPURegs.x.bx
 #define _CX CPURegs.x.cx
 #define _DX CPURegs.x.dx

 #define _AH CPURegs.x.ah
 #define _AL CPURegs.x.al
 #define _BH CPURegs.x.bh
 #define _BL CPURegs.x.bl
 #define _CH CPURegs.x.ch
 #define _CL CPURegs.x.cl
 #define _DH CPURegs.x.dh
 #define _DL CPURegs.x.dl

 #define geninterrupt(n) int86(n,&CPURegs,&CPURegs);
 #define O_DENYALL   0x10
 #define O_DENYWRITE 0x20
 #define O_DENYREAD  0x30
 #define O_DENYNONE  0x40
#endif

#endif
#endif
