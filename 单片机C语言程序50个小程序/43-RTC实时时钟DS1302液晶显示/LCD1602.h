#include<reg52.h>
#include<stdio.h>
#include<intrins.h>

sbit RS = P2^4;//Pin4
sbit RW = P2^5; //Pin5
sbit E  = P2^6;//Pin6

#define Data  P0    //数据端口

unsigned char TempData[8];

unsigned char  SecondLine[]="                ";
unsigned char  FirstLine[] ="                ";
/******************************************************************/
/*                    函数声明                                    */
/******************************************************************/
void DelayUs(unsigned char us)//delay us
{
 unsigned char uscnt;
 uscnt=us>>1;/* Crystal frequency in 12MHz*/
 while(--uscnt);
}
/******************************************************************/
void DelayMs(unsigned char ms)//delay Ms
{
 while(--ms)
   {
     DelayUs(250);
     DelayUs(250);
	 DelayUs(250);
	 DelayUs(250);
   }
}
void WriteCommand(unsigned char c)
{
 DelayMs(5);//short delay before operation
 E=0;
 RS=0;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
}
/****************************************************************/
void WriteData(unsigned char c)
{
 DelayMs(5);   //short delay before operation
 E=0;
 RS=1;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
 RS=0;
}
/*********************************************************************/
void ShowChar(unsigned char pos,unsigned char c)
{
 unsigned char p;
 if (pos>=0x10)
    p=pos+0xb0; //是第二行则命令代码高4位为0xc
 else 
    p=pos+0x80; //是第二行则命令代码高4位为0x8
 WriteCommand (p);//write command
 WriteData (c);   //write data
}
/*************************************************************************/
void ShowString (unsigned char line,char *ptr)
{
 unsigned char l,i;
 l=line<<4;
 for (i=0;i<16;i++)
  ShowChar (l++,*(ptr+i));//循环显示16个字符
}
/*********************************************************************/
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


/////////////////////////////////////////////////////////////////
void disp(void)

{

 //年-月-日 ^周^

 FirstLine[0]='2';
 FirstLine[1]='0';
 FirstLine[2]='0'+TempData[1]/10;
 FirstLine[3]='0'+TempData[1]%10;
 FirstLine[4]='-';
 FirstLine[5]='0'+TempData[2]/10;
 FirstLine[6]='0'+TempData[2]%10;
 FirstLine[7]='-';
 FirstLine[8]='0'+TempData[3]/10;
 FirstLine[9]='0'+TempData[3]%10;

 FirstLine[15]='0'+TempData[7]%10;



 
 SecondLine[0]='0'+TempData[4]/10;
 SecondLine[1]='0'+TempData[4]%10;
 SecondLine[2]=':';
 SecondLine[3]='0'+TempData[5]/10;
 SecondLine[4]='0'+TempData[5]%10;
 SecondLine[5]=':';
 SecondLine[6]='0'+TempData[6]/10;
 SecondLine[7]='0'+TempData[6]%10;

ShowString(0,FirstLine);

//时:分:秒
ShowString(1,SecondLine);



}