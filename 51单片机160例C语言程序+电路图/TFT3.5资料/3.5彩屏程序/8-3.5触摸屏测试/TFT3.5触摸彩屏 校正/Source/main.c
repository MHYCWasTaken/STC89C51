
#include"tft.h"
#include"reg52.h"
#include"touch.h"
#include<stdio.h>



main()
{
 //��ʼ�����ڣ������ڲ����ԡ�������Ҫ���и���
 SCON  = 0x50;		        // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ���  
 TMOD |= 0x20;               // TMOD: timer 1, mode 2, 8-bit ��װ
 TH1   = 0xFD;               // TH1:  ��װֵ 9600 ������ ���� 11.0592MHz  
 TR1   = 1;                  // TR1:  timer 1 ��  
                       
 TFT_Initial();
 Touch ();
 while(1);
}
