/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : RS232串口试验
* 实验说明       : 单片机将串口接收到的数据以10进制显示在数码管上。
* 连接方式       : 见连接图
* 注    意		 : 
*******************************************************************************/

#include<reg51.h>

//--定义使用的IO--//
#define GPIO_DIG P0

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

//--定义全局变量--//
unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char DisplayData[8];
//用来存放要显示的8位数的值

//--声明全局函数--//
void DigDisplay();         //动态显示函数	   
void UsartConfiguration();
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void main()
{
	unsigned char receiveData;

	UsartConfiguration();
	while(1)
	{
		if(RI == 1)				//查看是否接收到数据
		{
			receiveData = SBUF;	//读取数据
			RI = 0;				//清除标志位

			//--因为一次接受只能接收到8位数据，最大为255，所以只用显示百位之后--//
			DisplayData[0] = 0x00;
			DisplayData[1] = DIG_CODE[receiveData / 100];      // 百位
			DisplayData[2] = DIG_CODE[receiveData % 100 / 10]; // 十位
			DisplayData[3] = DIG_CODE[receiveData % 10];	   // 个位
			DisplayData[4] = 0x00;
			DisplayData[5] = DIG_CODE[receiveData / 100];      // 百位
			DisplayData[6] = DIG_CODE[receiveData % 100 / 10]; // 十位
			DisplayData[7] = DIG_CODE[receiveData % 10];
		}
		DigDisplay();
	}
}
/*******************************************************************************
* 函 数 名         :UsartConfiguration()
* 函数功能		   :设置串口
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void UsartConfiguration()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;		    //计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	TR1=1;					    //打开计数器
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
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		GPIO_DIG=DisplayData[i];//发送段码
		j=10;						 //扫描间隔时间设定
		while(j--);	
		GPIO_DIG=0x00;//消隐
	}
}