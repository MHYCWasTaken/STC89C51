/*
 *   X00API.C: X00 FOSSIL driver
 *   
 *   Created by R.F. Pels.
 *   modified by Bob Stout
 *   Placed in the public domain.
 */

/* FOSSIL call codes */

#define SET_BAUDRATE         0x0000
#define TX_CHAR              0x0100
#define RX_CHAR              0x0200
#define STATUS               0x0300
#define INITIALIZE           0x0400
#define DEINITIALIZE         0x0500
#define RAISE_DTR            0x0600
#define LOWER_DTR            0x0601
#define GET_SYS_INFO         0x0700
#define FLUSH_OUTPUT         0x0800
#define PURGE_OUTPUT         0x0900
#define PURGE_INPUT          0x0A00
#define TX_CHAR_NOWAIT       0x0B00
#define PEEK_AHEAD_INPUT     0x0C00
#define PEEK_AHEAD_KBD       0x0D00
#define READ_KBD             0x0E00
#define FLOW_CONTROL         0x0F00
#define CTLC_CTLK_CHECK      0x1000
#define SET_CUP              0x1100
#define GET_CUP              0x1200
#define WRITE_ANSI_CHAR      0x1300
#define ENABLE_WATCHDOG      0x1401
#define DISABLE_WATCHDOG     0x1400
#define WRITE_BIOS_CHAR      0x1500
#define INSERT_TICK_FUNC     0x1601
#define DELETE_TICK_FUNC     0x1600
#define BOOT_MACHINE         0x1700
#define READ_BLOCK           0x1800
#define WRITE_BLOCK          0x1900
#define START_BREAK_SIGNAL   0x1A01
#define STOP_BREAK_SIGNAL    0x1A00
#define GET_DRIVER_INFO      0x1B00
#define INSTALL_APPENDAGE    0x7e00
#define REMOVE_APPENDAGE     0x7f00


#include "x00api.h"
#include <dos.h>

static union  REGS  x00regs;
static struct SREGS x00sregs;
int                 x00error = 0;

#if __cplusplus
 extern "C" {
#endif

#ifdef __TURBOC__
 #define PEEK(s,o) peek(s,o)
#else /* MSC or ZTC */
 #define PEEK(s,o) *((unsigned _far *)(MK_FP((s),(o))))
#endif

#ifndef MK_FP
 #define MK_FP(seg,offset) \
        ((void _far *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))
#endif

unsigned int  x00_detect(void)
{
        unsigned int segofx00;
        unsigned int ofsofx00;
        
        /* Peek in interrupt vector table for start of FOSSIL */

        ofsofx00 = PEEK(0, 0x14 * 4);
        segofx00 = PEEK(0, (0x14 * 4) + 2);
        
        /* Peek in start of FOSSIL + 6 */

        return (PEEK(segofx00, ofsofx00 + 6));
}

FOSSILSTATUS  x00_set(unsigned char set, PORT port)
{
        FOSSILSTATUS retval;
        
        x00regs.x.ax = SET_BAUDRATE | set;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        retval.statusword = x00regs.x.ax;
        return retval;
}

FOSSILSTATUS  x00_tx_char(unsigned char c, PORT port)
{
        FOSSILSTATUS retval;
        
        x00regs.x.ax = TX_CHAR | c;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        retval.statusword = x00regs.x.ax;
        return retval;
}

unsigned char  x00_rx_char(PORT port)
{
        x00regs.x.ax = RX_CHAR;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        return (unsigned char)(x00regs.x.ax & 0xff);
}

FOSSILSTATUS  x00_status(PORT port)
{
        FOSSILSTATUS retval;
        
        x00regs.x.ax = STATUS;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        retval.statusword = x00regs.x.ax;
        return retval;
}

FOSSILINIT    x00_init(PORT port, unsigned char far *ctlc_flagbyte)
{
        FOSSILINIT retval;
        
        x00regs.x.ax = INITIALIZE;
        if (ctlc_flagbyte != (unsigned char far *)0)
        {
                x00regs.x.dx = 0x00ff;
                x00regs.x.bx = 0x4F50;
                segread(&x00sregs);
                x00sregs.es  = FP_SEG(ctlc_flagbyte);
                x00regs.x.cx = FP_OFF(ctlc_flagbyte);
        }
        else
        {
                x00regs.x.bx = 0x0000; /* in any case _NOT_ 0x4f50 */
                x00regs.x.dx = port;
        }
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        retval.result       = x00regs.x.ax;
        retval.max_function = (unsigned char)(x00regs.x.bx & 0xff);
        retval.revision     = (unsigned char)(x00regs.x.bx >> 8);
        return retval;
}

void          x00_deinit(PORT port)
{
        x00regs.x.ax = DEINITIALIZE;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_raise_dtr(PORT port)
{
        unsigned int retval;
        
        x00regs.x.ax = RAISE_DTR;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        if ((x00regs.x.ax & 0x0001) == 1)
        {
                retval = X00_DTR_HIGH;
        }
        else    retval = X00_DTR_LOW;
        return retval;
}

unsigned int  x00_lower_dtr(PORT port)
{
        unsigned int retval;
        
        x00regs.x.ax = LOWER_DTR;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        if ((x00regs.x.ax & 0x0001) == 1)
        {
                retval = X00_DTR_HIGH;
        }
        else    retval = X00_DTR_LOW;
        return retval;
}

FOSSILSYSINFO x00_sysinfo(void)
{
        FOSSILSYSINFO retval;
        
        x00regs.x.ax = GET_SYS_INFO;
        int86(0x14, &x00regs, &x00regs);
        retval.tick_number        = (unsigned char)(x00regs.x.ax & 0xff);
        retval.ticks_per_second   = (unsigned char)(x00regs.x.ax >> 8);
        retval.approx_ms_per_tick = x00regs.x.dx;
        return retval;
}

void          x00_flush_output(PORT port)
{
        x00regs.x.ax = FLUSH_OUTPUT;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_purge_output(PORT port)
{
        x00regs.x.ax = PURGE_OUTPUT;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_purge_input(PORT port)
{
        x00regs.x.ax = PURGE_INPUT;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_tx_char_nowait(unsigned char c, PORT port)
{
        unsigned int retval;
        
        x00regs.x.ax = TX_CHAR_NOWAIT | c;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        if ((x00regs.x.ax & 0x0001) == 1)
        {
                retval = X00_OK;
        }
        else    retval = X00_CHAR_NOT_SENT;
        return retval;
}

unsigned int  x00_peek_ahead_input(PORT port)
{
        x00regs.x.ax = PEEK_AHEAD_INPUT;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        return x00regs.x.ax;
}

unsigned int   x00_peek_ahead_kbd(void)
{
        x00regs.x.ax = PEEK_AHEAD_KBD;
        int86(0x14, &x00regs, &x00regs);
        return x00regs.x.ax;
}

unsigned int  x00_read_kbd(void)
{
        x00regs.x.ax = READ_KBD;
        int86(0x14, &x00regs, &x00regs);
        return x00regs.x.ax;
}

void          x00_flow_control(FOSSILFLOWCTRL f, PORT port)
{
        x00regs.x.ax = FLOW_CONTROL | 0xf0 | f.flowword;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_ctlc_ctlk_check(FOSSILCTLCCTLK c, PORT port)
{
        x00regs.x.ax = CTLC_CTLK_CHECK | c.checkword;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
        return x00regs.x.ax;
}

void          x00_set_cup(unsigned char row, unsigned char col)
{
        x00regs.x.ax = SET_CUP;
        x00regs.x.dx = (row << 8) | col;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_get_cup(unsigned char *row, unsigned char *col)
{
        x00regs.x.ax = GET_CUP;
        int86(0x14, &x00regs, &x00regs);
        *col = (unsigned char)(x00regs.x.dx & 0xff);
        *row = (unsigned char)(x00regs.x.dx >> 8);
}

void          x00_write_ANSI_char(unsigned char c)
{
        x00regs.x.ax = WRITE_ANSI_CHAR | c;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_enable_watchdog(PORT port)
{
        x00regs.x.ax = ENABLE_WATCHDOG;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_disable_watchdog(PORT port)
{
        x00regs.x.ax = DISABLE_WATCHDOG;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_write_BIOS_char(unsigned char c)
{
        x00regs.x.ax = WRITE_BIOS_CHAR | c;
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_insert_tick_func(void (far *tickfunc)())
{
        unsigned int retval;
        
        x00regs.x.ax = INSERT_TICK_FUNC;
        x00regs.x.dx = FP_OFF(tickfunc);
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(tickfunc);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        if (x00regs.x.ax == 0x0000)
        {
                retval = X00_OK;
        }
        else    retval = X00_INS_TICK;
        return retval;
}

unsigned int  x00_delete_tick_func(void (far *tickfunc)())
{
        unsigned int retval;
        
        x00regs.x.ax = DELETE_TICK_FUNC;
        x00regs.x.dx = FP_OFF(tickfunc);
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(tickfunc);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        if (x00regs.x.ax == 0x0000)
        {
                retval = X00_OK;
        }
        else    retval = X00_DEL_TICK;
        return retval;
}

void          x00_boot_machine(unsigned int boottype)
{
        x00regs.x.ax = BOOT_MACHINE | (boottype & 0x0001);
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_read_block(unsigned int count, void far *buf, PORT port)
{
        x00regs.x.ax = READ_BLOCK;
        x00regs.x.cx = count;
        x00regs.x.dx = port;
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(buf);
        x00regs.x.di = FP_OFF(buf);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        return x00regs.x.ax;
}

unsigned int  x00_write_block(unsigned int count, void far *buf, PORT port)
{
        x00regs.x.ax = WRITE_BLOCK;
        x00regs.x.cx = count;
        x00regs.x.dx = port;
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(buf);
        x00regs.x.di = FP_OFF(buf);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        return x00regs.x.ax;
}

void          x00_start_break_signal(PORT port)
{
        x00regs.x.ax = START_BREAK_SIGNAL;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

void          x00_stop_break_signal(PORT port)
{
        x00regs.x.ax = STOP_BREAK_SIGNAL;
        x00regs.x.dx = port;
        int86(0x14, &x00regs, &x00regs);
}

unsigned int  x00_get_driverinfo(void far *buf, PORT port)
{
        x00regs.x.ax = GET_DRIVER_INFO;
        x00regs.x.cx = sizeof(FOSSILINFO);
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(buf);
        x00regs.x.di = FP_OFF(buf);
        x00regs.x.dx = port;
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        return x00regs.x.ax;
}

unsigned int  x00_install_appendage(unsigned char appcode,
                                              void (far *appfunc)())
{
        unsigned int retval;
        
        x00regs.x.ax = INSTALL_APPENDAGE | appcode;
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(appfunc);
        x00regs.x.dx = FP_OFF(appfunc);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        if (x00regs.x.ax == X00_PRESENT)
        {
                if ((x00regs.x.bx & 0xff00) == 1)
                {
                        retval = X00_OK;
                }
                else    retval = X00_INS_APP;
        }
        else    retval = X00_NOT_HERE;
        return retval;
}

unsigned int  x00_remove_appendage(unsigned char appcode,
                                             void (far *appfunc)())
{
        unsigned int retval;
        
        x00regs.x.ax = REMOVE_APPENDAGE | appcode;
        segread(&x00sregs);
        x00sregs.es  = FP_SEG(appfunc);
        x00regs.x.dx = FP_OFF(appfunc);
        int86x(0x14, &x00regs, &x00regs, &x00sregs);
        if (x00regs.x.ax == X00_PRESENT)
        {
                if ((x00regs.x.bx & 0xff00) == 1)
                {
                        retval = X00_OK;
                }
                else    retval = X00_REM_APP;
        }
        else    retval = X00_NOT_HERE;
        return retval;
}

#if __cplusplus
 }
#endif
