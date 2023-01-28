#include "REG51.H"
#include "lcd_drive.h"
#include "gui.h"

void main(void)
{
	TFT_Init();
	TFT_ClearScreen(BLACK);   //ºÚ
	while(1)
	{
		GUI_Write32CnChar(60, 24, "ÆÕÖÐ¿Æ¼¼", RED, BLACK);
		GUI_Line(60, 54, 180, 54, RED);
		GUI_ShowPicture(TFT_XMAX - 150, TFT_YMAX - 300, 90, 150);
	}
}
	