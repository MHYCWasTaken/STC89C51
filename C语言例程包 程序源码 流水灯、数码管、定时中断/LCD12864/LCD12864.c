/*****************************************************************************************************************

作者：张德星

日期：2007年7月26日

单片机型号：ATMEGA16（L）

频率：8M HZ

功能：LCD12864 子程序程序

******************************************************************************************************************
PA0  接  RS

PA1  接  RW

PA2  接  E

PA3  接  CSA

PA4  接  CSB

RST  直接接到VCC不起作用

PC0-7 接DB0-7  

******************************************************************************************************************/
#include<avr/io.h>
#include<util/delay.h>
#include<avr/pgmspace.h>

#define RS1    PORTA|=_BV(0);
#define RS0    PORTA&=~_BV(0);

#define RW1    PORTA|=_BV(1);
#define RW0    PORTA&=~_BV(1);

#define EN1    PORTA|=_BV(2);
#define EN0    PORTA&=~_BV(2);

#define CSB1   PORTA=_BV(3);
#define CSB0   PORTA&=~_BV(3);

#define CSA1   PORTA|=_BV(4);
#define CSA0   PORTA&=~_BV(4);

#define LCD_ON           0x3f  //显示开
#define LCD_OFF          0x3e  //显示关
#define BEGIN_LINE_SET   0xc0  //显示起始行设置为第0行
#define PAGE_SET         0xb8  //页地址设置
#define Y_ADDRESS_SET    0x40  //列地址设置



unsigned char x;




/******************************************************************************************************************/
const unsigned char char_table[]PROGMEM={

0x80,0x40,0xF0,0x2C,0x43,0x20,0x98,0x0F,0x0A,0xE8,0x08,0x88,0x28,0x1C,0x08,0x00,
0x00,0x00,0x7F,0x00,0x10,0x0C,0x03,0x21,0x40,0x3F,0x00,0x00,0x03,0x1C,0x08,0x00,//你0
0x10,0x10,0xF0,0x1F,0x10,0xF0,0x80,0x82,0x82,0x82,0xF2,0x8A,0x86,0x82,0x80,0x00,
0x80,0x43,0x22,0x14,0x0C,0x73,0x20,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,//好1
0x00,0xFC,0x04,0x04,0xFC,0x00,0x82,0xFA,0x82,0x82,0x82,0x82,0xFE,0x80,0x00,0x00,
0x00,0x07,0x02,0x02,0x07,0x00,0x04,0x05,0x04,0x04,0x04,0x44,0x80,0x7F,0x00,0x00,//吗2
0x00,0x60,0x10,0x08,0x08,0x90,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//？3











/*延时*****************************************************************************************************************/
void delay(unsigned int t)
{while(t--){_delay_ms(1);}}


/*写指令*****************************************************************************************************************/
void Command_Input(unsigned char CSA_B, unsigned char Command)// ( 片选，指令代码 )
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 选择A区
if(CSA_B==1){CSA1;CSB0;}//CSB=0 选择B区
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 同时选择AB区

DDRC=0x00;PORTC=0XFF;       //端口设置为输入

RS0 RW1 EN1      			// RW=E=1 RS=0  读状态
while(PINC&0x80); 			// 忙等待

RS0 RW0 EN1          		// RS=RW=0 E=1  写指令      
PORTC=Command; DDRC=0XFF;	// 将指令代码送到总线
EN0       					// E=0     E下降延将指令代码写入LCD
}
/******************************************************************************************************************/

/*写数据*****************************************************************************************************************/
void Data_Input(unsigned char CSA_B, unsigned char Data)  //( 片选，数据 )
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 选择A区
if(CSA_B==1){CSA1;CSB0;}//CSB=0 选择B区
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 同时选择AB区

DDRC=0x00;PORTC=0XFF; 	//端口设置为输入

RS0 RW1 EN1      		// RW=E=1  RS=0  读状态
while(PINC&0x80); 		// 忙等待

RS1 RW0 EN1            	// RS=E=1  RW=0  写数据      
PORTC=Data;DDRC=0XFF;  	// 将数据代码送到总线
EN0     				// E=0     E下降延将数据代码写入LCD
}
/******************************************************************************************************************/

/*读数据*****************************************************************************************************************/
unsigned char Data_Output(unsigned char CSA_B)//( 片选 ) 返回数据
{
if(CSA_B==0){CSA0;CSB1;}//CSA=0 选择A区
if(CSA_B==1){CSA1;CSB0;}//CSB=0 选择B区
if(CSA_B==2){CSA0;CSB0;}//CSA=0 CSB=0 同时选择AB区

DDRC=0x00;PORTC=0XFF;	//端口设置为输入

RS0 RW1 EN1      		// RS=0  RW=E=1  读状态
while(PINC&0x80); 		// 忙等待

RS1 RW1 EN1            	// RS=E=1  RW=0  写数据  
    
EN1 EN1 EN1 EN1  EN0 EN0 EN0 EN0 EN0

return  PINC;     		//返回读到的数据
}
/******************************************************************************************************************/

/*清屏*****************************************************************************************************************/
void Clear_Screen(void)
{
unsigned char page,n;

for(page=7;page<8;page--)
	{	
	Command_Input(0,PAGE_SET|page);//设置业地址7-0
	Command_Input(0,Y_ADDRESS_SET);//设置列地址为0
    for(n=63;n<64;n--)Data_Input(0,0x00);//写数据0x00

	Command_Input(1,PAGE_SET|page);//设置业地址7-0
	Command_Input(1,Y_ADDRESS_SET);//设置列地址为0
	for(n=63;n<64;n--)Data_Input(1,0x00);//写数据0x00
	}
}
/******************************************************************************************************************/

/*显示一个汉字  方案1*****************************************************************************************************************/
void Show_One_Word(unsigned char X_Address,unsigned char Y_Address,unsigned char Char_Address)//(0-111, 0-6 , 0-255 )原点在左上角
{
unsigned int  char_table_address;	//数据在字表中的位置
unsigned char a_x_address;			//在a区的开始列地址
unsigned char b_x_address;			//在b区的开始列地址
unsigned char a_n;					//在a区的数据个数
unsigned char b_n;					//在b区的数据个数
unsigned char n;  					//循环计数


char_table_address = Char_Address*32;//确定数据在字表中的首地址，一个汉字32字节

if(X_Address<=48)//汉字在a区
	{
	 a_x_address=X_Address; 
	 a_n=16;b_n=0;
	 }

if(X_Address>=64)//汉字在b区
    {
	 b_x_address=X_Address-64; 
	 a_n=0;b_n=16;
	 }

if((48<X_Address)&&(X_Address<64))//汉字在a区和b区之间
	{
	 a_x_address=X_Address; 
	 b_x_address=0; 
	 a_n=64-X_Address;
	 b_n=X_Address-48;
     }

Command_Input(0,PAGE_SET|Y_Address);		//设置A区业地址为Y_Address 
Command_Input(0,Y_ADDRESS_SET|a_x_address);	//设置A区列地址为a_x_address

for(n=a_n ; n>0 ; n--)
	{
	Data_Input(0,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(1,PAGE_SET|Y_Address);		//设置B区业地址为Y_Address
Command_Input(1,Y_ADDRESS_SET|b_x_address); //设置B区列地址为b_x_address

for(n=b_n ; n>0 ; n--)
	{
	Data_Input(1,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(0,PAGE_SET|(Y_Address+1));	//设置A区业地址为Y_Address+1
Command_Input(0,Y_ADDRESS_SET|a_x_address);	//设置A区列地址为a_x_address

for(n=a_n ; n>0 ; n--)
	{
	Data_Input(0,pgm_read_byte(char_table+char_table_address++));
	}


Command_Input(1,PAGE_SET|(Y_Address+1));	//设置B区业地址为Y_Address+1 
Command_Input(1,Y_ADDRESS_SET|b_x_address); //设置B区列地址为b_x_address

for(n=b_n ; n>0 ; n--)
	{
	Data_Input(1,pgm_read_byte(char_table+char_table_address++));
	}

}
/******************************************************************************************************************/


///////////////////////////////////////////////////////////////
int main(void)
{
DDRA=0xff;

Command_Input(0,LCD_ON);         //a 区开显示
Command_Input(1,LCD_ON);         //b 区开显示

Command_Input(0,BEGIN_LINE_SET); //a 区显示起始行为0
Command_Input(1,BEGIN_LINE_SET); //b 区显示起始行为0

Clear_Screen();                  //清屏 


Show_One_Word( 0,4,0);
Show_One_Word(16,4,1);
Show_One_Word(32,4,2);
Show_One_Word(48,4,3);


while(1);


}
