#include "REG51.H"
#include "lcd_drive.h"
#include "gui.h"

void main(void)
{
	TFT_Init();
	while(1)
	{
		TFT_ClearScreen(~BLACK); 
		GUI_Write32CnChar(20, 24, "普中科技", ~0xF800, ~0x0000);
		GUI_Line(20, 54, 140, 54, ~0xF800);
		GUI_Line(20, 54, 140, 54, ~0xF800);
		GUI_WriteASCII(190, 0, "RST", ~0xF800, ~0x0000);
		GUI_WriteASCII(0, 65, "2013", ~0xF800, ~0x0000);
		GUI_Write32CnChar(64, 60, "年", ~0xF800, ~0x0000);
		GUI_WriteASCII(93, 65, "12", ~0xF800, ~0x0000);
		GUI_Write32CnChar(120, 60, "月", ~0xF800, ~0x0000);
		GUI_WriteASCII(0, 100, "31", ~0xF800, ~0x0000);
		GUI_Write32CnChar(32, 95, "日", ~0xF800, ~0x0000);
		GUI_WriteASCII(0, 135, "12-00-00", ~0xF800, ~0x0000);
		GUI_Write32CnChar(64, 95, "星期", ~0xF800, ~0x0000);
		GUI_WriteASCII(128, 100, "2", ~0xF800, ~0x0000);
		while(1);

	}
}
	