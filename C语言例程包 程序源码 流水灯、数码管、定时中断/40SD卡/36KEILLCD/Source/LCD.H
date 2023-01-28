/*
================================================================================
Copytight      : Yihetech Co,.Ltd, All rightd reserved. 版权归属亿和电子工作室
Store          : http://shop57165217.taobao.com/

File Name      : lcd.c
Description    : LCD module operations
Date           : 2009-7-19 21:17:45
Version        : V1.0
Author         : LiYong(李勇), yihe_liyong@126.com
Target device  : C8051F340
Compiler       : KeilC51 V8.16
Note           : 转载时务必保留此信息，否则无权以任何形式传播本文件
================================================================================
*/
#ifndef  _LCD_H_
#define  _LCD_H_


#include <reg51.h>
#include "mytypedef.h"
#include "GUI_Type.h"
#include <util\delay.h>
//#include <avr\pgmspace.h>



/*
================================================================================
-----------------------------LCD Internal register list-------------------------
================================================================================
*/
#ifdef _LCD_C_

#define TRACK20		0
#define TRACK21		1

#define NO_OPERATION            0x00
#define SOFT_RESET              0x01
//#define BOOSTER_OFF             0x02	//no such command, in new lcd
//#define BOOSTER_ON              0x03
#define DISPLAY_INFO            0x04   // read 25bits where MSB are dummy trash
#define DISPLAY_STATUS          0x09   // read 33bits where MSB are dummy trash
#define SLEEP_IN                0x10	//sleep in and booster_off
#define SLEEP_OUT               0x11	//sleep out and booster_on
#define PARTIAL_MODE_ON         0x12
#define NORMAL_MODE_ON          0x13
#define INVERSION_OFF           0x20
#define INVERSION_ON            0x21
#define PIXELS_OFF              0x22
#define PIXELS_ON               0x23
#define SET_CONTRAST            0x25	//param range 00~7fh
  // CONTRAST_VALE              0xXX
#define DISPLAY_OFF             0x28
#define DISPLAY_ON              0x29
#define SET_X_ADDR              0x2A  // ATENTION! when X,Y address are changed BOTH must be changed
  // START_X                    0xXX
  // STOP_X                     0xXX
#define SET_Y_ADDR              0x2B
  // START_Y                    0xXX
  // STOP_Y                     0xXX
#define MEM_WRITE               0x2C
  // x times pixel data         0xXX
#define COLOR_SET               0x2D	//param has some diff?
  // 8 times red palette        0xXX
  // 8 times green palette      0xXX
  // 4 times blue palette       0xXX
#define PARTIAL_AREA            0x30
  // START_Y                    0xXX
  // STOP_Y                     0xXX
#define SCROLL_DEFINITON        0x33  // always vertical
  // TOP_FIXED_LINES            0xXX
  // SCROLL_LINES               0xXX
  // BOTTOM_FIXED_LINES         0xXX
#define SET_SCROLL              0x37  // SEt scroll entry point
  // ROW_Y                      0xxx
#define MEM_CONTROL             0x36	//memory data access control
  // MEM_FLAGS                  0xXX
  #define MEM_90                0     // virtual flag not for PCF8833
  #define MEM_RGB               3     // color order
  #define MEM_LAO               4     // line access order
  #define MEM_VW                5     // vertical write
  #define MEM_MX                6     // mirror X
  #define MEM_MY                7     // mirror Y
#define MEMCTRLDAT	(0 << MEM_MX) | (1 << MEM_MY) | (1 << MEM_RGB) | (0 << MEM_VW) | (0 << MEM_LAO)
#define IDLE_OFF                0x38
#define IDLE_ON                 0x39
#define COLOR_INTERFACE         0x3A	//set the interface pixel format
  // pixelformat                0xXX
  #define COLOR_8_BIT           0x02
  #define COLOR_12_BIT          0x03
  #define COLOR_16_BIT          0x05
#define SET_VOP					0xc0	//only for ST7637
  // 1x                         0xXX
  //2x
#define VOP_INC					0xc1
#define VOP_DEC					0xc2
#define VOP_OFFSET				0xC7
	//1x
	//2x
#define BIAS_SET				0xc3
	//x
#define BOOST_SET				0xc4	//
	//x
#define BOOST_EFF				0xc5
	//x

#endif //_LCD_C_
/*
================================================================================
------------------------------Internal EXPORT functions-------------------------
================================================================================
*/
/*Initialize the LCD module*/
extern void LCD_Init( void );

/*draw a dollop with single color*/
extern void	LCDDrawDollop( DOLLOP* dollop );

/*draw a point*/
extern void LCDDrawPoint( POINT* pPoint );

/*draw a direct line*/
extern void LCDDrawHRLine( LINE* pLine );

/*//draw a bitmap with single color*/
extern void	PrintBitBlock( BitBlock *pBitBlock );

/*Print a image*/
void GUI_Image( IMAGE *pImage );


#endif //_LCD_H_

/*
================================================================================
-----------------------------------End of file----------------------------------
================================================================================
*/
