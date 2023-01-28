/*
1、程序目的：使用定时器学习秒表计时，中断0控制走表，中断1控制清零 
2、硬件要求：数码管、晶振12M
*/
#include <reg52.h>

code unsigned char tab[]=
			{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			//共阴数码管 0-9 

unsigned char Dis_Shiwei;//定义十位
unsigned char Dis_Gewei; //定义个位
unsigned char S_Dis_Shiwei;//定义秒十位
unsigned char S_Dis_Gewei; //定义秒个位
unsigned char second,msecond;

void CLR(void);
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
EX0=1;//外部中断0设置
IT0=1;

EX1=1;//外部中断1设置
IT1=1;

TMOD |=0x01;//定时器设置 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
ET0=1;  //打开中断
TR0=0;

EA=1;
CLR();
while(1)
  {
  P0=S_Dis_Shiwei;//显示秒十位
  P2=1;
  delay(300);//短暂延时
  P0=S_Dis_Gewei; //显示秒个位
  P2=2;
  delay(300);
  P0=0x40; //显示秒个位
  P2=3;
  delay(300);
  P0=Dis_Shiwei;//显示十位
  P2=4;
  delay(300);//短暂延时
  P0=Dis_Gewei; //显示个位
  P2=5;
  delay(300);
  }
}
/********************************/
/*      定时中断                */
/********************************/
void tim(void) interrupt 1 using 1
{

TH0=0xd8;//重新赋值
TL0=0xf0;
msecond++;
if (msecond==100)
    {
    msecond=0;
    second++;//秒加1
	if(second==100)
	   second=0;
	S_Dis_Shiwei=tab[second/10];//十位显示值处理
    S_Dis_Gewei=tab[second%10]; //个位显示处理

        }
    Dis_Shiwei=tab[msecond/10];//十位显示值处理
    Dis_Gewei=tab[msecond%10]; //个位显示处理

}
void ISR_INT0(void) interrupt 0 using 1
{
 TR0=!TR0; //利用外部中断打开和关闭定时器0 用于开始和停止计时

}
void ISR_INT1(void) interrupt 2 using 1
{
if(TR0==0)//停止时才可以清零
   CLR();
}

void CLR(void)
{
 second=0; //利用外部中断清零
 msecond=0;
 Dis_Shiwei=tab[msecond/10];//十位显示值处理
 Dis_Gewei=tab[msecond%10]; //个位显示处理
 S_Dis_Shiwei=tab[second/10];//十位显示值处理
 S_Dis_Gewei=tab[second%10]; //个位显示处理
}
