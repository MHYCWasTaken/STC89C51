/*******************************************************************************
*  标题:                  试验数码管上显示定时器中断计数                       *
*																			   *
*  连接方法：参照接线图                               *
********************************************************************************
*                         	       										       *
* 请学员认真消化本例程，用单片机脚直接控制数码管和定时器中断使用               *
********************************************************************************/

#include < reg51.h >
#include <intrins.h>

sbit LS138A=P2^2;  //定义 138管脚
sbit LS138B=P2^3;
sbit LS138C=P2^4; 


unsigned int LedNumVal,LedOut[8];

// 此表为 LED 的字模            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code Disp_Tab[] = { ~0xC0,~0xF9,~0xA4,~0xB0,~0x99,~0x92,~0x82,~0xF8,~0x80,~0x90,~0x88,~0x83,~0xC6,~0xA1,~0x86,~0xbf,~0xc7,~0x8c,~0xc1, ~0xff,  ~0xf7 }; 


/***********************************************************
purpose: 系统初始化
/**********************************************************/
void system_Ini()
{
     TMOD = 0x00; //选择模式0  13位计数 最大计数8192
  
   // TH0=0x1C;   //12.000M  预装初值
    //TL0=0x18;	
	 TH0 = (8192 - 1000)/32;
	 TL0 = (8192 - 1000)%32	;
     IE = 0x8A;   //中断	
     TR0  = 1;    //开启T0定时器


}

void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}


/********主函数****/
main()
{ 
   system_Ini()	;
 
 
   while(1)
   {
  
   unsigned char i;
	 
     LedOut[0]=Disp_Tab[LedNumVal%10000/1000];
     LedOut[1]=Disp_Tab[LedNumVal%1000/100];
     LedOut[2]=Disp_Tab[LedNumVal%100/10];
     LedOut[3]=Disp_Tab[LedNumVal%10];


	 for( i=0; i<8; i++) 
	 {	 P0 = LedOut[i];
			
	  switch(i)					  
	     {	    
			case 0:LS138A=0; LS138B=0; LS138C=0; break;         
	        case 1:LS138A=1; LS138B=0; LS138C=0; break;             	
	        case 2:LS138A=0; LS138B=1; LS138C=0; break; 
	        case 3:LS138A=1; LS138B=1; LS138C=0; break; 
			case 4:LS138A=0; LS138B=0; LS138C=1; break;
			case 5:LS138A=1; LS138B=0; LS138C=1; break;
			case 6:LS138A=0; LS138B=1; LS138C=1; break;
			case 7:LS138A=1; LS138B=1; LS138C=1; break;
			
	     }

		delay(150);
	 }

  }

 }

 

 /*************************************
 [ T0 (1ms)中断] 中断

*************************************/
void T0zd(void) interrupt 1      //3定时器1的中断号  1定时器0的中断号 0外部中断1 2外部中断2  4串口中断
{
	 //TH0 = 0xFc;    //12.000
	 //TL0 = 0x18;

	 TH0 = (8192 - 1000)/32	;
	 TL0 = (8192 - 1000)%32;

 	 LedNumVal++;
}