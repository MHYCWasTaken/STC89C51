/******************************************************************
/*                                                                *
/* OK300C单片机开发系统演示程序 			          *
/*                                                                *
/* 版本： V2.0 (2008/6/20)                                       *
/* 作者： 高山流水 (Email: aoke999@sohu.com)                      *
/* 网站： www.aokemcu.cn(奥科电子)                                *
/*                                                                *
/*                                                                *
/******************************************************************/

#include <reg51.h>
#include <intrins.h>
	
#define uchar unsigned char
#define uint  unsigned int
/*TS12864A-3 端口定义*/
#define LCD_data  P0             //数据口
sbit LCD_RS  =  P3^5;            //寄存器选择输入 
sbit LCD_RW  =  P3^6;            //液晶读/写控制
sbit LCD_EN  =  P3^4;            //液晶使能控制
//sbit LCD_PSB =  P2^3;            //串/并方式控制
//sbit LCD_RST =  P2^5;            //液晶复位端口

sbit  IRIN  = P3^2;				//红外遥控
sbit  BEEP  = P2^3;				//蜂鸣器
//sbit  RELAY = P2^1;			    //继电器

uchar code dis1[] = {"红外遥控码读取器"};
uchar code dis2[] = {" www.aokemcu.cn "};
uchar code dis3[] = {"键值编码：- - H "};
uchar code dis4[] = {"键值反码：- - H "};

#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
uchar IRDIS[2];
uchar IRCOM[4];
void delay0(uchar x);  //x*0.14MS
void beep();
void  dataconv();
void lcd_pos(uchar X,uchar Y);  //确定显示位置
/*******************************************************************/
/*                                                                 */
/*  延时函数                                                       */
/*                                                                 */
/*******************************************************************/
void delay(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}		
/*******************************************************************/
/*                                                                 */
/*检查LCD忙状态                                                    */
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */
/*                                                                 */
/*******************************************************************/
bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    delayNOP();
    result = (bit)(P0&0x80);
    LCD_EN = 0;
    return(result); 
 }
/*******************************************************************/
/*                                                                 */
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(uchar cmd)
{                          
   while(lcd_busy());
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    P0 = cmd;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   while(lcd_busy());
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    P0 = dat;
    delayNOP();
    LCD_EN = 1;
    delayNOP();
    LCD_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void lcd_init()
{ 
    
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
    lcd_wcmd(0x0C);      //显示开，关光标
    delay(5);
    lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
}
/*********************************************************/
/*														 */
/* 主程序           									 */
/*                                                       */
/*********************************************************/
  main()
 {
    uchar i;
    IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
    TCON = 0x01;               //触发方式为脉冲负边沿触发
    IRIN=1;                    //I/O口初始化
    BEEP=1;
	P0=0;
	P2&=0x1F;

    delay(10);                 //延时
    lcd_init();                //初始化LCD             
        
    lcd_pos(0,0);             //设置显示位置为第一行的第1个字符
     i = 0;
    while(dis1[i] != '\0')
     {                         //显示字符
       lcd_wdat(dis1[i]);
       i++;
     }
    lcd_pos(1,0);             //设置显示位置为第二行的第1个字符
     i = 0;
    while(dis2[i] != '\0')
     {
       lcd_wdat(dis2[i]);      //显示字符
       i++;
     }
 	 lcd_pos(2,0);             //设置显示位置为第三行的第1个字符
     i = 0;
    while(dis3[i] != '\0')
     {
       lcd_wdat(dis3[i]);      //显示字符
       i++;
     }
	 lcd_pos(3,0);             //设置显示位置为第四行的第1个字符
     i = 0;
    while(dis4[i] != '\0')
     {
       lcd_wdat(dis4[i]);      //显示字符
       i++;
     }
  
}
/*********************************************************/
/*														 */
/* INT0外部中断子程序									 */
/*                                                       */
/*********************************************************/
void IR_IN() interrupt 0 using 0   
{
  uchar j,k,N=0;
     EX0 = 0;   
	 delay0(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                         //确认IR信号出现
  while (!IRIN)           //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay0(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
    while (IRIN);       //等IR变为低电平，跳过4.5ms的前导高电平信号。
    while (!IRIN);      //等IR变为高电平
    while (IRIN)        //计算IR高电平时长
      {
        delay0(1);
        N++;           
       if (N>=30)
	    { EX0=1;
	      return;}            //0.14ms计数过长自动离开。
      }                       //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
   }//end for k
 }//end for j
     if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

     IRDIS[0]=IRCOM[2]&0x0F;     //取键码的低四位
     IRDIS[1]=IRCOM[2]>>4;  //取键码的高四位,右移4次，高四位变为低四位
	 dataconv();
     lcd_pos(2,5);             
     lcd_wdat(IRDIS[1]);        //第一位数显示 
     lcd_pos(2,6);             
     lcd_wdat(IRDIS[0]);        //第二位数显示

	 IRDIS[0]=IRCOM[3]&0x0F;     //取键码的低四位
     IRDIS[1]=IRCOM[3]>>4;  //取键码的高四位,右移4次，高四位变为低四位
     dataconv();
     lcd_pos(3,5);             
     lcd_wdat(IRDIS[1]);        //第一位数显示 
     lcd_pos(3,6);             
     lcd_wdat(IRDIS[0]);        //第二位数显示
     beep();
     EX0 = 1; 
}
/*********************************************************/
/*														 */
/* 转换为ASCII码										 */
/*                                                       */
/*********************************************************/
void  dataconv()	 
  {
     uchar  t; 
	 for (t=0;t<2;t++)   
     if(IRDIS[t]>9)
      IRDIS[t]=IRDIS[t]+0x37;
     else
	  IRDIS[t]=IRDIS[t]+0x30;
  }
/*********************************************************/
/*														 */
/* 蜂鸣器响一声子程序									 */
/*                                                       */
/*********************************************************/
void beep()
{
  uchar i;
  for (i=0;i<180;i++)
   {
     delay0(5);
     BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}
/*********************************************************/
/*														 */
/* 延时x*0.14ms子程序									 */
/*                                                       */
/*********************************************************/

void delay0(uchar x)    //x*0.14MS
{
  uchar i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}
/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     //显示地址
}