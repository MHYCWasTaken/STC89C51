/*
1、程序目的：使用定时器学习计时
2、硬件要求：数码管、晶振12M
*/
#include <reg52.h>

code unsigned char tab[]=
			{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90}; 
			//共阳数码管 0-9 

unsigned char Dis_Shiwei;//定义十位
unsigned char Dis_Gewei; //定义个位
void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{

TMOD |=0x01;//定时器设置 10ms in 12M crystal
TH0=0xd8;
TL0=0xf0;
IE= 0x82;  //打开中断
TR0=1;

while(1)
  {
  P0=Dis_Shiwei;//显示十位
  P2=0xFD;
  delay(300);//短暂延时
  P0=Dis_Gewei; //显示个位
  P2=0xFE;
  delay(300);
  }

}
/********************************/
/*      定时中断                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char second,count;
TH0=0xd8;//重新赋值
TL0=0xf0;
count++;
if (count==100)
    {
    count=0;
    second++;//秒加1
	if(second==100)
	   second=0;
	Dis_Shiwei=tab[second/10];//十位显示值处理
    Dis_Gewei=tab[second%10]; //个位显示处理
    
        }


}