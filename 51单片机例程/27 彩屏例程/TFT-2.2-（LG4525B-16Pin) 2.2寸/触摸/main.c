#include "REG51.H"
//#include "lcd_driver.h"
#include "touch.h"
#include "gui.h"
#include<stdio.h>

/****************************************************************************
*函数名：main
*输  入：无
*输  出：无
*功  能：主函数
****************************************************************************/

void main (void)
{
	uchar rst = 0;
	//--多出来两个值使用来在内存上面跟别的变量分隔的--//
	uchar  xValue[6] = {0, 0, 0, 0, 0, 0}, yValue[6] = {0, 0, 0, 0, 0, 0};
	long x, y;

	TFT_Init();
	rst = 1;

	while(1)
	{
		if(rst == 1)
		{	
			TFT_ClearScreen(0x0000);
			GUI_Write32CnChar(20, 24, "普中科技", 0xF800, 0x0000);
			GUI_Line(20, 54, 140, 54, 0xF800);
			GUI_Line(20, 54, 140, 54, 0xF800);
			GUI_WriteASCII(127, 0, "RST", 0xF800, 0x0000);
			rst = 0;
		}

		if(TOUCH_XPT_ReadXY() == 1)
		{
			//--如果触摸跟显示发生偏移，可以根据显示AD值--//
			//--调整下面公式里面的数值--//
			x = xpt_xy.x;
			x = (x - 304)*175/3422; 
			y =	xpt_xy.y;
			y = (y - 256) *219/3340;



			if (x > 175 )
			{
				x = 174;
			}
			if (y > 219)
			{
				y = 218;
			}
			if ((x > 127) && (y < 24))
			{
				rst = 1;
			}
			else
			{
				GUI_Dot((uint)x , (uint)y, 0x001F);
				//--计算读取到的AD值--//
				//--由于添加了显示AD值，计算需要时间，所以触摸有一点延迟--//xpt_xy.
				xValue[1] = (xpt_xy.x % 10000 /1000) + '0';
				xValue[2] = (xpt_xy.x % 1000 /100) + '0';
				xValue[3] = (xpt_xy.x % 100 /10) + '0';
				xValue[4] = (xpt_xy.x % 10) + '0';

				yValue[1] = (xpt_xy.y % 10000 /1000) + '0';
				yValue[2] = (xpt_xy.y % 1000 /100) + '0';
				yValue[3] = (xpt_xy.y % 100 /10) + '0';
				yValue[4] = (xpt_xy.y % 10) + '0';

				//--显示AD值--//
				GUI_WriteASCII(0, 170, "X:", 0xF800, 0x0000);
				GUI_WriteASCII(32, 170, &xValue[1], 0xF800, 0x0000);	
				GUI_WriteASCII(0, 195, "Y:", 0xF800, 0x0000);
				GUI_WriteASCII(32, 195, &yValue[1], 0xF800, 0x0000);
			}
		}
	}
}

	