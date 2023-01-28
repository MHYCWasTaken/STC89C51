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
#include "GUI_Basic.H"
#include "GUI_Type.H"
#include "fontlib.h"
/*
================================================================================
Function     : GUI_DrawRectangle( )
Description  : Draw a rectangle
Input        : -pRect, point to a rectangle structure
output       : None
================================================================================
*/
void GUI_DrawRectangle( RECT* pRect )
{
	LINE line;

	line.xs = pRect->xs;
	line.xe = pRect->xe;
	line.ys = pRect->ys;
	line.ye = pRect->ys;
	line.Color = pRect->Color;
	LCDDrawHRLine( &line );

	line.xe = pRect->xs;
	line.ye = pRect->ye;
	LCDDrawHRLine( &line );

	line.xs = pRect->xe;
	line.ys = pRect->ye;
	LCDDrawHRLine( &line );

	line.xe = pRect->xe;
	line.ye = pRect->ys;
	LCDDrawHRLine( &line );
}
/*
================================================================================
Function     : GUI_DrawLine( )
Description  : Draw a line
Input        : -pLine, point to a line structure
output       : None
================================================================================
*/
void GUI_DrawLine( LINE* pLine )
{
	INT32S   dx;						// 直线x轴差值变量
	INT32S   dy;          	// 直线y轴差值变量
	INT32S    dx_sym;				// x轴增长方向，为-1时减值方向，为1时增值方向
	INT32S    dy_sym;				// y轴增长方向，为-1时减值方向，为1时增值方向
	INT32S   dx_x2;					// dx*2值变量，用于加快运算速度
	INT32S   dy_x2;					// dy*2值变量，用于加快运算速度
	INT32S   di;						// 决策变量

	POINT    point;
	LINE     line;

	line.xs = pLine->xs;
	line.ys = pLine->ys;
	line.xe = pLine->xe;
	line.ye = pLine->ye;
	line.Color = pLine->Color;

  point.Color = pLine->Color;

	dx = line.xe - line.xs;
  dy = line.ye - line.ys;

 /* 判断增长方向，或是否为水平线、垂直线、点 */
	if( dx > 0 )					// 判断x轴方向
	{
		dx_sym = 1;					// dx>0，设置dx_sym=1
	}
	else
	{
		if( dx < 0 )
		{
			dx_sym = -1;		  // dx<0，设置dx_sym=-1
		}
		else
		{
			LCDDrawHRLine( &line );
			return;
		}
	}

	if( dy > 0 )							// 判断y轴方向
	{
		dy_sym = 1;					// dy>0，设置dy_sym=1
	}
	else
	{
		if( dy < 0 )
		{
			dy_sym = -1;				// dy<0，设置dy_sym=-1
		}
		else
		{  // dy==0，画水平线，或一点
			LCDDrawHRLine( &line );
			return;
		}
	}

	/* 将dx、dy取绝对值 */
	dx = dx_sym * dx;
	dy = dy_sym * dy;

	/* 计算2倍的dx及dy值 */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

 /* 使用Bresenham法进行画直线 */
	if( dx >= dy )						// 对于dx>=dy，则使用x轴为基准
	{
		di = dy_x2 - dx;
    while( line.xs != line.xe )
    {
			point.x = line.xs;
			point.y = line.ys;
			LCDDrawPoint( &point );
			line.xs += dx_sym;
			if( di < 0 )
			{
				di += dy_x2;			// 计算出下一步的决策值
			}
			else
			{
				di += dy_x2 - dx_x2;
				line.ys += dy_sym;
			}
    }
		LCDDrawPoint( &point );		// 显示最后一点
	}
	else								// 对于dx<dy，则使用y轴为基准
	{
		di = dx_x2 - dy;
    while( line.ys != line.ye )
    {
			point.x = line.xs;
			point.y = line.ys;
			LCDDrawPoint( &point );
			line.ys += dy_sym;
			if(di<0)
			{
				di += dx_x2;
			}
			else
			{
				di += dx_x2 - dy_x2;
				line.xs += dx_sym;
			}
    }
		LCDDrawPoint( &point );		// 显示最后一点
	}
}
/*
================================================================================
Name: PrintFont
Function: Display a character at a special area
Input:	1.Xs : Start position X
		2.Ys : Start position Y
		3.pFont : A pointer of a font structure
		4.Character : The ASCII code of the character.
Output: None
Note: The start position is inputted as a parameter, And the end position is calculated by the FONT
		structure.
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void	GUI_DisplayFont( INT8U	Xs, INT8U Ys, FONT* pFont, char Character )
{
	BitBlock	Block;
	INT32U	Bytes;
	INT8U DataBuffer[64];
	INT8U i;
	const unsigned char *offset;

	Block.Height = pFont->Height;
	Block.Width = pFont->Width;
	Block.Color = pFont->Color;
	Block.BackColor = pFont->BackColor;
	Block.xs = Xs;
	Block.ys = Ys;

	Bytes = pFont->Width >> 3;
	if( pFont->Width & 0x07 )
	{
	 	Bytes ++;
	}
	Bytes *= pFont->Height;
	Bytes *= Character - ' ';

	if( pFont->Height == 18 )
	{
	 	offset = (const unsigned char*)&FontLib_18;
	}
	else if( pFont->Height == 14 )
	{
	 	offset = (const unsigned char*)&FontLib_14;
	}
	else
	{
		return;
	}
	offset += Bytes;
	for( i = 0; i < 36; i ++ )
	{
		//DataBuffer[i] = pgm_read_byte( offset + i );	
	}

	
	Block.pData = DataBuffer;

	PrintBitBlock( &Block );
}
/*
========================================================================================================
Name: DisplayStr
Function: Display a character at a special area
Input:
    1.Xs : Start position X
		2.Ys : Start position Y
		3.pFont : A pointer of a font structure
		4.Str : The start address of a string
Output: None
Note: The start position is inputted as a parameter, And the end position is calculated by the FONT
		structure.
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void	GUI_DisplayStr( INT8U xs, INT8U ys, FONT* pFont, char* Str )
{
	while( *Str )
	{
		GUI_DisplayFont( xs, ys, pFont, *Str );
	 	Str ++;
		xs += pFont->Width;
	}
}
/*
================================================================================
Name: GUI_DrawCircle( )
Function: Display a cycle at a special area
Input:	-pCycle, A pinter point to a cycle structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawCircle( CIRCLE* pCircle )
{
   INT8S  draw_x0, draw_y0;			// 刽图点坐标变量
   INT8S  draw_x1, draw_y1;
   INT8S  draw_x2, draw_y2;
   INT8S  draw_x3, draw_y3;
   INT8S  draw_x4, draw_y4;
   INT8S  draw_x5, draw_y5;
   INT8S  draw_x6, draw_y6;
   INT8S  draw_x7, draw_y7;
   INT8S  xx, yy;					// 画圆控制变量

   INT8S  di;						// 决策变量
   POINT point;

   point.Color = pCircle->Color;

   /* 参数过滤 */
   if(0 == pCircle->r ) return;

   /* 计算出8个特殊点(0、45、90、135、180、225、270度)，进行显示 */
   point.x = draw_x0 = draw_x1 = pCircle->x;
   point.y = draw_y0 = draw_y1 = pCircle->y + pCircle->r;

   if( draw_y0 < GUI_LCM_YMAX ) LCDDrawPoint( &point );	// 90度

   point.x = draw_x2 = draw_x3 = pCircle->x;
   point.y = draw_y2 = draw_y3 = pCircle->y - pCircle->r;
   if( draw_y2 >= 0 ) LCDDrawPoint( &point );			// 270度


   point.x = draw_x4 = draw_x6 = pCircle->x + pCircle->r;
   point.y = draw_y4 = draw_y6 = pCircle->y;
   if(draw_x4<GUI_LCM_XMAX) LCDDrawPoint( &point );	// 0度

   point.x = draw_x5 = draw_x7 = pCircle->x - pCircle->r;
   point.y = draw_y5 = draw_y7 = pCircle->y;
   if(draw_x5>=0) LCDDrawPoint( &point );			// 180度
   if(1==pCircle->r) return;					// 若半径为1，则已圆画完


   /* 使用Bresenham法进行画圆 */
   di = 3 - 2*pCircle->r;					// 初始化决策变量

   xx = 0;
   yy = pCircle->r;
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;
	  }
	  else
	  {  di += 4*(xx - yy) + 10;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;
	  }

	  xx++;
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;


	  /* 要判断当前点是否在有效范围内 */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )
	  {
	  	point.x = draw_x0;
	  	point.y = draw_y0;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x1>=0)&&(draw_y1>=0) )
	  {
	  	point.x = draw_x1;
	  	point.y = draw_y1;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x2;
	  	point.y = draw_y2;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x3;
	  	point.y = draw_y3;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )
	  {
	  	point.x = draw_x4;
	  	point.y = draw_y4;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )
	  {
	  	point.x = draw_x5;
	  	point.y = draw_y5;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x6;
	  	point.y = draw_y6;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x7;
	  	point.y = draw_y7;
	  	LCDDrawPoint( &point );
	  }
	}
}
/*
================================================================================
Name: GUI_DrawCircleFill( )
Function: Display a cycle at a special area and fill its area
Input:	-pCycle, A pinter point to a cycle structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawCircleFill( CIRCLE* pCircle )
{
   INT8S  draw_x0, draw_y0;			// 刽图点坐标变量
   INT8S  draw_x1, draw_y1;
   INT8S  draw_x2, draw_y2;
   INT8S  draw_x3, draw_y3;
   INT8S  draw_x4, draw_y4;
   INT8S  draw_x5, draw_y5;
   INT8S  draw_x6, draw_y6;
   INT8S  draw_x7, draw_y7;
   INT8S  fill_x0, fill_y0;			// 填充所需的变量，使用垂直线填充
   INT8S  fill_x1;
   INT8S  xx, yy;					// 画圆控制变量

   INT8S  di;						// 决策变量
   POINT   point;
   LINE    line;

   point.Color = pCircle->Color;
   line.Color = pCircle->Color;



   /* 参数过滤 */
   if(0==pCircle->r) return;

   /* 计算出4个特殊点(0、90、180、270度)，进行显示 */
   point.x = draw_x0 = draw_x1 = pCircle->x;
   point.y = draw_y0 = draw_y1 = pCircle->y + pCircle->r;
   if(draw_y0<GUI_LCM_YMAX)
   {
      LCDDrawPoint( &point );
   }

   point.x = draw_x2 = draw_x3 = pCircle->x;
   point.y = draw_y2 = draw_y3 = pCircle->y - pCircle->r;
   if(draw_y2>=0)
   {
   		LCDDrawPoint( &point );
   }

   point.x = draw_x4 = draw_x6 = pCircle->x + pCircle->r;
   point.y = draw_y4 = draw_y6 = pCircle->y;
   if(draw_x4<GUI_LCM_XMAX)
   {
   		LCDDrawPoint( &point );	// 0度
      fill_x1 = draw_x4;
   }
   else
   {
   		fill_x1 = GUI_LCM_XMAX;
   }
   fill_y0 = pCircle->y;							// 设置填充线条起始点fill_x0
   fill_x0 = pCircle->x - pCircle->r;						// 设置填充线条结束点fill_y1
   if(fill_x0<0) fill_x0 = 0;
	 line.xs = fill_x0;
	 line.ys = fill_y0;
	 line.ye = fill_y0;
	 line.xe = fill_x1;
   LCDDrawHRLine( &line );

   point.x = draw_x5 = draw_x7 = pCircle->x - pCircle->r;
   point.y = draw_y5 = draw_y7 = pCircle->y;
   if(draw_x5>=0)
   {
   		LCDDrawPoint( &point );	// 180度
   }
   if(1==pCircle->r) return;


   /* 使用Bresenham法进行画圆 */
   di = 3 - 2*pCircle->r;							// 初始化决策变量

   xx = 0;
   yy = pCircle->r;
   while(xx<yy)
   {  if(di<0)
	  {  di += 4*xx + 6;
	  }
	  else
	  {  di += 4*(xx - yy) + 10;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
		 draw_x4--;
		 draw_x5++;
		 draw_x6--;
		 draw_x7++;
	  }

	  xx++;
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
	  draw_y4++;
	  draw_y5++;
	  draw_y6--;
	  draw_y7--;


	  /* 要判断当前点是否在有效范围内 */
	  if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )
	  {
	  	point.x = draw_x0;
	  	point.y = draw_y0;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x1>=0)&&(draw_y1>=0) )
	  {
	  	point.x = draw_x1;
	  	point.y = draw_y1;
	  	LCDDrawPoint( &point );
	  }

	  /* 第二点水直线填充(下半圆的点) */
	  if(draw_x1>=0)
	  {  /* 设置填充线条起始点fill_x0 */
	     fill_x0 = draw_x1;
	     /* 设置填充线条起始点fill_y0 */
	     fill_y0 = draw_y1;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* 设置填充线条结束点fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x1;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;

         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x2;
	  	point.y = draw_y2;
	  	LCDDrawPoint( &point );
	  }

	  if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x3;
	  	point.y = draw_y3;
	  	LCDDrawPoint( &point );
	  }

	  /* 第四点垂直线填充(上半圆的点) */
	  if(draw_x3>=0)
	  {  /* 设置填充线条起始点fill_x0 */
	     fill_x0 = draw_x3;
	     /* 设置填充线条起始点fill_y0 */
	     fill_y0 = draw_y3;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* 设置填充线条结束点fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x3;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;

         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )
	  {
	  	point.x = draw_x4;
	  	point.y = draw_y4;
	  	LCDDrawPoint( &point );
	  }
	  if( (draw_x5>=0)&&(draw_y5>=0) )
	  {
	  	point.x = draw_x5;
	  	point.y = draw_y5;
	  	LCDDrawPoint( &point );
	  }

	  /* 第六点垂直线填充(上半圆的点) */
	  if(draw_x5>=0)
	  {  /* 设置填充线条起始点fill_x0 */
	     fill_x0 = draw_x5;
	     /* 设置填充线条起始点fill_y0 */
	     fill_y0 = draw_y5;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* 设置填充线条结束点fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x5;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }


	  if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x6;
	  	point.y = draw_y6;
	  	LCDDrawPoint( &point );
	  }

	  if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )
	  {
	  	point.x = draw_x7;
	  	point.y = draw_y7;
	  	LCDDrawPoint( &point );
	  }

	  /* 第八点垂直线填充(上半圆的点) */
	  if(draw_x7>=0)
	  {  /* 设置填充线条起始点fill_x0 */
	     fill_x0 = draw_x7;
	     /* 设置填充线条起始点fill_y0 */
	     fill_y0 = draw_y7;
         if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
         if(fill_y0<0) fill_y0 = 0;
         /* 设置填充线条结束点fill_x1 */
         fill_x1 = pCircle->x*2 - draw_x7;
         if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
         	line.xs = fill_x0;
         	line.xe = fill_x1;
         	line.ys = line.ye = fill_y0;
         	LCDDrawHRLine( &line );
      }

   }
}
/*
================================================================================
Name: GUI_DrawEllipse( )
Function: Display a ellipse at a special area
Input:	-pCycle, A pinter point to a ellipse structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawEllipse( ELLIPSE* pEllipse )
{
   INT32S  draw_x0, draw_y0;			// 刽图点坐标变量
   INT32S  draw_x1, draw_y1;
   INT32S  draw_x2, draw_y2;
   INT32S  draw_x3, draw_y3;
   INT32S  xx, yy;					// 画图控制变量

   INT32S  center_x, center_y;		// 椭圆中心点坐标变量
   INT32S  radius_x, radius_y;		// 椭圆的半径，x轴半径和y轴半径
   INT32S  radius_xx, radius_yy;		// 半径乘平方值
   INT32S  radius_xx2, radius_yy2;	// 半径乘平方值的两倍
   INT32S  di;						// 定义决策变量

   POINT   point;

   point.Color = pEllipse->Color;

   /* 参数过滤 */
   if( (pEllipse->xs == pEllipse->xe ) ||
   	   (pEllipse->ys == pEllipse->ye ) ) return;

   /* 计算出椭圆中心点坐标 */
   center_x = (pEllipse->xs + pEllipse->xe) >> 1;
   center_y = (pEllipse->ys + pEllipse->ye) >> 1;

   /* 计算出椭圆的半径，x轴半径和y轴半径 */
   if(pEllipse->xs > pEllipse->xe)
   {  radius_x = (pEllipse->xs - pEllipse->xe) >> 1;
   }
   else
   {  radius_x = (pEllipse->xe - pEllipse->xs) >> 1;
   }
   if(pEllipse->ys > pEllipse->ye)
   {  radius_y = (pEllipse->ys - pEllipse->ye) >> 1;
   }
   else
   {  radius_y = (pEllipse->ye - pEllipse->ys) >> 1;
   }

   /* 计算半径平方值 */
   radius_xx = radius_x * radius_x;
   radius_yy = radius_y * radius_y;

   /* 计算半径平方值乘2值 */
   radius_xx2 = radius_xx<<1;
   radius_yy2 = radius_yy<<1;

   /* 初始化画图变量 */
   xx = 0;
   yy = radius_y;

   di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化决策变量

   /* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
   draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
   draw_y0 = draw_y1 = center_y + radius_y;
   draw_y2 = draw_y3 = center_y - radius_y;

	 point.x = draw_x0;
	 point.y = draw_y0;
	 LCDDrawPoint( &point );
	 point.x = draw_x2;
	 point.y = draw_y2;
	 LCDDrawPoint( &point );	// 画y轴上的两个端点

   while( (radius_yy*xx) < (radius_xx*yy) )
   {  if(di<0)
	  {  di+= radius_yy2*(2*xx+3);
	  }
	  else
	  {  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
	  }

	  xx ++;						// x轴加1

	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );
   }

   di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
   while(yy>=0)
   {  if(di<0)
	  {  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

	     xx ++;						// x轴加1
	     draw_x0++;
	     draw_x1--;
	     draw_x2++;
	     draw_x3--;
	  }
	  else
	  {  di += radius_xx2*3 - 2*radius_xx2*yy;
	  }

	  yy--;
 	  draw_y0--;
	  draw_y1--;
	  draw_y2++;
	  draw_y3++;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );
   }
}
/*
================================================================================
Name: GUI_DrawEllipseFill( )
Function: Display a ellipse at a special area and fill its area
Input:	-pCycle, A pinter point to a ellipse structure
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void  GUI_DrawEllipseFill( ELLIPSE* pEllipse )
{
	INT32S  draw_x0, draw_y0;			// 刽图点坐标变量
	INT32S  draw_x1, draw_y1;
	INT32S  draw_x2, draw_y2;
	INT32S  draw_x3, draw_y3;
	INT32S  xx, yy;					      // 画图控制变量

	INT32S  center_x, center_y;		// 椭圆中心点坐标变量
	INT32S  radius_x, radius_y;		// 椭圆的半径，x轴半径和y轴半径
	INT32S  radius_xx, radius_yy;		// 半径乘平方值
	INT32S  radius_xx2, radius_yy2;	// 半径乘平方值的两倍
	INT32S  di;						// 定义决策变量

	POINT point;
	LINE  line;

	point.Color = pEllipse->Color;
	line.Color = pEllipse->Color;

	/* 参数过滤 */
	if( (pEllipse->xs==pEllipse->xe) ||
	    (pEllipse->ys==pEllipse->ye) ) return;

	/* 计算出椭圆中心点坐标 */
	center_x = (pEllipse->xs + pEllipse->xe) >> 1;
	center_y = (pEllipse->ys + pEllipse->ye) >> 1;

	/* 计算出椭圆的半径，x轴半径和y轴半径 */
	if(pEllipse->xs > pEllipse->xe)
	{
		radius_x = (pEllipse->xs - pEllipse->xe) >> 1;
	}
	else
	{
		radius_x = (pEllipse->xe - pEllipse->xs) >> 1;
	}
	if(pEllipse->ys > pEllipse->ye)
	{
		radius_y = (pEllipse->ys - pEllipse->ye) >> 1;
	}
	else
	{
		radius_y = (pEllipse->ye - pEllipse->ys) >> 1;
	}

	/* 计算半径乘平方值 */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* 计算半径乘4值 */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* 初始化画图变量 */
	xx = 0;
	yy = radius_y;

	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化决策变量

	/* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;

	point.x = draw_x0;
	point.y = draw_y0;
	LCDDrawPoint( &point );
	point.x = draw_x2;
	point.y = draw_y2;
	LCDDrawPoint( &point );// 画y轴上的两个端点

   while( (radius_yy*xx) < (radius_xx*yy) )
   {  if(di<0)
	  {  di+= radius_yy2*(2*xx+3);
	  }
	  else
	  {  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;

	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;
	  }

	  xx ++;						// x轴加1

	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );

	  /* 若y轴已变化，进行填充 */
	  if(di>=0)
	  {
	  	line.xs = draw_x0;
	  	line.xe = draw_x1;
	  	line.ys = line.ye = draw_y0;
	  	LCDDrawHRLine( &line );
	  	line.xs = draw_x2;
	  	line.xe = draw_x3;
	  	line.ys = line.ye = draw_y2;
	  	LCDDrawHRLine( &line );

	  }
   }

   di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy +
        radius_yy2*xx - radius_xx2*radius_yy;
   while(yy>=0)
   {  if(di<0)
	  {  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

	     xx ++;						// x轴加1
	     draw_x0++;
	     draw_x1--;
	     draw_x2++;
	     draw_x3--;
	  }
		else
		{  di += radius_xx2*3 - 2*radius_xx2*yy;
		}

		yy--;
		draw_y0--;
		draw_y1--;
		draw_y2++;
		draw_y3++;

		point.x = draw_x0;
		point.y = draw_y0;
		LCDDrawPoint( &point );
		point.x = draw_x1;
		point.y = draw_y1;
		LCDDrawPoint( &point );
		point.x = draw_x2;
		point.y = draw_y2;
		LCDDrawPoint( &point );
		point.x = draw_x3;
		point.y = draw_y3;
		LCDDrawPoint( &point );

	  /* y轴已变化，进行填充 */
		line.xs = draw_x0;
		line.xe = draw_x1;
		line.ys = line.ye = draw_y0;
		LCDDrawHRLine( &line );
		line.xs = draw_x2;
		line.xe = draw_x3;
		line.ys = line.ye = draw_y2;
		LCDDrawHRLine( &line );
   }
}
/*
================================================================================
Name: GUI_Inital( )
Function: Initialize GUI with single color
Input:	-Color, Initialize color
Output: None
Author: LiYong
Date  : 2008.08.09
================================================================================
*/
void GUI_Inital( TCOLOR Color )
{
	DOLLOP dollop;

	dollop.xs = 0;
	dollop.xe = GUI_LCM_XMAX;
	dollop.ys = 0;
	dollop.ye = GUI_LCM_YMAX;
	dollop.Color = Color;

	LCDDrawDollop( &dollop );
}	
/*
================================================================================
=======================================End of file==============================
================================================================================
*/


