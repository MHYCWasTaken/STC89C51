//打开串口调试程序，将波特率设置为2400，无奇偶校验


#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */


#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
sbit KEY=P3^2;

void delay(unsigned int U)
{
 while(--U);
}
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void)
{

/*------------------------------------------------
Setup the serial port for 1200 baud at 11.0592MHz.
------------------------------------------------*/
#ifndef MONITOR51
    SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr      */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 0xf3;                /* TH1:  reload value for 2400 baud @ 12MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    TI    = 1;                  /* TI:   set TI to send first char of UART    */
#endif

/*------------------------------------------------
Note that an embedded program never exits (because
there is no operating system to return to).  It
must loop and execute forever.
------------------------------------------------*/
  while (1) 
    {
    
    if(!KEY)
       {
        delay(10000);
        if(!KEY)
          {
       		 printf ("This programer test ok!\n");  
      		 printf ("12:25:26\n");  
           }
		}
    }
}

