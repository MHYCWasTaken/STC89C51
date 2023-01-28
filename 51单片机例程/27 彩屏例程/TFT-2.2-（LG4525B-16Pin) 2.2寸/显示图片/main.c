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
	TFT_Init();
	TFT_ClearScreen(BLACK);
	GUI_ShowPicture(175-60, 220-32, 60, 32); //显示在右下角则用最大值减去图片大小
 	GUI_Write32CnChar(22, 0, "普中科技", RED, BLACK);
	GUI_Line(22, 29, 150, 29, RED);
    while(1);
}

	