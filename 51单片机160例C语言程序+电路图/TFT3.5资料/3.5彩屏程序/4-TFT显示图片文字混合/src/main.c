
/*------------------------------------------------
                ����ͷ�ļ�
------------------------------------------------*/
#include"reg52.h"
#include"tft.h"
/*------------------------------------------------
             		������
------------------------------------------------*/
void  main(void)
{   

TFT_Initial();//��ʼ��
while(1)
{
ClearScreen(0x0000);
ClearScreen(0x00ff);
ClearScreen(0xff00);


show_colour_bar();	//��ʾ����
ClearScreen(0x00ff);

LCD_PutString(40,40,"www.doflye.net",0x0000,0xffff);
LCD_PutString(40,60,"I LOVE MY JOB ",0x0000,0xf800);
LCD_PutString(10,80,"�Ϻ�������ӿƼ����޹�˾",0x0000,0xf800);
LCD_PutString(40,100,"רҵ����֧����̳",0x07e0,0xf800);
LCD_PutString(40,120,"רҵ�����幤�ذ�",0x001f,0xf800);
LCD_PutString(40,140,"1234567890",0x001f,0xf800);
LCD_PutString(0,160,"abcdefghijklmnopqistuvwxyz",0x001f,0xf800);
LCD_PutString(0,180,"`,./<>';:[]{}\|?)(-=+*&^%$",0x001f,0xf800);

show_photo(40,200,260,380);


	
while(1);       	//ˢ��ֹͣ
	}
}
