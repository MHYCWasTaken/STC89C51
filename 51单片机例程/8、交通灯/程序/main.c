//
/*******************************************************************************
* 实验名			   : 动态显示数码管实验
* 使用的IO	     : 
* 实验效果       : 数码管显示76543210。
*	注意					 ：当位选用P1口的时候注意可能会有一位不亮，那么调整J21
*******************************************************************************/
#include<reg51.h>

//--定义使用的IO口--//
#define GPIO_DIG   P0
#define GPIO_PLACE P1

#define GPIO_TRAFFIC P2

sbit RED10   = P2^0;   //上人行道红灯
sbit GREEN10 = P2^1;   //上人行道绿灯
sbit RED11   = P2^2;
sbit YELLOW11= P2^3;
sbit GREEN11 = P2^4;

sbit RED00   = P3^0;	//右人行道红灯
sbit GREEN00 = P3^1;	//右人行道绿灯
sbit RED01   = P2^5;
sbit YELLOW01= P2^6;
sbit GREEN01 = P2^7;

//--定义全局变量--//
unsigned char code DIG_PLACE[8] = {
0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//位选控制   查表的方法控制
unsigned char code DIG_CODE[17] = {
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char DisplayData[8];
//用来存放要显示的8位数的值
unsigned char Time, Second;		  //用来存放定时时间

//--声明全局函数--//
void DigDisplay(); //动态显示函数
void Timer0Cofig(void);

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void main(void)
{
	Second = 1;

	Timer0Cofig();

	while(1)
	{
		if(Second == 70)
		{
			Second = 1;
		}

		//--宝田路通行，30秒--//
		if(Second < 31)
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = DIG_CODE[(30 - Second) % 100 / 10];
			DisplayData[3] = DIG_CODE[(30 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--宝田路通行--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			GREEN11 = 0;    //宝田路绿灯亮			
			GREEN10	= 0;    //宝田路人行道绿灯亮

			RED01 = 0;      //前进路红灯亮
			RED00 = 0;      //前进路人行道红灯亮
		}

		//--黄灯等待切换状态，5秒--//
		else if(Second < 36) 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = DIG_CODE[(35 - Second) % 100 / 10];
			DisplayData[3] = DIG_CODE[(35 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			YELLOW11 = 0;    //宝田路黄灯亮			
			RED10	= 0;     //宝田路人行道红灯亮

			YELLOW01 = 0;    //前进路红灯亮
			RED00 = 0;       //前进路人行道红灯亮
		}

		//--前进路通行--//
		else if(Second < 66) 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = DIG_CODE[(65 - Second) % 100 / 10];
			DisplayData[3] = DIG_CODE[(65 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			RED11 = 0;       //宝田路红灯亮			
			RED10 = 0;       //宝田路人行道红灯亮

			GREEN01 = 0;     //前进路绿灯亮
			GREEN00 = 0;     //前进路人行道绿灯亮
		}

		//--黄灯等待切换状态，5秒--//
		else 
		{
			DisplayData[0] = 0x00;
			DisplayData[1] = 0x00;
			DisplayData[2] = DIG_CODE[(70 - Second) % 100 / 10];
			DisplayData[3] = DIG_CODE[(70 - Second) %10];
			DisplayData[4] = 0x00;
			DisplayData[5] = 0x00;
			DisplayData[6] = DisplayData[2];
			DisplayData[7] = DisplayData[3];
			DigDisplay();

			//--黄灯阶段--//

			GPIO_TRAFFIC = 0xFF;  //将所有的灯熄灭
			RED00 = 1;
			GREEN00 = 1;

			YELLOW11 = 0;    //宝田路黄灯亮			
			RED10	= 0;     //宝田路人行道红灯亮

			YELLOW01 = 0;    //前进路红灯亮
			RED00 = 0;       //前进路人行道红灯亮
		}
	}				
}

/*******************************************************************************
* 函 数 名         : DigDisplay
* 函数功能		   : 使用数码管显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void DigDisplay()
{
	unsigned char i;
	unsigned int j;


	for(i=0; i<8; i++)
	{
		GPIO_PLACE = DIG_PLACE[i];	   //发送位选
		GPIO_DIG = DisplayData[i];     //发送段码
		j = 10;						   //扫描间隔时间设定
		while(j--);	
		GPIO_DIG = 0x00;               //消隐
	}
}

/*******************************************************************************
* 函 数 名         : Timer0Cofig
* 函数功能		   : 配置定时器
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Timer0Cofig(void)
{
	TMOD = 0x01;     //定时器0选择工作方式1
    TH0  = 0x3C;	 //设置初始值,定时50MS
    TL0  = 0xB0; 
    EA   = 1;	     //打开总中断
    ET0  = 1;		 //打开定时器0中断
    TR0  = 1;		 //启动定时器0	
}

/*******************************************************************************
* 函 数 名         : Timer0
* 函数功能		   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		   

void Timer0() interrupt 1
{
	TH0 = 0x3C;	 //设置初始值
	TL0 = 0xB0;
	Time++;
	if(Time == 20)
	{
		Second ++;
		Time = 0;
	}
}
