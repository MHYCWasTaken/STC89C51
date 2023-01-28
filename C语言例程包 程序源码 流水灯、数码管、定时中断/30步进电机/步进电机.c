/********************************************************************
* 名称 : 步进电机.c
* 功能 : 通过P1.3，P1.4，P1.5，P1.6四个IO口，来控制步进电机。
		 杜邦线的连接线，可以参考使用说明的1.3节。
/***********************************************************************/
#include <reg52.h> 
#define uchar unsigned char
#define uint  unsigned int
 
sbit KEY1 = P3^2;   //步进电机顺时针方向转
sbit KEY2 = P3^3;   //步进电机逆时针方向转
sbit KEY3 = P3^4;	//步进电机调速

uchar  Step = 0;
bit FB_flag = 0;

unsigned char code F_Rotation[8]={0x08,0x18,0x10,0x30,0x20,0x60,0x40,0x48};    //顺时针转表格
unsigned char code B_Rotation[8]={0x48,0x40,0x60,0x20,0x30,0x10,0x18,0x08};    //逆时针转表格

/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms * x
* 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay(uint i)
{
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}

void KEY(void)
{
	if(KEY1 == 0)	   //按P3.2，实现步进电机的顺时针转动
	{
		Delay(15);
		if(KEY1 == 0)
		{
			FB_flag = 0;
		}
		Delay(200);
	}
	if(KEY2 == 0) 		//按P3.3，实现步进电机的逆时针转动
	{
		Delay(15);
		if(KEY2 == 0)
		{
			FB_flag = 1;
		}
		Delay(200);
	}
	if(KEY3 == 0)		//按P3.4，实现步进电机的调速
	{
		Delay(15);
		if(KEY3 == 0)
		{
			Step++;
			if(Step == 3)
			{
				Step = 0;
			}
		}
		Delay(200);
	}
}
main()
{
 	uchar i; 
	//uint k = 0;
	while(1)
 	{ 
		KEY();				  		//按键处理函数
  		for(i=0;i<8;i++)      		//因为有8路的控制时序
     	{
			//k++;
			//if(k == 4096) while(1);
			if(FB_flag == 0)
			{
     			P1 = F_Rotation[i];  //顺时针转动
			}
			else
			{
				P1 = B_Rotation[i];	 //逆时针转动
			}
     		Delay(1+Step);        	 //改变这个参数可以调整电机转速
	 	}
  	}
}