#include <REG51.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
//延时子程序模块
//**********************************************
void mdelay(uint delay)
{	uint i;
 	for(;delay>0;delay--)
   		{for(i=0;i<80;i++) //1ms延时.
       		{;}
   		}
}

//************************************************

void show();   //液晶显示程序
//****************************************
//12864液晶显示部分子程序模块
//****************************************
sbit rs		= P3^0;
sbit rw		= P3^1;
sbit e  	= P3^2;
#define lcddata P0
sbit busy=P0^7;   //lcd busy bit
void wr_d_lcd(uchar content);
void wr_i_lcd(uchar content);
void clrram_lcd (void);
void init_lcd(void);
void busy_lcd(void);
void rev_row_lcd(uchar row);
void rev_co_lcd(uchar row,uchar col,uchar mode);
void clr_lcd(void);
void wr_co_lcd(uchar row,uchar col,uchar lcddata1,uchar lcddtta2);
void wr_row_lcd(uchar row,char *p);
//**********************************
//液晶初始化
//**********************************
void init_lcd(void)
{
	wr_i_lcd(0x06);  /*光标的移动方向*/
	wr_i_lcd(0x0c);  /*开显示，关游标*/
}
//***********************************
//填充液晶DDRAM全为空格
//**********************************
void clrram_lcd (void)
{
	wr_i_lcd(0x30);
	wr_i_lcd(0x01);
}
//***********************************
//对液晶写数据
//content为要写入的数据
//***********************************
void wr_d_lcd(uchar content)
{
	busy_lcd();
	rs=1;
    rw=0;
	lcddata=content;
	e=1;
	;
	e=0;
}
//********************************
//对液晶写指令
//content为要写入的指令代码
//*****************************
void wr_i_lcd(uchar content)
{
	busy_lcd();
	rs=0;
    rw=0;
	lcddata=content;
	e=1;
	;
	e=0;
}
//********************************
//液晶检测忙状态
//在写入之前必须执行
//********************************
void busy_lcd(void)
{
  lcddata=0xff;
  rs=0;
  rw=1;
  e =1;
  while(busy==1);
  e =0;
}
//********************************
//指定要显示字符的坐标
//*******************************
void gotoxy(unsigned char y, unsigned char x)
{
	if(y==1)
		wr_i_lcd(0x80|x);
	if(y==2)
        wr_i_lcd(0x90|x);
	if(y==3)
		wr_i_lcd((0x80|x)+8);
	if(y==4)
        wr_i_lcd((0x90|x)+8);
}
//**********************************
//液晶显示字符串程序
//**********************************
void print(uchar *str)
{
	while(*str!='\0')
	{
		wr_d_lcd(*str);
		str++;
	}
}

//***************************************
//液晶显示主程序模块
//***************************************
void show()   
{
  
  gotoxy(1,0);
  print("欢迎光临锐志电子");
    mdelay(200);             
  gotoxy(2,0);
  print("【锐志电子】");
  mdelay(200);                 //扫描延时

gotoxy(3,0);
  print("WWW.RICHMCU.COM");
  mdelay(200);                 //扫描延时
gotoxy(4,0);
  print("12864 液晶测试");
  mdelay(200);                 //扫描延时
}


//************************************
//主程序
//*************************************
main()
{
  
	init_lcd();
	clrram_lcd();
	
	while(1)
	{   
        
            show();                //液晶显示数据
          			
 		}		
	
}




