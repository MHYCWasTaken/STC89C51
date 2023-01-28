#include "REG51.H"
#include "lcd_drive.h"
#include "gui.h"

void main(void)
{
	TFT_Init();
	TFT_ClearScreen(BLACK);
//	TFT_ClearScreen(WHITE);
	while(1)
	{
		
		GUI_Write32CnChar(100, 20, "¼¼¿ÆÖÐÆÕ", RED, BLACK); 
		GUI_ShowPicture(74, 69, 151, 150);
		while(1);

	}
}
	