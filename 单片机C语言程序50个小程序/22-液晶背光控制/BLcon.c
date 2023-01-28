
/*1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK*/

/*-----------------------------------------------
  名称：LCD1602
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：通过标准程序静态显示2行字符
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include<stdio.h> //标准输入输出
#include<intrins.h>


sbit RS = P2^4; //Pin4
sbit RW = P2^5; //Pin5
sbit E  = P2^6; //Pin6

sbit BLCtrl = P2^7;//背光控制端口
sbit KEY    = P3^3;//按键端口

#define Data  P0   //数据端口

char data TimeNum[]="                ";
char data Test1[]="                  ";
/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/
/******************************************************************/
/*                    微秒延时函数                                */
/******************************************************************/
void DelayUs(unsigned char us)//delay us
{
 unsigned char uscnt;
 uscnt=us>>1;        /*12MHz频率*/
 while(--uscnt);
}
/******************************************************************/
/*                    毫秒函数声明                                */
/******************************************************************/
void DelayMs(unsigned char ms)
{
 while(--ms)
   {
     DelayUs(250);
     DelayUs(250);
	 DelayUs(250);
	 DelayUs(250);
   }
}
/******************************************************************/
/*                   写入命令函数                                 */
/******************************************************************/
void WriteCommand(unsigned char c)
{
 DelayMs(5);//操作前短暂延时，保证信号稳定
 E=0;
 RS=0;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
}
/******************************************************************/
/*                   写入数据函数                                 */
/******************************************************************/
void WriteData(unsigned char c)
{
 DelayMs(5);  //操作前短暂延时，保证信号稳定
 E=0;
 RS=1;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
 RS=0;
}
/******************************************************************/
/*                   写入字节函数                                 */
/******************************************************************/
void ShowChar(unsigned char pos,unsigned char c)
{
 unsigned char p;
 if (pos>=0x10)
    p=pos+0xb0; //是第二行则命令代码高4位为0xc
 else 
    p=pos+0x80; //是第二行则命令代码高4位为0x8
 WriteCommand (p);//写命令
 WriteData (c);   //写数据
}
/******************************************************************/
/*                   写入字符串函数                               */
/******************************************************************/
void ShowString (unsigned char line,char *ptr)
{
 unsigned char l,i;
 l=line<<4;
 for (i=0;i<16;i++)
  ShowChar (l++,*(ptr+i));//循环显示16个字符
}
/******************************************************************/
/*                   初始化函数                                   */
/******************************************************************/
void InitLcd()
{
 DelayMs(15);
 WriteCommand(0x38); //display mode
 WriteCommand(0x38); //display mode
 WriteCommand(0x38); //display mode
 WriteCommand(0x06); //显示光标移动位置
 WriteCommand(0x0c); //显示开及光标设置
 WriteCommand(0x01); //显示清屏

}

void main(void)

{


InitLcd();//初始化液晶
DelayMs(15);

sprintf(Test1," www.haotun.com ");  //第一行
ShowString(0,Test1);

sprintf(TimeNum,"LCM1602 Test ok!");//第二行
ShowString(1,TimeNum);

while(1){
if(!KEY)
  {
  DelayMs(10);   //按下去抖 
  while(!KEY);
  if(KEY)
    {
     DelayMs(10);//释放去抖
	 if(KEY)
     BLCtrl=!BLCtrl; //背光关掉或者打开
    }
 }
}

}


