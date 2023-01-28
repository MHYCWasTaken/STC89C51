/********************************************************************
* 名称 : 电机调速.c
* 功能 : 通过P1.0口来控制直流电机的旋转的快慢
/***********************************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int
sbit KEY1 = P3^2;		//定义调速的按键
sbit PWM = P1^0;		//定义直流电机的控制端口
uchar CYCLE;  			//定义周期 
uchar PWM_ON ;			//定义低电平时间

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

void Time0(void)
{
	TMOD |=0x01;		  //定时器设置 1ms in 12M crystal
	TH0=(65536-1000)/256; 
	TL0=(65536-1000)%256; //定时1mS 
	IE= 0x82;  			  //打开中断
	TR0=1;				  //定时器打开
}
main()
{
	uchar PWM_Num;	//定义档位
	Time0();
	CYCLE = 10;		//时间可以调整 这个是10步调整 周期10ms 8位PWM就是256步
	PWM = 0;
	while(1)
	{
		if(!KEY1)
  		{
   			Delay(15);
   			if(!KEY1)
     		{
      			PWM_Num++;	      //按一下P3.2口，就给直流电机换一个档位
	  			if(PWM_Num==4)
				{
					PWM_Num=0;
		        }
				switch(PWM_Num)
				{
		        	case 0:P0=0x06;		PWM_ON=0;break;			//低电平时长     //给P0口赋值，是让P0口显示：1,2,3,4 这四个档位
		       		case 1:P0=0x5B;		PWM_ON=2;break;
		       		case 2:P0=0x4F;		PWM_ON=4;break;
		       		case 3:P0=0x66;		PWM_ON=6;break;
		         	default:break;
     			}
  			}
		}
	}
}
/********************************/
/*      定时中断                */
/********************************/
void tim(void) interrupt 1 using 1
{
	static unsigned char count; 
	TH0=(65536-1000)/256; 
	TL0=(65536-1000)%256;//定时1mS 
	if(count==PWM_ON)
	{
		PWM = 1; 		//直流电机转 
	}
	count++;
	if(count == CYCLE)
	{
		count=0;
		if(PWM_ON!=0)   //如果左右时间是0 保持原来状态
		{
			PWM = 0;    //直流电机不转
		}
	}
}
