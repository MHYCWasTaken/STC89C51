#include "REG51.H"
#include "lcd_drive.h"

void main(void)
{
	TFT_Init();
	while(1)
	{
		TFT_ClearScreen(WHITE);   //°×
		TFT_ClearScreen(BLACK);   //ºÚ
		TFT_ClearScreen(BLUE);    //À¶
		TFT_ClearScreen(RED);     //ºì
		TFT_ClearScreen(MAGENTA); 
		TFT_ClearScreen(GREEN);   

	}
}
	