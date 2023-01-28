#include "REG51.H"
#include "lcd_drive.h"
#include "gui.h"

void main(void)
{
	TFT_Init();
	while(1)
	{
		TFT_ClearScreen(BLACK); 
		GUI_Write32CnChar(100, 24, "普中科技", RED, BLACK);
		GUI_Line(100, 54, 220, 54, RED);
		GUI_WriteASCII(270, 0, "RST", RED, BLACK);
		GUI_WriteASCII(20, 75, "Welcome To Use MCU", RED, BLACK);

		GUI_WriteASCII(75, 105, "Hello World", RED, BLACK);
		GUI_WriteASCII(25, 150, "The Time:", RED, BLACK);
		GUI_WriteASCII(165, 150, "12:00:00", GREEN, BLACK);
		GUI_WriteASCII(25, 180, "The Date:", RED, BLACK);
		GUI_WriteASCII(165, 180, "3", YELLOW, BLACK);
		GUI_Write32CnChar(180, 180, "月", YELLOW, BLACK);
		GUI_WriteASCII(210, 180, "31", YELLOW, BLACK);
		GUI_Write32CnChar(235, 180, "日", YELLOW, BLACK);
		while(1);

	}
}
	