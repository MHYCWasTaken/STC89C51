#ifndef __TFT_H__
#define __TFT_H__

#define X_MAX 240
#define Y_MAX 400

/*------------------------------------------------
             		清屏函数
------------------------------------------------*/
void ClearScreen(unsigned int bColor);

/*------------------------------------------------
             写8x16字符函数
------------------------------------------------*/

void LCD_PutChar8x16(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);

/*------------------------------------------------
              写字符串函数
------------------------------------------------*/
void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);

/*------------------------------------------------
             写16x16汉字函数
------------------------------------------------*/


void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor);
/*------------------------------------------------
                写字符串函数
------------------------------------------------*/
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) ;
/*------------------------------------------------
              指定区域显示指定RGB颜色
------------------------------------------------*/
void Show_RGB (unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,unsigned int Color);

/*------------------------------------------------
                显示彩条
------------------------------------------------*/
void show_colour_bar (void);

/*------------------------------------------------
              写16位数据函数
------------------------------------------------*/
void  Write_Data_U16(unsigned int y);
/*------------------------------------------------
                写指令函数
------------------------------------------------*/
void LCD_Write_Command(unsigned char u);
/*------------------------------------------------
                 写数据函数
------------------------------------------------*/
void LCD_Write_Data(unsigned char u);
WriteCom(a,b);

WriteData(a,b);
/*------------------------------------------------
                延时函数
------------------------------------------------*/
void delayms(unsigned int count);
/*------------------------------------------------
             	液晶初始化代码
------------------------------------------------*/

void TFT_Initial(void);
/*------------------------------------------------
函数名称:LCD_DefineDispWindow
功    能:定义显示窗体
参    数:x0:  窗体中X坐标中较小者
	 x1:  窗体中X坐标中较大者
	 y0:  窗体中Y坐标中较小者
	 y1:  窗体中Y坐标中较大者 
返 回 值:无
------------------------------------------------*/
static void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
/*----------------------------------------------------------------
                            画点
输入参数：x，y 需要画点坐标
          color 点的颜色
----------------------------------------------------------------*/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color);

/*----------------------------------------------------------------
                           在屏幕上画线
输入参数：  起始坐标X0，Y0，终止坐标X1，Y1
			color 线颜色
----------------------------------------------------------------*/
void Line(	unsigned int X0,
			unsigned int Y0,
			unsigned int X1,
			unsigned int Y1,
			unsigned int color);
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
				unsigned int color);
				
#endif
