/*------------------------------------------------
                包含头文件
------------------------------------------------*/
#include"reg52.h"

void TFT_Initial(void);
void Write_Cmd_Data(unsigned char x, unsigned int y);
void Write_Cmd(unsigned char DH,unsigned char DL);
void delayms(unsigned int tt);
void Write_Data_U16(unsigned int y);
void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
void ClearScreen(unsigned int bColor);
void LCD_Write_Data(unsigned char u);

/*------------------------------------------------
	             定义单片机引脚
------------------------------------------------*/
sbit CS=P2^2;		//片选
sbit RES=P2^1;		//复位
sbit RS=P2^4;		//数据/命令选择
sbit RW=P2^5;
#define DataPort P0                    //数据口使用DataPort

#define X_MAX 240
#define Y_MAX 400
/*------------------------------------------------
             		清屏函数
------------------------------------------------*/
void ClearScreen(unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(0,X_MAX-1,0,Y_MAX-1);//
 for (i=0;i<Y_MAX;i++)
	{
	
	   for (j=0;j<X_MAX;j++)
	       Write_Data_U16(bColor);

	}
}




/*------------------------------------------------
              写16位数据函数
------------------------------------------------*/
void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	LCD_Write_Data(m);
    LCD_Write_Data(n);

}
/*------------------------------------------------
                写指令函数
------------------------------------------------*/
void LCD_Write_Command(unsigned char u)
{
  	
	CS=0;
	RS=0;
	DataPort=u;	
	RW=0;
	RW=1;
	CS=1;
}

/*------------------------------------------------
                 写数据函数
------------------------------------------------*/
void LCD_Write_Data(unsigned char u)
{	
	CS=0;
	RS=1;
	DataPort=u;	
	RW=0;
	RW=1;
	CS=1;
}

WriteCom(a,b)
{
LCD_Write_Command(a);
LCD_Write_Command(b);
}

WriteData(a,b)
{
LCD_Write_Data(a);
LCD_Write_Data(b);
}
/*------------------------------------------------
                延时函数
------------------------------------------------*/
void delayms(unsigned int count)
{
    int i,j;                                                                                
    for(i=0;i<count;i++)                                                                    
       {
	     for(j=0;j<260;j++);
       }                                                                                     
}


/*------------------------------------------------
             	液晶初始化代码
------------------------------------------------*/

void TFT_Initial(void)
{ 
// VCI=2.80V
//************* Reset LCD Driver ****************//
RES = 1;
delayms(1); // delaymsms 1ms
RES = 0;
delayms(10); // delaymsms 10ms // This delaymsms time is necessary
RES = 1;
delayms(50); // delaymsms 50 ms
 // Synchronization after reset//////////
////////////////////////////////////////////
LCD_Write_Command(0x11); //Exit Sleep
delayms(100);
LCD_Write_Command(0xD1);
LCD_Write_Data (0x00);
LCD_Write_Data (0x71);
LCD_Write_Data (0x19);
LCD_Write_Command(0xD0);
LCD_Write_Data (0x07);
LCD_Write_Data (0x01);
LCD_Write_Data (0x08);
LCD_Write_Command(0x36);
LCD_Write_Data (0x48);
LCD_Write_Command(0x3A);
LCD_Write_Data (0x05);
LCD_Write_Command(0xC1);
LCD_Write_Data (0x10);
LCD_Write_Data (0x10);
LCD_Write_Data (0x02);
LCD_Write_Data (0x02);
LCD_Write_Command(0xC0); //Set Default Gamma
LCD_Write_Data (0x00);
LCD_Write_Data (0x35);
LCD_Write_Data (0x00);
LCD_Write_Data (0x00);
LCD_Write_Data (0x01);
LCD_Write_Data (0x02);
LCD_Write_Command(0xC5); //Set frame rate
LCD_Write_Data (0x04);
LCD_Write_Command(0xD2); //power setting
LCD_Write_Data (0x01);
LCD_Write_Data (0x44);
LCD_Write_Command(0xC8); //Set Gamma
LCD_Write_Data (0x04);
LCD_Write_Data (0x67);
LCD_Write_Data (0x35);
LCD_Write_Data (0x04);
LCD_Write_Data (0x08);
LCD_Write_Data (0x06);
LCD_Write_Data (0x24);
LCD_Write_Data (0x01);
LCD_Write_Data (0x37);
LCD_Write_Data (0x40);
LCD_Write_Data (0x03);
LCD_Write_Data (0x10);
LCD_Write_Data (0x08);
LCD_Write_Data (0x80);
LCD_Write_Data (0x00);
LCD_Write_Command(0x2A); 
LCD_Write_Data (0x00);
LCD_Write_Data (0x00);
LCD_Write_Data (X_MAX>>8);
LCD_Write_Data (X_MAX);
LCD_Write_Command(0x2B); 
LCD_Write_Data (0x00);
LCD_Write_Data (0x00);
LCD_Write_Data (Y_MAX>>8);
LCD_Write_Data (Y_MAX);
LCD_Write_Command(0x29); //display on      
LCD_Write_Command(0x2C); //display on
delayms(100);

}

/*------------------------------------------------
函数名称:LCD_DefineDispWindow
功    能:定义显示窗体
参    数:x0:  窗体中X坐标中较小者
	 x1:  窗体中X坐标中较大者
	 y0:  窗体中Y坐标中较小者
	 y1:  窗体中Y坐标中较大者 
返 回 值:无
------------------------------------------------*/
static void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
{

WriteCom(0x00,0x2A);
WriteData(x0>>8,x0);
WriteData(x1>>8,x1);
WriteCom(0x00,0x2B);
WriteData(y0>>8,y0);
WriteData(y1>>8,y1);
WriteCom(0x00,0x2C);


}
void Write_Data(unsigned char DH,unsigned char DL)
{
 
  
	
	CS=0;
	
	RS=1;
	P0=DH;
	RW=0;
    RW=1;

	P0=DL;	
	RW=0;
	RW=1;
	CS=1;
}



