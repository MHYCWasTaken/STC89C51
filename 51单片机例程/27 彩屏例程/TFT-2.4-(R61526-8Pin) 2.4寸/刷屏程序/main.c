#include "REG51.H"
#include "lcd_drive.h"

void main(void)
{
	TFT_Init();
	while(1)
	{
		TFT_ClearScreen(~BLUE);    
		TFT_ClearScreen(~RED);     		
		TFT_ClearScreen(~GREEN);

	}
}
	