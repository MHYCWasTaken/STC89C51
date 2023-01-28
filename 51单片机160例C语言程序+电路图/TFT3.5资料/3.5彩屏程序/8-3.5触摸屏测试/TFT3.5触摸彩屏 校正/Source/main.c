
#include"tft.h"
#include"reg52.h"
#include"touch.h"
#include<stdio.h>



main()
{
 //初始化串口，用于内部测试。更具需要自行更改
 SCON  = 0x50;		        // SCON: 模式 1, 8-bit UART, 使能接收  
 TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit 重装
 TH1   = 0xFD;               // TH1:  重装值 9600 波特率 晶振 11.0592MHz  
 TR1   = 1;                  // TR1:  timer 1 打开  
                       
 TFT_Initial();
 Touch ();
 while(1);
}
