/*------------------------------------------------
                包含头文件
------------------------------------------------*/
#include"reg52.h"
#include"tft.h"
/*------------------------------------------------
	             定义单片机引脚
------------------------------------------------*/
sbit CS=P2^2;		//片选
sbit RES=P2^1;		//复位
sbit RS=P2^4;		//数据/命令选择
sbit RW=P2^5;
#define DataPort P0                    //数据口使用DataPort
//unsigned char code pic[];

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
/*----------------------------------------------------------------
                            画点
输入参数：x，y 需要画点坐标
          color 点的颜色
----------------------------------------------------------------*/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(x,x,y,y);
	Write_Data_U16(color);
}


/*----------------------------------------------------------------
                           在屏幕上画线
输入参数：  起始坐标X0，Y0，终止坐标X1，Y1
			color 线颜色
----------------------------------------------------------------*/
void Line(	unsigned int X0,
			unsigned int Y0,
			unsigned int X1,
			unsigned int Y1,
			unsigned int color)
{
	int dx = X1 - X0;
	int dy = Y1 - Y0;
	int P  = 2 * dy - dx;
	int dobDy = 2 * dy;
	int dobD = 2 * (dy - dx);
	int PointX = 0,PointY = 0;
	int incx = 0,incy = 0;
	int distance = 0,xerr = 0,yerr = 0;
	unsigned int i = 0;

	if(dx == 0)		//k=1斜率为1
	{
		PointX = X0;
		if(Y0 < Y1)
		{
			PointY = Y0;
		}
		else
		{
			PointY = Y1;
		}
		for(i = 0;i <= ((Y0<Y1) ? (Y1-Y0) : (Y0-Y1));i++)
		{

				Put_pixel(PointX,PointY,color);
			PointY++;
		}
		return;
	}
	if(dy == 0)		//k=0斜率为0
	{
		PointY = Y0;
		if(X0 < X1)
		{
			PointX = X0;
		}
		else
		{
			PointX = X1;
		}
		for(i = 0;i <= ((X0<X1) ? (X1-X0) : (X0-X1));i++)
		{

				Put_pixel(PointX,PointY,color);
			PointX++;
		}
		return;
	}

	if(dx > 0)
		incx = 1;
	else if(dx == 0)
		incx = 0;
	else
		incx = -1;

	if(dy > 0)
		incy = 1;
	else if(dy == 0)
		incy = 0;
	else
		incy = -1;

    dx = ((X0>X1) ? (X0-X1) : (X1-X0));
    dy = ((Y0>Y1) ? (Y0-Y1) : (Y1-Y0));

    if(dx>dy) distance=dx;
    else distance=dy;

    PointX = X0;
    PointY = Y0;
    for(i=0;i<=distance+1;i++)
    {

    		Put_pixel(PointX,PointY,color);
        xerr+=dx;
        yerr+=dy;
        if(xerr>distance)
        {
            xerr-=distance;
            PointX+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            PointY+=incy;
        }
    }
}
/*---------------------------------------------------------------------------
                            绘制矩形框
输入参数：矩形的起始位置left,top 
 		  矩形的结束位置right,bottom 
 		 矩形框的颜色color

-----------------------------------------------------------------------------*/
void Rectangle( unsigned int left,
				unsigned int top,
				unsigned int right,
				unsigned int bottom,
				unsigned int color)
{
	Line(left,top,right,top,color);
	Line(left,top,left,bottom,color);
	Line(right,top,right,bottom,color);
	Line(left,bottom,right,bottom,color);
}
