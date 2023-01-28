
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
ClearScreen(0x0000);
ClearScreen(0x00ff);
ClearScreen(0xff00);


show_colour_bar();	//显示彩条
ClearScreen(0x00ff);

LCD_PutString(40,40,"www.doflye.net",0x0000,0xffff);
LCD_PutString(40,60,"I LOVE MY JOB ",0x0000,0xf800);
LCD_PutString(10,80,"上海朗译电子科技有限公司",0x0000,0xf800);
LCD_PutString(40,100,"专业技术支持论坛",0x07e0,0xf800);
LCD_PutString(40,120,"专业开发板工控板",0x001f,0xf800);
LCD_PutString(40,140,"1234567890",0x001f,0xf800);
LCD_PutString(0,160,"abcdefghijklmnopqistuvwxyz",0x001f,0xf800);
LCD_PutString(0,180,"`,./<>';:[]{}\|?)(-=+*&^%$",0x001f,0xf800);

show_photo(40,200,260,380);


	
while(1);       	//刷完停止
	}
}
