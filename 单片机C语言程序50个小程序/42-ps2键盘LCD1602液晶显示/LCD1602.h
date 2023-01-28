#include<stdio.h>
//定义数据连接端口
//1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK

void ShowString (unsigned char line,char *ptr);

void ShowChar(unsigned char pos,unsigned char c);

void DelayMs(unsigned char ms);// Ms  延时

void DelayUs(unsigned char us);// us  延时

void InitLcd();//初始化

void WriteCommand(unsigned char c);//写命令

