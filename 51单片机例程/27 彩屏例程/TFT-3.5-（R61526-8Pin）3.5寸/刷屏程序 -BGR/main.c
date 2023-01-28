#include "REG51.H"
#include "lcd_drive.h"

void main(void)
{
	TFT_Init();
	TFT_ClearScreen(BLACK);
	while(1)
	{
		TFT_ClearScreen(RED);
		TFT_ClearScreen(BLUE);    		     		
		TFT_ClearScreen(GREEN);
		TFT_ClearScreen(YELLOW);
		TFT_ClearScreen(MAGENTA);
		TFT_ClearScreen(CYAN);
	}
}
	