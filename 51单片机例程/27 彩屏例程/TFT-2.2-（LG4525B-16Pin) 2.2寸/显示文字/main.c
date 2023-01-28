#include "REG51.H"
#include "lcd_driver.h"
#include "gui.h"

/****************************************************************************
*函数名：main
*输  入：无
*输  出：无
*功  能：主函数
****************************************************************************/

void main (void)
{
	TFT_Init();                   //初始化TFT
	TFT_ClearScreen(BLACK);	  	  //刷黑色

	while(1)
	{
		GUI_Write32CnChar(25, 24, "普中科技", RED, BLACK);
		GUI_Line(30, 54, 150, 54, RED);
		GUI_WriteASCII(0, 65, "2013", RED, BLACK);
		GUI_Write32CnChar(64, 60, "年", RED, BLACK);
		GUI_WriteASCII(100, 65, "12", RED, BLACK);
		GUI_Write32CnChar(130, 60, "月", RED, BLACK);
		GUI_WriteASCII(160, 65, "31", RED, BLACK);
		//GUI_Write32CnChar(186, 60, "日", RED, BLACK);
		GUI_WriteASCII(0, 100, "12-00-00", RED, BLACK);
		GUI_Write32CnChar(0, 140, "星期", RED, BLACK);
		GUI_WriteASCII(70, 140, "2", RED, BLACK);
		while(1);
	}		
}

	