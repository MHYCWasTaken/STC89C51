/*
1、按键可以调整当时时间
2、按键可以调节定时时间
3、定时时间到继电器吸合

*8位数码管显示 时间格式格式 12-08-00 标示12点08分00秒

S1 用于小时 加1操作  
S2 用于分钟 加1操作
S2 用于分钟减1操作
*/

#include <reg52.h>


sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;


sbit LED=P1^2;
code unsigned char tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; 
			//共阴数码管 0-9
unsigned char StrTab[8];  //定义缓冲区
unsigned char minute=30,hour=12,second;  //定义并且初始化值 12:30:00

void delay(unsigned int cnt)//延时函数
{
 while(--cnt);
}


void Displaypro(void)
{

	        StrTab[0]=tab[hour/10];    //显示正常时间
			StrTab[1]=tab[hour%10];
			StrTab[2]=0x40;
			StrTab[3]=tab[minute/10];
			StrTab[4]=tab[minute%10];
			StrTab[5]=0x40;
            StrTab[6]=tab[second/10];
			StrTab[7]=tab[second%10];
	
}

main()//主函数
{

    
    TMOD |=0x01;//定时器0 10ms in 12M crystal 用于计时
	TH0=0xd8;
	TL0=0xf0;
	ET0=1;
	TR0=1;

    TMOD |=0x10; //定时器1用于动态扫描
	TH1=0xF8;
	TL1=0xf0;
	ET1=1;
	TR1=1;
    EA =1;
    

 Displaypro();

while(1)//主循环
  {
   
/////////////////////////////////////////////////////////////////
  if(!KEY1) //按键1去抖以及动作
    {
    delay(10000);
    if(!KEY1)
	   {          
	     hour++;if(hour==24)hour=0;     //正常时间 小时 加1
         Displaypro();
	   }
	}
////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
  if(!KEY2) //按键去抖以及动作
    {
    delay(10000);
    if(!KEY2)
	   {
	    minute++;if(minute==59)minute=0;//分加1
	    Displaypro();
	  }
	}
/////////////////////////////////////////////////////////////////
  if(!KEY3) //按键去抖以及动作
    {
    delay(10000);
    if(!KEY3)
	   {
	    minute--;if(minute==0)minute=59;  //分减1
	    Displaypro();
	  }
	}
  }
}
////////////////////////////////////////////////////////////////
/********************************/
/*      定时中断1                */
/********************************/
void time1_isr(void) interrupt 3 using 0//定时器1用来动态扫描
{
    static unsigned char num;
    TH1=0xF8;//重入初值
	TL1=0xf0;

	switch (num){
     case 0:P2=0;P0=StrTab[num];break; //分别调用缓冲区的值进行扫描
	 case 1:P2=1;P0=StrTab[num];break;
	 case 2:P2=2;P0=StrTab[num];break;
	 case 3:P2=3;P0=StrTab[num];break;
	 case 4:P2=4;P0=StrTab[num];break;
	 case 5:P2=5;P0=StrTab[num];break;
     case 6:P2=6;P0=StrTab[num];break;
	 case 7:P2=7;P0=StrTab[num];break;
	 default:break;
}
num++;     //扫描8次，使用8个数码管
if(num==8)
   num=0;
}
//////////////////////////////////////////////////////////////////
/********************************/
/*      定时中断0                */
/********************************/
void tim(void) interrupt 1 using 1
{
static unsigned char count;//定义内部静态变量

TH0=0xd8;//重新赋值
TL0=0xf0;
count++;
   switch (count)
          {
           case 0:
           case 20:
           case 40:
           case 60:
           case 80:Displaypro(); //隔一定时间调用显示处理
                   break;
           case 50:LED=!LED; //半秒 LED闪烁
                   break;
           default:break;
          }

if (count==100)
    {
    count=0;
    second++;//秒加1
	if(second==60)
	   {
	   second=0;
	   minute++; //分加1
	   if(minute==60)
		  {
		   minute=0;
		   hour++;  //时加1
		   if(hour==24)
		      hour=0;
		  }  
      
	   }
    
   } 
 
}