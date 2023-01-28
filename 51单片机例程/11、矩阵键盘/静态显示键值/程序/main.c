/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : 矩阵键盘显示试验
* 实验说明       : 静态数码管显示矩阵键盘键值
* 连接方式       : 见连接图
* 注    意		 : 
*******************************************************************************/

#include<reg51.h>

//--定义使用的IO口--//
#define GPIO_DIG P0		
#define GPIO_KEY P1

//--定义全局变量--//
unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char KeyValue;
//用来存放读取到的键值

//--声明全局函数--//
void Delay10ms(unsigned int c);    //延时10ms
void KeyDown();		 //检测按键函数
 
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	while(1)
	{
		KeyDown();
		GPIO_DIG = ~DIG_CODE[KeyValue];
	}				
}

/*******************************************************************************
* 函 数 名         : KeyDown
* 函数功能		   : 检测有按键按下并读取键值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void KeyDown(void)
{
	char a = 0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		Delay10ms(1);//延时10ms进行消抖
		if(GPIO_KEY!=0x0f)//再次检测键盘是否按下
		{
			
			//测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=4;break;
				case(0X0d): KeyValue=8;break;
				case(0X0e):	KeyValue=12;break;
			}
			//测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue+3;break;
				case(0Xb0):	KeyValue=KeyValue+2;break;
				case(0Xd0): KeyValue=KeyValue+1;break;
				case(0Xe0):	KeyValue=KeyValue;break;
			}
			while((a<50) && (GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				Delay10ms(1);
				a++;
			}
		}
	}
}

/*******************************************************************************
* 函 数 名         : Delay10ms
* 函数功能		   : 延时函数，延时10ms
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a, b;

	//--c已经在传递过来的时候已经赋值了，所以在for语句第一句就不用赋值了--//
    for (;c>0;c--)
	{
		for (b=38;b>0;b--)
		{
			for (a=130;a>0;a--);
		}          
	}       
}