/*******************************************************************************
*  标题:                  试验74HC595驱动数码管上显示数字（C语言）             *
* 																			   *
*   连接方法：JP595用跳线冒短接  JP3和P595_A 用8PIN排线连接					       *													 
********************************************************************************
* 通过本例程了解 74HC595（串入并出）基本原理和使用  		                   *
*                         	       										       *
* 请学员认真消化本例程，懂74C595在C语言中的操作                                *
********************************************************************************/


#include <reg51.h> 
#include <intrins.h>
#define  NOP() _nop_()  /* 定义空指令 */

//SPI IO
sbit MOSIO =P3^4;  //串行数据线
sbit R_CLK =P3^5;  //数据并行输出控制
sbit S_CLK =P3^6;  //串行时钟线


void delay(unsigned int i);      //函数声名
void HC595SendData(unsigned char SendVal);  //函数声名
 
// 此表为 LED 的字模            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)
unsigned char code LED7Code[] = {~0x3F,~0x06,~0x5B,~0x4F,~0x66,~0x6D,~0x7D,~0x07,~0x7F,~0x6F,~0x77,~0x7C,~0x39,~0x5E,~0x79,~0x71};
 
main()
{  unsigned char  HC595SendVal;
   unsigned int LedNumVal;
   
  while(1)
  {	 
	 	LedNumVal++;
		HC595SendVal = LED7Code[LedNumVal%16];     //LED7;显示0-F  LedNumVal%10 显示0-9
	    HC595SendData(HC595SendVal); //调用595驱动函数	
  		delay(200);
	 
  }   
}	
/*******************延时函数************/
void delay(unsigned int i)
{
    unsigned int j;
    for(i; i > 0; i--)	 //CPU循环执行i*300次
        for(j = 300; j > 0; j--);
}


/*********************************************************************************************************
** 函数名称: HC595SendData
** 功能描述: 向SPI总线发送数据
*********************************************************************************************************/
void HC595SendData(unsigned char SendVal)
{  
  unsigned char i;
		
  for(i=0;i<8;i++) 
   {
	if((SendVal<<i)&0x80) MOSIO=1; //set dataline high  0X80  最高位与SendVal左移的最高位 进行逻辑运算
	else MOSIO=0;				   // 如果为真 MOSIO = 1  
 
	S_CLK=0;  
	NOP();	 //短暂延时产生一定宽度的脉冲信号
	NOP();	 //短暂延时
	S_CLK=1;
		
   }

 
  R_CLK=0; //set dataline low
  NOP();  //短暂延时
  NOP();  //短暂延时
  R_CLK=1; 	//
	

}

