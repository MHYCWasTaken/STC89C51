/*-----------------------------------------------
  名称：18B20温度传感器
  公司：上海浩豚电子科技有限公司
  网站：www.doflye.cn
  编写：师访
  日期：2009.5
  修改：无
  内容：18B20单线温度检测的应用样例程序,请将18b20插紧，
        然后在数码管可以显示XX.XC，C表示摄氏度，如显示25.3C表示当前温度25.3度
        开发板使用11.0592M晶振
        串口测温软件 选择对应的com口 
------------------------------------------------*/
#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<math.h>
#include <stdio.h>
#include<INTRINS.H>

#define uchar unsigned char
#define uint   unsigned int;
/******************************************************************/
/*                    定义端口                                    */
/******************************************************************/
sbit seg1=P2^0;
sbit seg2=P2^1;
sbit seg3=P2^2;
sbit DQ=P1^3;//ds18b20 端口
sfr dataled=0x80;//显示数据端口
/******************************************************************/
/*                    全局变量                                    */
/******************************************************************/
uint temp;
uchar flag_get,count,num,minute,second;
uchar code tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
                                               //7段数码管段码表共阳
uchar  str[6];
/******************************************************************/
/*                   函数声明                                     */
/******************************************************************/
void delay1(uchar MS);
unsigned int ReadTemperature(void);
void Init_DS18B20(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
void delay(unsigned int i);

void UARTinit(void)
{
 
    SCON  = 0x50;		        /* SCON: 模式 1, 8-bit UART, 使能接收         */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 0xFD;               /* TH1:  reload value for 9600 baud @ 11.0592MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    //EA    = 1;                  /*打开总中断*/
    //ES    = 1;       
	TI    = 1; 
}
/******************************************************************/
/*                    主函数                                      */
/******************************************************************/
main()
{
unsigned int TempH,TempL;

TMOD|=0x01;//定时器设置
TH0=0xef;
TL0=0xf0;
IE=0x82;
TR0=1;
UARTinit();
P2=0x00;  
count=0;
while(1)
{
   str[5]=0x39;         //显示C符号
   str[1]=tab[TempH/100]; //十位温度
   str[2]=tab[(TempH%100)/10]; //十位温度
   str[3]=tab[(TempH%100)%10]|0x80; //个位温度,带小数点
   str[4]=tab[TempL];
  if(flag_get==1)       //定时读取当前温度
    {
  temp=ReadTemperature();
  if(temp&0x8000)
     {
     str[0]=0x40;//负号标志
     temp=~temp;  // 取反加1
	 temp +=1;
	 }
  else
     str[0]=0;
  TempH=temp>>4;
  TempL=temp&0x0F;
  TempL=TempL*6/10;//小数近似处理
  printf("%d.%d\n",TempH,TempL);
  flag_get=0;
    }
  }
}


/******************************************************************/
/*                  定时器中断                                    */
/******************************************************************/
void tim(void) interrupt 1 using 1//中断，用于数码管扫描和温度检测间隔
{
TH0=0xef;//定时器重装值
TL0=0xf0;
num++;
if (num==200)
    {num=0;
	  flag_get=1;//标志位有效
      second++;
       if(second>=60)
         {second=0;
           minute++;  
          }
      }
count++;
if(count==1)
   {P2=0;
    dataled=str[0];}//数码管扫描
if(count==2)
   {P2=1;
    dataled=str[1];}
if(count==3)
   { P2=2;
     dataled=str[2];
     }
if(count==4)
   { P2=3;
     dataled=str[3];
     }
if(count==5)
   { P2=4;
     dataled=str[4];
     }
if(count==6)
   { P2=5;
     dataled=str[5];
     count=0;}
}
/******************************************************************/
/*                    延时函数                                    */
/******************************************************************/
void delay(unsigned int i)//延时函数
{
 while(i--);
}
/******************************************************************/
/*                    初始化                                      */
/******************************************************************/
void Init_DS18B20(void)
{
 unsigned char x=0;
 DQ = 1;    //DQ复位
 delay(8);  //稍做延时
 DQ = 0;    //单片机将DQ拉低
 delay(80); //精确延时 大于 480us
 DQ = 1;    //拉高总线
 delay(10);
 x=DQ;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
 delay(5);
}

/******************************************************************/
/*                    读一个字节                                  */
/******************************************************************/
unsigned char ReadOneChar(void)
{
unsigned char i=0;
unsigned char dat = 0;
for (i=8;i>0;i--)
 {
  DQ = 0; // 给脉冲信号
  dat>>=1;
  DQ = 1; // 给脉冲信号
  if(DQ)
   dat|=0x80;
  delay(5);
 }
 return(dat);
}

/******************************************************************/
/*                 写一个字节                                     */
/******************************************************************/
void WriteOneChar(unsigned char dat)
{
 unsigned char i=0;
 for (i=8; i>0; i--)
 {
  DQ = 0;
  DQ = dat&0x01;
  delay(5);
  DQ = 1;
  dat>>=1;
 }
delay(5);
}

/******************************************************************/
/*                   读取温度                                     */
/******************************************************************/
unsigned int ReadTemperature(void)
{
unsigned char a=0;
unsigned int b=0;
unsigned int t=0;
Init_DS18B20();
WriteOneChar(0xCC); // 跳过读序号列号的操作
WriteOneChar(0x44); // 启动温度转换
delay(200);
Init_DS18B20();
WriteOneChar(0xCC); //跳过读序号列号的操作 
WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
a=ReadOneChar();   //低位
b=ReadOneChar();   //高位

b<<=8;
t=a+b;

return(t);
}
