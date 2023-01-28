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
#include "image.h"			 

    RECT rect  ;
    CIRCLE circle ;
    LINE line;
    FONT font ;
    ELLIPSE ellipse;
IMAGE image_girl;
int main( void )
{	
    LCD_Init( );

    image_girl.ys = 0;
	image_girl.xs = 0;
	image_girl.height = 128;
	image_girl.length = 128;
	image_girl.pData = &gImage_image[0];
	GUI_Image( &image_girl );
	while( 1 );
}

/*
================================================================================
-----------------------------------End of file----------------------------------
================================================================================
*/
