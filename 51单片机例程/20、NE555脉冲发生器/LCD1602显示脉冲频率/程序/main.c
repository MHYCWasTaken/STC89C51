/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : NE555脉冲计数显示试验
* 实验说明       : LCD1602显示接收到的脉冲频率。
* 连接方式       : 见连接图
* 注    意		 : 
*******************************************************************************/

#include<reg51.h>
#include"lcd.h"

#define uchar unsigned char 
#define uint  unsigned int
#define ulong	unsigned long
//--定义LCD显示的文字--//
uchar code EN_CHAR1[16]={"   CYMOMETER    "};
uchar code EN_CHAR2[16]={"FREQ:         HZ"};

//--声明全局函数--//
void TIMER_Configuration();	//初始化定时器
//--定义全局变量--//
ulong Freq;        //用来存放要显示的频率值
ulong	TimeCount; //用于计算1S钟的
/*******************************************
*函数名		：main
*功  能 	：主函数
*输	 入 	：无
*输  出		：无
*******************************************/

void main()
{
	uchar i, freqValue[6];

	LcdInit();
	TIMER_Configuration();

	//--显示要写的字母--//
	for(i=0;i<16;i++)
	{
		LcdWriteData(EN_CHAR1[i]);
	}	
	LcdWriteCom(0xc0);//第二行显示
	for(i=0;i<16;i++)
	{
		LcdWriteData(EN_CHAR2[i]);
	}
	
	while(1)
	{
		if(TR0==0)         //当计数器停下的时候，表明计数完毕
		{
			Freq = Freq + TL1;       //读取TL的值
			Freq = Freq + (TH1 * 256); //读取TH的值
			LcdWriteCom(0xc8);
			//--求频率的个十百千万十万位--//
			freqValue[0]='0'+Freq%1000000/100000;	
			freqValue[1]='0'+Freq%100000/10000;	
			freqValue[2]='0'+Freq%10000/1000;	
			freqValue[3]='0'+Freq%1000/100;	
			freqValue[4]='0'+Freq%100/10;	
			freqValue[5]='0'+Freq%10;	
			//--显示求得的数值--//
			for(i=0;i<5;i++)//从最高位开始查找不为0的数开始显示(最低位为0显示0)
			{
				if(freqValue[i]==0x30)
				{
					freqValue[i]=0x20;	//若为0则赋值空格键
				}
								
				else
				{
					break;
				}
					
			}
			for(i=0;i<6;i++)
			{
				LcdWriteData(freqValue[i]);	
			}
			//--显示完，重新计算下一次频率。--//	
			Freq=0;//将计算的频率清零
			TH1=0; //将计数器的值清零
			TL1=0;
			TR0=1; //开启定时器
			TR1=1; //开启计数器
		}
	}
}

/*******************************************
*函数名		：TIMER_Configuration
*功  能 	：配置定时/计数器T0和T1
*输	 入 	：无
*输  出		：无
*******************************************/

void TIMER_Configuration()
{
	//--定时器T1做计数器，工作方式1（16位定时器），只由TRx打开计数器--//
	//--定时器T0做定时器，工作方式1（16位定时器），只由TRx打开定时器--//	
	TMOD=0x51;

	//--设置定时器晶振为12MHZ时定时50ms--//
	TH0=0x3C;
	TL0=0xB0;

	//--打开中断-//
	ET0=1;
	ET1=1;
	EA=1;

	//--打开定时器*/
	TR0=1;
	TR1=1;
}
/*******************************************
*函数名		：Timer0
*功  能 	：定时器0的中断函数
*输	 入 	：无
*输  出		：无
*******************************************/

void Timer0()	interrupt 1
{
	TimeCount++;
	if(TimeCount==20)//计时到1S
	{
		TR0=0;
		TR1=0;
		TimeCount=0;		
	}

	//--12MHZ设置定时50ms的初值--//
	TH0=0x3C;
	TL0=0xB0;	
}
/*******************************************
*函数名		：Timer1
*功  能 	：定时器1的中断函数
*输	 入 	：无
*输  出		：无
*******************************************/

void Timer1()	interrupt 3
{	
	//--进入一次中断，表明计数到了65536--//
	Freq=Freq+65536;		
}
