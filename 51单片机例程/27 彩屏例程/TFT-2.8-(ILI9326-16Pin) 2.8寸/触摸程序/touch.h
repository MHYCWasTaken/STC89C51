#ifndef __TOUCH_H
#define __TOUCH_H

//---包含头文件---//
#include<reg51.h>
#include<intrins.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---定义数据类型---//
typedef struct 
{
	uint x;
	uint y;
} XPT_XY;

extern XPT_XY xpt_xy;

//---定义芯片命令字节---//
#define XPT_CMD_X	0xD0    //读取X轴的命令
#define XPT_CMD_Y	0x90	//读取Y轴的命令

//---定义使用的IO口---//
sbit TOUCH_DOUT = P2^0;	  //输出
sbit TOUCH_CLK  = P2^1;	  //时钟
sbit TOUCH_DIN  = P2^2;	  //输入
sbit TOUCH_CS   = P2^3;	  //片选
sbit TOUCH_PEN  = P2^4;	  //检测触摸屏响应信号

//---声明全局函数---//
void TOUCH_SPI_Start(void);
void TOUCH_SPI_Write(uchar dat);
uint TOUCH_SPI_Read(void);
uint TOUCH_XPT_ReadData(uchar cmd);
uchar TOUCH_XPT_ReadXY(void);



#endif


//#ifndef __TOUCH_H
//#define __TOUCH_H
//
////---包含头文件---//
//#include<reg51.h>
//#include<intrins.h>
//#include"gui.h"
//
////---重定义关键词---//
//#ifndef uchar
//#define uchar unsigned char
//#endif
//
//#ifndef uint
//#define uint  unsigned int
//#endif
//
//#ifndef ulong
//#define ulong  unsigned long
//#endif
//
////---定义数据类型---//
//typedef struct 
//{
//	uint x;
//	uint y;
//} XPT_XY;
//
////--- 偏移结构体 ---//
//typedef struct{   
//    int xOffset;
//    int yOffset; 
//    float xFactor;
//    float yFactor;
//} PosTypeDef;
//
//extern XPT_XY xpt_xy;
//extern PosTypeDef TouchAdj; //定义一阵数据用来保存校正因数
//
//
////--- 设置触摸屏的对应关系 ---//
////--- 当LCDx对应ADx；LCDy对应ADy；且最小值在左上角时，设置为0 ---//
////--- 当LCDx对应ADy；LCDy对应ADx；且最小值在右下角时，设置为1 ---//
////--- LCD_TOUCH_TYPE为0时，要自己手动校正，当为1时，利用程序自身提供的数据校正 ---//
//# define LCD_TOUCH_TYPE 1   
//
////--- 触摸校正因数设置 ---//
//#define LCD_ADJX_MIN ((uint)10)                        //读取四个点的最小X值
//#define LCD_ADJX_MAX ((uint)TFT_XMAX - LCD_ADJX_MIN) //读取四个点的最大X值
//#define LCD_ADJY_MIN ((uint)10)                        //读取四个点的最小Y值
//#define LCD_ADJY_MAX ((uint)TFT_YMAX - LCD_ADJY_MIN) //读取四个点的最大Y值
//
//#define LCD_ADJ_MUX  ((uint)1)                     //偏移因数的比例倍数
//
//#define LCD_ADJx (LCD_ADJX_MAX - LCD_ADJY_MIN)         //读取方框的宽度
//#define LCD_ADJy (LCD_ADJY_MAX - LCD_ADJY_MIN)         //读取方框的高度
//#define ADJ_CHECK_DELAY {for(i=0; i<0x0FFF; i++);}     //读点是的延时大小
//
////---定义芯片命令字节---//
//#define XPT_CMD_X	0xD0    //读取X轴的命令
//#define XPT_CMD_Y	0x90	//读取Y轴的命令
//
////---定义使用的IO口---//
//sbit TOUCH_DOUT = P2^0;	  //输出
//sbit TOUCH_CLK  = P2^1;	  //时钟
//sbit TOUCH_DIN  = P2^2;	  //输入
//sbit TOUCH_CS   = P2^3;	  //片选
//sbit TOUCH_PEN  = P2^4;	  //检测触摸屏响应信号
//
//
//
////---声明全局函数---//
//void TOUCH_SPI_Start(void);
//void TOUCH_SPI_Write(uchar dat);
//uint TOUCH_SPI_Read(void);
//uint TOUCH_XPT_ReadData(uchar cmd);
//uchar TOUCH_XPT_ReadXY(void);
//void TOUCH_Adjust(void);
//
//
//
//#endif
