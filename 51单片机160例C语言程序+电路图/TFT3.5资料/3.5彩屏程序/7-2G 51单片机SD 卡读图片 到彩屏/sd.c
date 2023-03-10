/*-----------------------------------------------
  名称：读SD卡写彩屏
  公司：德飞莱
  网站：www.doflye.net
  编写：shifang
  日期：2009.12
  修改：无
  内容：通过读出SD卡中320x480像素、16位BMP图片的HEX数据，依次写到屏上，还原图片，使用image2LCD软件转换图片，详见使用说明
  注意事项：由于SD卡使用SPI模式，并且是块读取(512Byte),所以需要单片机提供大于512的可用RAM，以供缓冲使用
            这里可以选择STC89c52RD+、STC89C58RD+、STC89C516RD+，后缀RD+表明扩展RAM是1024字节，RC表明扩展RAM256字节
------------------------------------------------*/
#include <reg52.h>
#include <stdio.h>
#include<tft35.h>
//=============================================================
//定义SD卡需要的4根信号线
sbit SD_CLK = P1^1;
sbit SD_DI  = P1^2;
sbit SD_DO  = P1^0;
sbit SD_CS  = P1^3;
//===========================================================
//定义按键端口
sbit KEY = P3^2;
//===========================================================
//定义512字节缓冲区，注意需要使用 xdata关键字
unsigned char xdata DATA[512];

//===========================================================
//写一字节到SD卡,模拟SPI总线方式
void SdWrite(unsigned char n)
{

unsigned char i;

for(i=8;i;i--)
{
SD_CLK=0;
SD_DI=(n&0x80);
n<<=1;
SD_CLK=1;
}
SD_DI=1; 
} 
//===========================================================
//从SD卡读一字节,模拟SPI总线方式
unsigned char SdRead()
{
unsigned char n,i;
for(i=8;i;i--)
{
SD_CLK=0;
SD_CLK=1;
n<<=1;
if(SD_DO) n|=1;

}
return n;
}
//============================================================
//检测SD卡的响应
unsigned char SdResponse()
{
unsigned char i=0,response;

while(i<=8)
{
response = SdRead();
if(response==0x00)
break;
if(response==0x01)
break;
i++;
}
return response;
} 
//================================================================
//发命令到SD卡
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{

SdWrite(command|0x40);
SdWrite(((unsigned char *)&argument)[0]);
SdWrite(((unsigned char *)&argument)[1]);
SdWrite(((unsigned char *)&argument)[2]);
SdWrite(((unsigned char *)&argument)[3]);
SdWrite(CRC);
}
//================================================================
//初始化SD卡
unsigned char SdInit(void)
{
int delay=0, trials=0;
unsigned char i;
unsigned char response=0x01;

SD_CS=1;
for(i=0;i<=9;i++)
SdWrite(0xff);
SD_CS=0;

//Send Command 0 to put MMC in SPI mode
SdCommand(0x00,0,0x95);


response=SdResponse();

if(response!=0x01)
{
return 0;
} 

while(response==0x01)
{
SD_CS=1;
SdWrite(0xff);
SD_CS=0;
SdCommand(0x01,0x00ffc000,0xff);
response=SdResponse();
} 

SD_CS=1;
SdWrite(0xff);
return 1; 
}
//================================================================
//往SD卡指定地址写数据,一次最多512字节
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
unsigned char dataResp;
//Block size is 512 bytes exactly
//First Lower SS

SD_CS=0;
//Then send write command
SdCommand(0x18,address,0xff);

if(SdResponse()==00)
{
SdWrite(0xff);
SdWrite(0xff);
SdWrite(0xff);
//command was a success - now send data
//start with DATA TOKEN = 0xFE
SdWrite(0xfe);
//now send data
for(count=0;count<len;count++) SdWrite(*Block++);

for(;count<512;count++) SdWrite(0);
//data block sent - now send checksum
SdWrite(0xff); //两字节CRC校验, 为0XFFFF 表示不考虑CRC
SdWrite(0xff);
//Now read in the DATA RESPONSE token
dataResp=SdRead();
//Following the DATA RESPONSE token
//are a number of BUSY bytes
//a zero byte indicates the MMC is busy

while(SdRead()==0);

dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
SD_CS=1;
SdWrite(0xff);
if(dataResp==0x0b)
{
//printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
return 0;
}
if(dataResp==0x05)
return 1;

//printf("Invalid data Response token.\n");
return 0;
}
//printf("Command 0x18 (Write) was not received by the MMC.\n");
return 0;
}

//=======================================================================
//从SD卡指定地址读取数据,一次最多512字节
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
//Block size is 512 bytes exactly
//First Lower SS

 //printf("MMC_read_block\n");

SD_CS=0;
//Then send write command
SdCommand(0x11,address,0xff);

if(SdResponse()==00)
{
//command was a success - now send data
//start with DATA TOKEN = 0xFE
while(SdRead()!=0xfe);

for(count=0;count<len;count++) *Block++=SdRead(); 

for(;count<512;count++) SdRead();

//data block sent - now send checksum
SdRead();
SdRead();
//Now read in the DATA RESPONSE token
SD_CS=1;
SdRead();
return 1;
}
 //printf("Command 0x11 (Read) was not received by the MMC.\n");
return 0;
}

//============================================================
//主程序	
main()
{
 unsigned int x,y; //定义液晶屏坐标
 unsigned long j;  //执行循环需要的临时变量
  unsigned int i;
     unsigned long AddTemp=328192;//SD卡地址第一个数据物理地址初始值，可以用winhex查看，这里是641物理扇区，512x641=328192，根据实际SD卡内容更改
 TFT_Initial();
 SdInit();         //SD卡初始化
 ClearScreen(0xff00);    //刷屏
 while(1)
 {

 for(j=0;j<375;j++)   //375表示一幅图片含有375x512字节的信息,240x400x2/512=375
    {
    SdReadBlock(DATA,AddTemp+(j*512),512);//每次读出512字节放到缓冲区
    for(i=0;i<256;i++)                    //然后写到液晶屏，可以显示256个像素，每个像素16位即2个字节
	   {   
   	   LCD_SetPos(x,x,y,y);
   	   Write_Data(DATA[2*i+1],DATA[2*i]);	 
	   x++;
	   if(x==240)                         //检测是否写到屏的边缘 400x240
	     {
	     y++;
	     x=0;
	     if(y==400)
	       y=0;
	     }
       }
     } 
     AddTemp = AddTemp+((j)*512);          //写完一幅图片后把SD地址加600x512到下一个图片地址
     while(KEY);                         //等待按键按下继续执行循环显示下一幅图片，如果没有按下则等待
	 
 } 
}


