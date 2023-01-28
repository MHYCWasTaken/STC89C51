/*******************************************************************************
* 实验名			  : EEPROM实验
* 使用的IO	    : 
* 实验效果      : 按K1保存显示的数据，按K2读取上次保存的数据，按K3显示数据加一，
*按K4显示数据清零。
*	注意					：由于P3.2口跟红外线共用，所以做按键实验时为了不让红外线影响实验效果，最好把红外线先
*取下来。
*
*********************************************************************************/
#include<reg51.h>
#include"i2c.h"

//--定义使用的IO口--//
//数码管IO
#define GPIO_DIG	P0
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
//按键IO
sbit K1=P3^0;
sbit K2=P3^1;
sbit K3=P3^2;
sbit K4=P3^3;

//--定义全局变量--//
unsigned char code GPIO_DIG_CODE[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char Num=0;
unsigned int DisplayData[8] = {0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};

//--声明全局函数--//
void At24c02Write(unsigned char ,unsigned char );
unsigned char At24c02Read(unsigned char );
void Delay10ms(unsigned int c);   //误差 0us
void Timer0Configuration();

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void main()
{
	unsigned int num0 = 0,num1 = 0,n;

	Timer0Configuration();

	while(1)
	{
		if(K1 == 0)		 //按键1按下
		{
			Delay10ms(1);
			if(K1 == 0)
				At24c02Write(2,num0);
			while((n < 50)&&(K3==0))
			{
				n++;
				Delay10ms(1);	
			}			
			n=0;
			n=0;
		}
		
		if(K2 == 0)		 //按键2按下
		{
			Delay10ms(1);
			if(K2 == 0)
				num1 = At24c02Read(2);
			while((n < 50)&&(K2 == 0))
			{
				n++;
				Delay10ms(1);	
			}			
			n=0;
		}			
		if(K3 == 0)		 //按键3按下
		{
			Delay10ms(1);
			if(K3 == 0)
				num0++;
			while((n < 50)&&(K3 == 0))
			{
				n++;
				Delay10ms(1);	
			}			
			n=0;
			if(num0==256)
				num0=0;
		}
			
		if(K4 == 0)		 //按键4按下
		{
			Delay10ms(1);
			if(K4 == 0)
				num0 = 0;
			while((n < 50) && (K4 == 0))
			{
				n++;
				Delay10ms(1);	
			}			
			n=0;
		}
					
		DisplayData[0] = GPIO_DIG_CODE[num1/1000];//千位
		DisplayData[1] = GPIO_DIG_CODE[num1%1000/100];//百位
		DisplayData[2] = GPIO_DIG_CODE[num1%1000%100/10];//十位
		DisplayData[3] = GPIO_DIG_CODE[num1%1000%100%10];//个位
		DisplayData[4] = GPIO_DIG_CODE[num0/1000];//千位
		DisplayData[5] = GPIO_DIG_CODE[num0%1000/100];//百位
		DisplayData[6] = GPIO_DIG_CODE[num0%1000%100/10];//十位
		DisplayData[7] = GPIO_DIG_CODE[num0%1000%100%10];	
	}
}									 
/*******************************************************************************
* 函 数 名         : Timer0Configuration()
* 函数功能		   : 设置计时器
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Timer0Configuration()
{
	TMOD=0X02;  //选择为定时器模式，工作方式2，仅用TRX打开启动。

	TH0=0X9C;	//给定时器赋初值，定时100us
	TL0=0X9C;	
	ET0=1;      //打开定时器0中断允许
	EA=1;       //打开总中断
	TR0=1;      //打开定时器		
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

/*******************************************************************************
* 函 数 名         : void At24c02Write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2C_Start();
	I2C_SendByte(0xa0, 1);//发送写器件地址
	I2C_SendByte(addr, 1);//发送要写入内存地址
	I2C_SendByte(dat, 0);	//发送数据
	I2C_Stop();
}
/*******************************************************************************
* 函 数 名         : unsigned char At24c02Read(unsigned char addr)
* 函数功能		   : 读取24c02的一个地址的一个数据
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	I2C_Start();
	I2C_SendByte(0xa0, 1); //发送写器件地址
	I2C_SendByte(addr, 1); //发送要读取的地址
	I2C_Start();
	I2C_SendByte(0xa1, 1); //发送读器件地址
	num=I2C_ReadByte(); //读取数据
	I2C_Stop();
	return num;	
}
/*******************************************************************************
* 函 数 名         : GPIO_DIGDisplayDatalay() interrupt 1
* 函数功能		   : 中断数码管显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void GPIO_DIGDisplayDatalay() interrupt 1
{
//定时器在工作方式二会自动重装初，所以不用在赋值。
//	TH0=0X9c;//给定时器赋初值，定时1ms
//	TL0=0X00;		
	GPIO_DIG=0; //消隐
	switch(Num)	 //位选，选择点亮的数码管，
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
	GPIO_DIG = DisplayData[Num]; //段选，选择显示的数字。
	Num++;
	if(Num>7)
	{
		Num=0;
	}
	
}