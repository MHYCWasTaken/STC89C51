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
#define _LCD_C_
#include "LCD.H"


#define LCDCOM           0
#define LCDDAT           1

sbit LED_RD = P2^5;
sbit LED_CS = P2^7;
sbit LED_WR = P2^4;
sbit LED_A0 = P2^3;
sbit BLACKLIGHT = P2^0;


void _delay_ms(unsigned char del)
{
	unsigned int i,j;
	for(i=0; i<del; i++)
	for(j=0; j<182; j++)   
	;
}
/*
================================================================================
* Name        : LCD_Init( )
* Description : Initialize the LCD module
* Input       : None
* Output      : None
* Note        : None
================================================================================
*/
void LCDWR( INT8U cmd_dt, INT8U value )
{
	LED_RD = 1;
	LED_CS = 1;
	LED_WR = 1;
	if(cmd_dt)
	{
		LED_A0 = 1;
	}
	else
	{
		LED_A0 = 0;	
	}
	P0 = value;
	LED_CS = 0;
	LED_WR = 0;
	LED_WR = 1;
	LED_CS = 1;
}
/*
================================================================================
* Name        : LCD_Init( )
* Description : Initialize the LCD module
* Input       : None
* Output      : None
* Note        : None
================================================================================
*/
void LCD_Init( void )
{
		LCDWR( LCDCOM, 0x01);//software reset
		_delay_ms(50);       //delay 150ms
		LCDWR( LCDCOM, 0x11);//software reset

		LCDWR( LCDCOM,0x26); // Auto load set
		LCDWR ( LCDDAT,0x04);//auto load disable//0x9f
		LCDWR( LCDCOM,0xf2);  //EE read/write mode
		LCDWR ( LCDDAT,0x00); //set read  mode
		LCDWR( LCDCOM,0xb1);   
		LCDWR ( LCDDAT,0x08);
		LCDWR ( LCDDAT,0x14); 		
		LCDWR( LCDCOM,0xc0);   
		LCDWR ( LCDDAT,0x0a); 
		LCDWR ( LCDDAT,0x05); 
		LCDWR( LCDCOM,0xc1); 
		LCDWR ( LCDDAT,0x02);
		LCDWR( LCDCOM,0xec); 
		LCDWR ( LCDDAT,0x44);
		LCDWR ( LCDDAT,0x52);
		LCDWR( LCDCOM,0xc7); 
		LCDWR ( LCDDAT,0xbf);

//////////


		LCDWR( LCDCOM,0x2a);  //display off
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x7f);
		LCDWR( LCDCOM,0x2b);  //sleep out
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x00);
		LCDWR ( LCDDAT,0x7f);
		LCDWR( LCDCOM,0x36); 
		LCDWR ( LCDDAT,0xc8);
		LCDWR( LCDCOM,0x3a); 
		LCDWR ( LCDDAT,0x55);
		
                //////////////////////////////////////////////////////
		/////VOP set///////////
		LCDWR( LCDCOM,0xf2);  //vo voltage set   
		LCDWR ( LCDDAT,0x01);///////////f5/////  08//00

        LCDWR( LCDCOM,0xe0);   //write contrast for mobile
		LCDWR ( LCDDAT,0x3f);
		LCDWR ( LCDDAT,0x1b);
		LCDWR ( LCDDAT,0x19);
		LCDWR ( LCDDAT,0x27);
		LCDWR ( LCDDAT,0x1f);

		LCDWR ( LCDDAT,0x0d);
		LCDWR ( LCDDAT,0x45);
		LCDWR ( LCDDAT,0xb8);
		LCDWR ( LCDDAT,0x30);

		LCDWR ( LCDDAT,0x17);
		LCDWR ( LCDDAT,0x0f);
		LCDWR ( LCDDAT,0x05);
		LCDWR ( LCDDAT,0x13);

		LCDWR ( LCDDAT,0x02);
		LCDWR ( LCDDAT,0x00);

		//-----------OTPB SET----------------------//
		LCDWR( LCDCOM,0xe1);// bias set
		LCDWR ( LCDDAT,0x00);    //1/12
		LCDWR ( LCDDAT,0x24);

		LCDWR ( LCDDAT,0x26);
		LCDWR ( LCDDAT,0x08);
		LCDWR ( LCDDAT,0x10);
		LCDWR ( LCDDAT,0x12);

		LCDWR ( LCDDAT,0x3a);
		LCDWR ( LCDDAT,0x74);
		LCDWR ( LCDDAT,0x4f);
		LCDWR ( LCDDAT,0x08);

		LCDWR ( LCDDAT,0x20);
		LCDWR ( LCDDAT,0x2a);
		LCDWR ( LCDDAT,0x3c);
		LCDWR ( LCDDAT,0x3d);
		LCDWR ( LCDDAT,0x3f);
		_delay_ms(50);  
		LCDWR( LCDCOM,0x29);   //booster set
		_delay_ms(50);  
		LCDWR( LCDCOM,0x2c);   //booster efficiency set
}
/*
========================================================================================================
Name: LCD_SetArea
Function: Set the area of screen for displaying
Input:
    1.xs :	Start position  X
    2.xe :  End position  X
    3.ys :  Start position Y
    4.ye :  End position Y
Output: None
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void LCDSetArea( INT16U x1, INT16U y1, INT16U x2, INT16U y2 )
{
	x1+=2;
	x2+=2;
	y1+=3;
	y2+=3;	

    LCDWR ( LCDCOM, 0x2A);
	LCDWR ( LCDDAT,  x1>>8);
	LCDWR ( LCDDAT, x1);
	LCDWR ( LCDDAT,  x2>>8);
	LCDWR ( LCDDAT, x2 + 0); 

	LCDWR ( LCDCOM, 0x2B);
	LCDWR ( LCDDAT,  y1>>8);
	LCDWR ( LCDDAT, y1); 
	LCDWR ( LCDDAT,  y2>>8);
	LCDWR ( LCDDAT, y2); 
	LCDWR ( LCDCOM, 0x2C);
}
/*
========================================================================================================
Name: LCDDrawDollop
Function: Draw a dollop at a special area
Input:	1. *dollop : The pointer of a DOLLOP struct
Output: None
Note: The value of the input structrue must be correct, This function does not check the parameters.
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void	LCDDrawDollop( DOLLOP* dollop )
{
	INT8U x,y;

	LCDSetArea(dollop->xs, dollop->ys, dollop->xe, dollop->ye);   	//Set a area at the screen
	for( x = 0; x < dollop->xe - dollop->xs + 1; x ++ )					//Display rows
	{
	 	for( y = 0; y < dollop->ye - dollop->ys + 1; y ++ )	 			//Display columns
		{
		 	LCDWR( LCDDAT, dollop->Color>>8 );
			LCDWR( LCDDAT, dollop->Color );
		}
	}
}
/*
========================================================================================================
Name: LCDDrawPoint( );
Function: Draw a point
Input:	-pPoint, A structure pointer
Output: None
========================================================================================================
*/
void LCDDrawPoint( POINT* pPoint )
{
    LCDSetArea( pPoint->x, pPoint->y, pPoint->x, pPoint->y );
	LCDWR ( LCDDAT,  ( pPoint->Color >> 8 ) & 0xff );
	LCDWR ( LCDDAT,  pPoint->Color & 0xff );
}
/*
========================================================================================================
Name: LCDDrawHRLine( );
Function: Draw a line
Input:	-pLine, A structure pointer to a line
Output: None
========================================================================================================
*/
void LCDDrawHRLine( LINE* pLine )
{
	INT8U x0, x1, y0, y1;
	if( pLine->xs != pLine->xe && pLine->ys != pLine->ye )   return;
	if( pLine->ys > pLine->ye )
	{
		y0 = pLine->ye;
		y1 = pLine->ys;
	}
	else
	{
		y0 = pLine->ys;
		y1 = pLine->ye;
	}
	if( pLine->xs > pLine->xe )
	{
		x0 = pLine->xe;
		x1 = pLine->xs;
	}
	else
	{
		x0 = pLine->xs;
		x1 = pLine->xe;
	}
	LCDSetArea( x0, y0, x1, y1 );
	x0 = x1 - x0;
	if( x0 == 0 ) x0 = y1 - y0;
	for( y0 = 0; y0 < x0; y0 ++ )
	{
		LCDWR( LCDDAT, pLine->Color >> 8 );
		LCDWR( LCDDAT, pLine->Color );
	}
}
/*
========================================================================================================
Name: PrintBitBlock
Function: Print a special block in the screen.
Input:	pBitBlock: This is a pointer, It points to a block whitch contains informations of the block,
	eg. height, width, and so on.
Output: None
Note:   None
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void	PrintBitBlock( BitBlock *pBitBlock )
{
    INT8U	Row, Column;
	INT32U	BytesAbs;
	INT8U	RowBytes;

	LCDSetArea( pBitBlock->xs, pBitBlock->ys,
		pBitBlock->xs + pBitBlock->Width - 1, pBitBlock->ys + pBitBlock->Height - 1 );

	RowBytes = pBitBlock->Width >> 3;
	if( pBitBlock->Width & 0x07 )
	{
	 	RowBytes ++;
	}
	for( Row = 0; Row <  pBitBlock->Height; Row ++ )
	{
	 	for( Column = 0; Column < pBitBlock->Width; Column ++ )
		{
		 	BytesAbs = Row * RowBytes + ( Column >> 3 )	;
			if( *( pBitBlock->pData + BytesAbs )	& ( 1<<( Column & 0x07 )) )
			{
			 	LCDWR( LCDDAT, pBitBlock->Color >> 8 );
				LCDWR( LCDDAT, pBitBlock->Color );
			}
			else
			{
			 	LCDWR( LCDDAT, pBitBlock->BackColor >> 8 );
				LCDWR( LCDDAT, pBitBlock->BackColor );
			}
		}
	}
}
/*
========================================================================================================
Name: GUI_Image( )
Function: Print a image
Input:	point to a struct which contains the informations of the image
Output: None
Note:   None
Author: LiYong
Date  : 2008.08.09
========================================================================================================
*/
void GUI_Image( IMAGE *pImage )
{
    INT8U x, y;
    INT16U datacount = 0;
    LCDSetArea( pImage->xs, pImage->ys, pImage->xs + pImage->length - 1, pImage->ys + pImage->height - 1 );

    for( x = 0; x < pImage->length; x ++ )
    {
        for( y = 0; y < pImage->height; y ++ )
        {
            LCDWR( LCDDAT, *( pImage->pData + datacount++ ) );
            LCDWR( LCDDAT, *( pImage->pData + datacount++ ) );

        }
    }
}
/*
================================================================================
-----------------------------------End of file----------------------------------
================================================================================
*/
