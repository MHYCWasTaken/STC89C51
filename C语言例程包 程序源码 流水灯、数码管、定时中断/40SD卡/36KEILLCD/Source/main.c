/*
================================================================================
Copytight      : Yihetech Co,.Ltd, All rightd reserved. 版权归属亿和电子工作室
Store          : http://shop57165217.taobao.com/

File Name      : main.c
Description    :  
Date           : 2009-7-19 21:17:45
Version        : V1.0
Author         : LiYong(李勇), yihe_liyong@126.com
Target device  : ATmega32L
Compiler       : AVRStudio + GCCAVR
Note           : 转载时务必保留此信息，否则无权以任何形式传播本文件
================================================================================
*/

#include <reg51.h>
#include "MyTypedef.h"
#include "GUI_Basic.h"
#include "LCD.H"			 



    RECT rect  ;
    CIRCLE circle ;
    LINE line;
    FONT font ;
    ELLIPSE ellipse;

int main( void )
{	
  
    LCD_Init( );
	rect.xs = 10;
	rect.ys = 10;
	rect.xe = 50;
	rect.ye = 50;
	rect.Color = 0xFF00;

	circle.x = 64;
	circle.y = 64;
	circle.r = 40;
	circle.Color = 0xFF00;

	line.xs = 10;
	line.xe = 100;
	line.ys = 10;
	line.ye = 120;
	line.Color = 0x00FF;

	font.Height = 14;
	font.Width = 7;
	font.Color = 0xF800;
	font.BackColor = 0x07FF;

	ellipse.xs = 10;
	ellipse.xe = 100;
	ellipse.ys = 80;
	ellipse.ye = 120;
	ellipse.Color = 0x00FF;


	GUI_Inital( 0x000f );
	GUI_DrawRectangle( &rect );
	GUI_DrawCircle( &circle );

	circle.r = 20;
	GUI_DrawCircleFill( &circle );
	GUI_DrawLine( &line );

	line.xe = 160;
	GUI_DrawLine( &line );

	GUI_DrawEllipse( &ellipse );
	ellipse.xs = 10;
	ellipse.xe = 120;
	ellipse.ys = 90;
	ellipse.ye = 110;
	ellipse.Color = 0x07E0;
	GUI_DrawEllipseFill( &ellipse );

	GUI_DisplayStr( 60, 20, &font, "taobao" );
	GUI_DisplayStr( 60, 80, &font, "ABCDEFG" );



	while( 1 );
}

/*
================================================================================
-----------------------------------End of file----------------------------------
================================================================================
*/
