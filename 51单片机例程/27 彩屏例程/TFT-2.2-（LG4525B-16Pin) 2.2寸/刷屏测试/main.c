#include "REG51.H"
#include "lcd_driver.h"

/****************************************************************************
*函数名：main
*输  入：无
*输  出：无
*功  能：主函数
****************************************************************************/

void main (void)
{
	TFT_Init();                   //初始化TFT

	while(1)
	{
		TFT_ClearScreen(BLACK);	  //刷黑色
		TFT_ClearScreen(BLUE);
		TFT_ClearScreen(RED);
		TFT_ClearScreen(MAGENTA);
		TFT_ClearScreen(GREEN);
		TFT_ClearScreen(CYAN);
		TFT_ClearScreen(YELLOW);
		TFT_ClearScreen(WHITE);		
	}
}

	