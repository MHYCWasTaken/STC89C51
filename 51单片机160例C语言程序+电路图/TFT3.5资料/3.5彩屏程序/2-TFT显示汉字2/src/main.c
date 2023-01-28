
/*------------------------------------------------
                包含头文件
------------------------------------------------*/
#include"reg52.h"
#include"tft.h"
/*------------------------------------------------
             		主函数
------------------------------------------------*/
void  main(void)
{   

TFT_Initial();//初始化
while(1)
{

show_colour_bar();	//显示彩条
ClearScreen(0xff00);
Show_RGB (0,240,0,320,0xf800);//刷出320x240的小区域
ClearScreen(0xff00);
LCD_PutString(0,0,"朗译电子科技",0x0000,0x00f8);
LCD_PutString(0,32,"朗译电子科技",0xffff,0xf800);
LCD_PutString(0,64,"朗译电子科技",0x2334,0x0045);
LCD_PutString(0,96,"朗译电子科技",0x0560,0x0a80);
LCD_PutString(0,128,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,160,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,192,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,224,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,256,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,288,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,320,"朗译电子科技",0x0000,0x0f80);
LCD_PutString(0,352,"朗译电子科技",0x0000,0x0f80);

	
while(1);       	//刷完停止
	}
}
