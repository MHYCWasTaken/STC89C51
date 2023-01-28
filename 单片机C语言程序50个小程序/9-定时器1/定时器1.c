/*-----------------------------------------------
  名称：定时器1
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：通过定时让LED灯闪烁
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

sbit LED=P1^2;    //定义LED端口

/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer1(void)
{
 TMOD |= 0x10;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响 
 TH1=0x00;	      //给定初值，这里使用定时器最大值从0开始计数一直到65535溢出
 TL1=0x00;
 EA=1;            //总中断打开
 ET1=1;           //定时器中断打开
 TR1=1;           //定时器开关打开
}
/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 Init_Timer1();
 while(1);
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 using 1
{
 TH1=0x00;		 //重新赋值
 TL1=0x00;

 LED=~LED;       //LED闪烁

}
