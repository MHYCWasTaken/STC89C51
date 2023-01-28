//实例52：用定时器T0控制输出高低宽度不同的矩形波
#include<reg51.h>   //包含51单片机寄存器定义的头文件
sbit u=P3^0;      //将u位定义为P3.0，从该引脚输出矩形脉冲
unsigned char Countor;  //设置全局变量，储存负跳变累计数
/*************************************************
函数功能：延时约30ms (3*100*100=30 000μs =30ms)
*************************************************/
void delay30ms(void)
{
  unsigned char m,n;
  for(m=0;m<100;m++)
    for(n=0;n<100;n++)
	   ;
}

/*******************************************
函数功能：主函数
******************************************/	
void main(void)
  {
   unsigned char i;
   EA=1;   //开放总中断
   EX0=1;  //允许使用外中断
   IT0=1;  //选择负跳变来触发外中断
	Countor=0;
   for(i=0;i<100;i++)  //输出100个负跳变
	{
	  u=1;
	  delay30ms();
	  u=0;
	  delay30ms();
	}
	while(1)                
       ; //无限循环， 防止程序跑飞
 }
/**************************************************************
函数功能：外中断T0的中断服务程序
**************************************************************/
void int0(void) interrupt 0 using 0 //外中断0的中断编号为0
{
  Countor++;
  P1=Countor;

 }
		