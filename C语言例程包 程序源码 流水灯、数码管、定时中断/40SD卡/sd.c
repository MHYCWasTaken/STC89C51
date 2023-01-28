/*-----------------------------------------------

------------------------------------------------*/
#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include<9325TP.h>
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
unsigned char xdata DATA[75]={0};
unsigned char xdata DATA1[75]={0};

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
/********************************************************************
* 名称 : Com_Init()
* 功能 : 初始化串口程序，晶振11.0592, 波特率9600
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Com_Init(void)
{
     TMOD = 0x20;
     PCON = 0x00;
     SCON = 0x50;			
     TH1 = 0xFd;
     TL1 = 0xFd;
     TR1 = 1;			
}
//============================================================
//主程序	
main()
{

  unsigned long AddTemp=262144;//SD卡地址第一个数据物理地址初始值，可以用winhex查看，这里是512扇区，512x512=262144，根据实际SD卡内容更改
unsigned char i;
unsigned char *p;
  	CS=1;
	delayms(5);
	RES=0;
	delayms(5);
	RES=1;
	delayms(5);
	SdInit();         //SD卡初始化
	Com_Init();
	for(i=0;i<75;i++)
	{
		DATA1[i]=i;
	}  	

		SdWriteBlock(DATA1, AddTemp, 75);
    	SdReadBlock(DATA, AddTemp, 75);
		p= DATA; 
while(1)		
for(i=0;i<75;i++)
	{
		SBUF = *p+48; 
		while(!TI)                   //如果发送完毕，硬件会置位TI
		{
			_nop_();	
		}
		p++;
		TI = 0;		                //TI清零
		delayms(500);
	}
}


