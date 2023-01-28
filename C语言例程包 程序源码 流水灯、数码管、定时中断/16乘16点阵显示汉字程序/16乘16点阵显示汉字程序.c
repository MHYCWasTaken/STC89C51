#include <reg52.h>

typedef unsigned char uint8;
typedef unsigned int uint16;

sbit acon=P1^0;	//上8位地址线
sbit bcon=P1^1;
sbit ccon=P1^2;
sbit acon1=P1^3;	//下8位地址线
sbit bcon1=P1^4;
sbit ccon1=P1^5;

sbit en=P1^6;	//38译码器使能
sbit en1=P1^7;

uint8 j=0,k=0;

code uint8 ledcode[]={0xDC,0x07,0xDD,0xF7,0xDD,0xF7,0xDC,0x07,0x07,0xFF,0xD8,0x43,0xDB,0x5B,0xD3,0x5B,
0xC8,0x43,0x1F,0xBF,0xD0,0x01,0xDE,0x9F,0xDD,0xAF,0xDB,0xB7,0x47,0xB9,0xBF,0xBF};	 //“操” 



void timer0_init()	 //timer0初始化
{
TMOD|=0x01;
TMOD&=0xfd;
TH0=0xfc;	 //1ms 
TL0=0x66;
TR0=1;
}

void inter_init()
{
ET0=1;
EA=1;
}

void frush()
{
switch(j)	  //扫描
{
case 0:en=1;en1=0;acon=0;bcon=0;ccon=0;break;
case 1:en=1;en1=0;acon=1;bcon=0;ccon=0;break;
case 2:en=1;en1=0;acon=0;bcon=1;ccon=0;break;
case 3:en=1;en1=0;acon=1;bcon=1;ccon=0;break;
case 4:en=1;en1=0;acon=0;bcon=0;ccon=1;break;
case 5:en=1;en1=0;acon=1;bcon=0;ccon=1;break;
case 6:en=1;en1=0;acon=0;bcon=1;ccon=1;break;
case 7:en=1;en1=0;acon=1;bcon=1;ccon=1;break;
case 8:en1=1;en=0;acon1=0;bcon1=0;ccon1=0;break;
case 9:en1=1;en=0;acon1=1;bcon1=0;ccon1=0;break;
case 10:en1=1;en=0;acon1=0;bcon1=1;ccon1=0;break;
case 11:en1=1;en=0;acon1=1;bcon1=1;ccon1=0;break;
case 12:en1=1;en=0;acon1=0;bcon1=0;ccon1=1;break;
case 13:en1=1;en=0;acon1=1;bcon1=0;ccon1=1;break;
case 14:en1=1;en=0;acon1=0;bcon1=1;ccon1=1;break;
case 15:en1=1;en=0;acon1=1;bcon1=1;ccon1=1;break;
}
P2=ledcode[k+1];
P3=ledcode[k];
if(j==15)j=0;
if(k==30)k=0;
j++;
k+=2;

}

main()
{
timer0_init();
inter_init();
en=1;
en1=1;
while(1);
}



void timer0_interrupt() interrupt 1
{
TH0=0xfc;
TL0=0x66;
frush();

}