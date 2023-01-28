/*
================================================================================
File Name : GUI_Type.H
Author    : LiYOng
Date      : 2008-12-12 15:51
Version   : 1.0
Decription: This file offers some data type for GUI module
================================================================================
*/
#ifndef  _GUI_TYPE_H_
#define  _GUI_TYPE_H_

#include "GUI_Config.H"

typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;


/*Informations of a point*/
typedef struct
{
	INT8U x;
	INT8U y;
	TCOLOR Color;
}POINT;

/*Informations of a line*/
typedef struct
{
	INT8U xs;
	INT8U xe;
	INT8U ys;
	INT8U ye;
	TCOLOR Color;
}LINE;

/*Informations of a dollop*/
typedef	struct
{
	INT8U	xs;
	INT8U	xe;
	INT8U	ys;
	INT8U	ye;
	TCOLOR	Color;
}DOLLOP;

/*Informations of a character*/
typedef struct
{
	INT8U	Height;
	INT8U	Width;
	TCOLOR	Color;
	TCOLOR  BackColor;
}FONT;

/*Informations of a bit block*/
typedef struct
{
	INT8U	xs;
	INT8U	ys;
	INT8U	Width;
	INT8U	Height;
	TCOLOR	Color;
	TCOLOR  BackColor;
	INT8U   *pData;
}BitBlock;

/*Informations of a rectangle*/
typedef struct
{
	INT8U xs;
	INT8U ys;
	INT8U xe;
	INT8U ye;
	TCOLOR Color;
}RECT;

/*Informations of a circle*/
typedef struct
{
	INT8U x;
	INT8U y;
	INT8U r;
	TCOLOR Color;
}CIRCLE;

/*Informations of a ellipse*/
typedef struct
{
	INT8U xs;
	INT8U xe;
	INT8U ys;
	INT8U ye;
	TCOLOR Color;
}ELLIPSE;

/*Informations of a image*/
typedef struct
{
    INT8U xs;
    INT8U ys;
    INT8U height;
    INT8U length;
    INT8U *pData;
}IMAGE;

#endif//_GUI_TYPE_H_
/*
================================================================================
====================================End of file=================================
================================================================================
*/
