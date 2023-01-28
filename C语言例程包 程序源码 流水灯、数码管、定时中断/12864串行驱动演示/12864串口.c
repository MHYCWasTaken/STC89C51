/*
12864LCD （ST7920）驱动程序
马文垒     2004/12/27

*/

#include <REGx51.H>
  //引脚定义
#define CS    P3_0	//片选 高电平有效 单片LCD使用时可固定高电平
#define SID   P3_1	//数据
#define SCLK  P3_2	//时钟
#define PSB   P3_3	//低电平时表示用串口驱动，可固定低电平
#define RESET P3_4	//LCD复位，LCD模块自带复位电路。可不接

void write(bit start, unsigned char ddata);
void sendbyte(unsigned char bbyte);
void delaynms(unsigned int di);
void lcdinit(void);
void lcdtest(void);
void lcdcls(void);

unsigned char code ma[] ={"  汉字显示LCD   "};
unsigned char code ma2[]={"自带字库图文混排"};
unsigned char code ma3[]={"控制IC：ST7920  "};
unsigned char code ma4[]={"功能强大使用方便"};

void lcdinit(void)	//初始化LCD 
{
  delaynms(10); //启动等待，等LCM讲入工作状态
  PSB=0; ;   	 //串口驱动模式
  RESET=0; delaynms(1); RESET=1; // 复位LCD
  CS=1;
  write(0,0x30);  //8 位介面，基本指令集
  write(0,0x0c);  //显示打开，光标关，反白关
  write(0,0x01);  //清屏，将DDRAM的地址计数器归零  
}

void write(bit start, unsigned char ddata) //写指令或数据
{
  unsigned char start_data,Hdata,Ldata;
  if(start==0) start_data=0xf8;	 //写指令
    else       start_data=0xfa;  //写数据
  
  Hdata=ddata&0xf0;		  //取高四位
  Ldata=(ddata<<4)&0xf0;  //取低四位
  sendbyte(start_data);	  //发送起始信号
  delaynms(5); //延时是必须的
  sendbyte(Hdata);	      //发送高四位
  delaynms(1);  //延时是必须的
  sendbyte(Ldata);		  //发送低四位
  delaynms(1);  //延时是必须的
}

void sendbyte(unsigned char bbyte) //发送一个字节
{
 unsigned char i;
 for(i=0;i<8;i++)
   {
   SID=bbyte&0x80; //取出最高位
   SCLK=1;
   SCLK=0;
   bbyte<<=1; //左移
   }  
}

void delaynms(unsigned int di) //延时
{
 unsigned int da,db;
 for(da=0;da<di;da++)
   for(db=0;db<10;db++);
}

void lcdtest(void)	//图形方式下屏幕全黑
{ 
unsigned char i,j;
for(i=0;i<32;i++)
  { write(0,0x80+i);write(0,0x80);
    for(j=0;j<16;j++) write(1,0xff);
  }

 for(i=0;i<32;i++)
  { write(0,0x80+i);write(0,0x88);
    for(j=0;j<16;j++) write(1,0xff);
  }
}

void lcdcls(void)  //图形方式下清屏
{ 
unsigned char i,j;
for(i=0;i<32;i++)
  { write(0,0x80+i);write(0,0x80);
    for(j=0;j<16;j++) write(1,0x00);
  }

 for(i=0;i<32;i++)
  { write(0,0x80+i);write(0,0x88);
    for(j=0;j<16;j++) write(1,0x00);
  }
}

void main(void)
{
 unsigned char i;
 lcdinit();

 while(1)
 {
  write(0,0x80); for(i=0;i<16;i++)  write(1,ma[i]);
  write(0,0x90); for(i=0;i<16;i++)  write(1,ma2[i]);
  write(0,0x88); for(i=0;i<16;i++)  write(1,ma3[i]);
  write(0,0x98); for(i=0;i<16;i++)  write(1,ma4[i]);

  write(0,0x36); //图形方式
  lcdtest();
  lcdcls();
 }

}
 



