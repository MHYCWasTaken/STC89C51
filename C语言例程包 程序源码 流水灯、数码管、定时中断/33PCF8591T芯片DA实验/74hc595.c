
/*************************此部分为74HC595的驱动程序使用SPI总线连接*************************************/

#include<reg52.h>
#include <intrins.h>

#define  NOP()   _nop_()    /* 定义空指令 */
#define  _Nop()  _nop_()    /*定义空指令*/

//SPI IO
sbit    MOSIO =P1^5;
sbit    R_CLK =P1^6;
sbit    S_CLK =P1^7;
sbit    OE    =P3^6;

void HC595SendData(unsigned int SendVal);


/*********************************************************************************************************
** 函数名称: HC595SendData
** 功能描述: 向SPI总线发送数据
** 管脚描述：请参考相关的芯片资料(学习光盘中以配)
*********************************************************************************************************/
void HC595SendData(unsigned int SendVal)
{  
  unsigned char i;
		
  for(i=0;i<16;i++) 
   {
	if((SendVal<<i)&0x8000) MOSIO=1; // set dataline high
	else MOSIO=0;
 
	S_CLK=0;
	NOP();
	NOP();
	S_CLK=1;	
   }
   
	
  R_CLK=0; //set dataline low
  NOP();
  NOP();
  R_CLK=1; //片选
  OE=0;

}


