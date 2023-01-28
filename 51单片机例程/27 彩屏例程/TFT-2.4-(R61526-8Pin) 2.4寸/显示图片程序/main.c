#include "REG51.H"
#include "lcd_drive.h"
#include "gui.h"

void main(void)
{
	TFT_Init();
	while(1)
	{
		TFT_ClearScreen(~BLACK);
		GUI_Write32CnChar(35, 20, "∆’÷–ø∆ºº", ~0xF800, ~0x0000); 
		GUI_ShowPicture(24, 69, 151, 150);
		while(1);

	}
}
	