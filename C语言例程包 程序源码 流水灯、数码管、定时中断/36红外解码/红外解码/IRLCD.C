
/********************************************************************************
*  描述:                                                                       *
*                                                                   *
********************************************************************************/

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

uchar LCD_ID_1[16] = {"Red Control"};
uchar LCD_ID_2[16] = {"IR CODE:--H"};

void delay(uchar x);  //x*0.14MS
void delay1(int ms);
void beep();

sbit IRIN = P3^3;         //红外接收器数据线
sbit BEEP = P1^5;         //蜂鸣器驱动线
sbit RELAY= P1^4;         //继电器驱动线

uchar IRCOM[7];

sbit E=P2^7;		//1602使能引脚
sbit RW=P2^6;		//1602读写引脚	
sbit RS=P2^5;		//1602数据/命令选择引脚


/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间大概为140US。
* 输入 : 无
* 输出 : 无
***********************************************************************/

void Delay2()
{
	int i,j;
	for(i=0; i<=10; i++)
	for(j=0; j<=2; j++)
;
}


/********************************************************************
* 名称 : enable(uchar del)
* 功能 : 1602命令函数
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/

void enable(uchar del)
{
	P0 = del;
	RS = 0;
	RW = 0;
	E = 0;
	Delay2();
	E = 1;
	Delay2();
}

/********************************************************************
* 名称 : write(uchar del)
* 功能 : 1602写数据函数
* 输入 : 需要写入1602的数据
* 输出 : 无
***********************************************************************/

void write(uchar del)
{
	P0 = del;
	RS = 1;
	RW = 0;
	E = 0;
	Delay2();
	E = 1;
	Delay2();
}

/********************************************************************
* 名称 : L1602_init()
* 功能 : 1602初始化，请参考1602的资料
* 输入 : 无
* 输出 : 无
***********************************************************************/
void L1602_init(void)
{
	enable(0x01);
	enable(0x38);
	enable(0x0c);
	enable(0x06);
	enable(0xd0);
}

/********************************************************************
* 名称 : L1602_char(uchar hang,uchar lie,char sign)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，调用该函数如下
		 L1602_char(1,5,'b')
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_char(uchar hang,uchar lie,char sign)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	enable(a);
	write(sign);
}

/********************************************************************
* 名称 : L1602_string(uchar hang,uchar lie,uchar *p)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符开始显示"ab cd ef" ，调用该函数如下
	 	 L1602_string(1,5,"ab cd ef;")
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_string(uchar hang,uchar lie,uchar *p)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	enable(a);
	while(1)
	{
		if(*p == '\0') break;
		write(*p);
		p++;
	}
}

/*******************************************************************/
main()
{
 
    IE = 0x84;                 //允许总中断中断,使能 INT1 外部中断
    TCON = 0x10;               //触发方式为脉冲负边沿触发
    
    IRIN=1;                    //I/O口初始化
    BEEP=1;
    RELAY=1; 
	
    delay1(10);                 //延时
    L1602_init();               //初始化LCD             
	L1602_string(1, 1, LCD_ID_1);
	L1602_string(2, 1, LCD_ID_2);
   while(1)	;

} //end main
/**********************************************************/
void IR_IN() interrupt 2 using 0
{
  unsigned char j,k,N=0;
     EX1 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX1 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {delay(1);}
    while (!IRIN)          //等 IR 变为高电平
     {delay(1);}
     while (IRIN)           //计算IR高电平时长
      {
    delay(1);
    N++;           
    if (N>=30)
	 { EX1=1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX1=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;
//////////////////////////
L1602_char(2,10,IRCOM[5]);
L1602_char(2,9,IRCOM[6]);
     //beep();
     EX1 = 1; 
} 

/**********************************************************/
void beep()
{
  unsigned char i;
  for (i=0;i<100;i++)
   {
   delay(4);
   BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}
/**********************************************************/
void delay(unsigned char x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}

/**********************************************************/
void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}


