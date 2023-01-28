/*
================================================================================
File Name : GUI_Basic.c
Author    : LiYOng
Date      : 2008-12-12 15:51
Version   : 1.0
Decription: This file contains some basic functions for GUI, It need the LCD
            Drive functions            
================================================================================
*/
#ifndef _GUI_BASIC_H_
#define _GUI_BASIC_H_

#include "GUI_Type.h"

/*
================================================================================
Important Notes :
There are servral GUI functions in this file, The low level functions related 
to the LCD harware and the BitMap / FontLib, You mast use a special method to 
catch them, The method must be :
	 Catch By ROWs:
	(BYTE0)7,6,5,4,3,2,1,0,(BYTE1)7,6,5,4,3,2,1,0,(BYTE2)7,6,5,4...
	(BYTE3)7,6,5,4,3,2,1,0,(BYTE4)7,6,5,4,3,2,1,0,(BYTE5)7,6,5,4...
	.
	.
	.
	LEFT --> RIGHT,  TOP --> BOTTOM
if you use another font libs, you must change the functions to support them.
================================================================================
*/
/*
================================================================================
External IMPORT function list, you must offer all of them as LCD driver. About the
data type, Please refer to GUI_Type.H

extern void	LCDDrawDollop( DOLLOP* dollop ); //draw a dollop with single color
extern void LCDDrawPoint( POINT* pPoint );//draw a point
extern void LCDDrawHRLine( LINE* pLine ); //draw a direct line
extern void	PrintBitBlock( BitBlock *pBitBlock );//draw a bitmap with single color          
================================================================================
*/
#include <LCD.H>
/*
================================================================================
Export function list          
================================================================================
*/
/*Initialize GUI with single color*/
void GUI_Inital( TCOLOR Color );

/*Draw a rectangle*/
void GUI_DrawRectangle( RECT* pRect );

/*Draw a line*/
void GUI_DrawLine( LINE* pLine );

/*Display a character*/
void GUI_DisplayFont( INT8U	Xs, INT8U Ys, FONT* pFont, char Character );

/*Display a string*/
void GUI_DisplayStr( INT8U xs, INT8U ys, FONT* pFont, char* Str ); 
 
/*Draw a cycle*/
void GUI_DrawCircle( CIRCLE* pCircle );

/*Draw a cycle and fill its color*/
void  GUI_DrawCircleFill( CIRCLE* pCircle );

/*Draw a ellipse*/
void  GUI_DrawEllipse( ELLIPSE* pEllipse );

/*Draw a ellipse and fill its color*/
void  GUI_DrawEllipseFill( ELLIPSE* pEllipse );

/*Draw a line*/
void GUI_DrawLine( LINE* pLine );

#endif//_GUI_BASIC_H_
/*
================================================================================
=======================================End of file==============================
================================================================================
*/

