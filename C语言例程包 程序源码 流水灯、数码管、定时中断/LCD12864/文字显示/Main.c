/*
 ----------------------------------------------------
 *作    者：温子祺
 ----------------------------------------------------
 */
#include "stc.h"
#include <intrins.h>
/***************************************************
 *          大量宏定义，便于代码移植和阅读
 ***************************************************/
#define NOP()              _nop_() 
#define HIGH               1
#define LOW                0
#define LS164_DATA(x)      {if((x))P0_4=1;else P0_4=0;}         
#define LS164_CLK(x)       {if((x))P0_5=1;else P0_5=0;} 
#define LCD12864_RS(x)      {if((x))P0_0=1;else P0_0=0;}//RS引脚控制
#define LCD12864_RW(x)      {if((x))P0_1=1;else P0_1=0;}//R/W引脚控制
#define LCD12864_EN(x)      {if((x))P0_2=1;else P0_2=0;}//E引脚控制
#define LCD12864_MD(x)      {if((x))P0_3=1;else P0_3=0;}//PSB引脚控制
#define LCD12864_PORT        LS164Send//发送数据

/********************************************
*函数名称:DelayNus
*输    入:t 延时时间
*输    出:无
*说    明:微秒级延时
********************************************/
void DelayNus(unsigned int t)
{
	unsigned int d=0;

	d=t;

	do
	{
		NOP();
	}while(--d >0);	
}
/********************************************
*函数名称:LS164Send
*输    入:byte 要发送的字节
*输    出:无
*说    明:74LS164发送数据
********************************************/
void LS164Send(unsigned char byte)
{
   unsigned char j;

   for(j=0;j<=7;j++)
   {
     if(byte&(1<<(7-j)))
     {
	    LS164_DATA(HIGH);
	 }
	 else
	 {
	    LS164_DATA(LOW);
	 }

      LS164_CLK(LOW); 
      LS164_CLK(HIGH);
       
   }
}
/********************************************
*函数名称:LCD12864WriteByte
*输    入:byte 要写入的字节
*输    出:无
*说    明:LCD12864 写字节
********************************************/
void LCD12864WriteByte(unsigned char byte)
{
     LCD12864_PORT(byte);
     LCD12864_RS(HIGH);
     LCD12864_RW(LOW);
     LCD12864_EN(LOW);
     DelayNus(5);
     LCD12864_EN(HIGH); 
}
/********************************************
*函数名称:LCD12864WriteCommand
*输    入:command 要写入的命令
*输    出:无
*说    明:LCD12864 写命令
********************************************/
void LCD12864WriteCommand(unsigned char command)
{
     LCD12864_PORT(command);
     LCD12864_RS(LOW);
     LCD12864_RW(LOW);
     LCD12864_EN(LOW);
     DelayNus(5);
     LCD12864_EN(HIGH); 

}
/********************************************
*函数名称:LCD12864SetXY
*输    入:x 横坐标 y 纵坐标
*输    出:无
*说    明:LCD12864 设置坐标
********************************************/
void LCD12864SetXY(unsigned char x,unsigned char y)
{
 	 switch(y)
     {
		case 1:
        {
             LCD12864WriteCommand(0x80|x);
        }
        break;

		case 2:
        {
             LCD12864WriteCommand(0x90|x);
        }
        break;

		case 3:
        {
             LCD12864WriteCommand(0x88|x);
        }
        break;

		case 4:
        {
             LCD12864WriteCommand(0x98|x);
        }
        break;

		default:break;

     }
}
/********************************************
*函数名称:LCD12864PrintfString
*输    入:x 横坐标 y 纵坐标 s字符串
*输    出:无
*说    明:LCD12864 打印字符串
********************************************/
void LCD12864PrintfString(unsigned char x,
                          unsigned char y,
                          unsigned char *s)
{
	LCD12864SetXY(x,y);
	 
     while(s && *s)
     {
        LCD12864WriteByte(*s);
        s++;
        
     }
}
/********************************************
*函数名称:LCD12864ClearScreen
*输    入:无
*输    出:无
*说    明:LCD12864 清屏
********************************************/ 
void LCD12864ClearScreen(void)
{
	 LCD12864WriteCommand(0x01);
     DelayNus(20);
}
/********************************************
*函数名称:LCD12864Init
*输    入:无
*输    出:无
*说    明:LCD12864 初始化
********************************************/
void LCD12864Init(void)
{
     LCD12864_MD(HIGH);
     LCD12864WriteCommand(0x30);//功能设置，一次送8位数据，基本指令集
     LCD12864WriteCommand(0x0C);//整体显示，游标off，游标位置off
     LCD12864WriteCommand(0x01);//清DDRAM    
     LCD12864WriteCommand(0x02);//DDRAM地址归位
     LCD12864WriteCommand(0x80);//设定DDRAM 7位地址000，0000到地址计数器AC
     LCD12864ClearScreen();
}
/********************************************
*函数名称:PortInit
*输    入:无
*输    出:无
*说    明:I/O初始化
********************************************/
void PortInit(void)
{
	 P0=P1=P2=P3=0xFF;
}
/********************************************
*函数名称:main
*输    入:无
*输    出:无
*说    明:函数主体
********************************************/
void main(void)
{
    unsigned char i=0;

    PortInit();    
    LCD12864Init();//初始化
    LCD12864PrintfString(0,1,"1234567890ABCDEF");//第一行打印
    LCD12864PrintfString(0,2,"----------------");//第二行打印
    LCD12864PrintfString(0,3,"学好电子成就自己");//第三行打印
    LCD12864PrintfString(0,4,"----------------");//第四行打印

	while(1)
    {

        
		;

    }

}

