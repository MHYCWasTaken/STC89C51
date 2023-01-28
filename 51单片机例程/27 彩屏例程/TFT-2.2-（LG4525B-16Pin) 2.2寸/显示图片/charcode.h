#ifndef __CHARCODE_H
#define __CHARCODE_H

#ifdef CHAR32_SHOW

// ------------------  汉字字模的数据结构定义 ------------------------ //
struct  Cn32CharTypeDef                   // 汉字字模数据结构 
{
	unsigned char  Index[2];            // 汉字内码索引,一个汉字占两个字节	
	unsigned char   Msk[116];            // 点阵码数据(32*29/8) 
};
/////////////////////////////////////////////////////////////////////////
// 汉字字模表,大小为：32*29                                                          //
// 汉字库: 楷体二号,横向取模左高位,数据排列:从左到右从上到下           //
/////////////////////////////////////////////////////////////////////////
struct  Cn32CharTypeDef code CnChar32x29[]=
{
/*--  文字:  普  --*/
/*--  楷体_GB231222;  此字体下对应的点阵为：宽x高=30x29   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x29  --*/
"普",0x00,0x00,0x18,0x00,0x00,0x60,0x38,0x00,0x00,0x70,0x78,0x00,0x00,0x38,0xF0,0x00,
0x00,0x39,0xC0,0x00,0x00,0x11,0xFC,0x00,0x00,0xFF,0xFC,0x00,0x00,0xFF,0x80,0x00,
0x00,0x19,0x8C,0x00,0x00,0x19,0x9C,0x00,0x03,0x99,0xB8,0x00,0x01,0xD9,0xF0,0x00,
0x01,0xD9,0xC1,0xC0,0x00,0x1B,0xFF,0xF0,0x07,0xFF,0xFF,0xF0,0xFF,0xF0,0x00,0x00,
0x78,0x00,0x40,0x00,0x00,0xDF,0xF0,0x00,0x00,0xFF,0xF0,0x00,0x00,0xC0,0x70,0x00,
0x00,0xCF,0x70,0x00,0x00,0xFF,0x70,0x00,0x00,0xD0,0x70,0x00,0x00,0xC0,0x70,0x00,
0x00,0xC0,0x70,0x00,0x00,0xFF,0xF0,0x00,0x00,0x7F,0xE0,0x00,0x00,0x40,0x60,0x00,
0x00,0x00,0x60,0x00,

/*--  文字:  中  --*/
/*--  楷体_GB231222;  此字体下对应的点阵为：宽x高=30x29   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x29  --*/
"中",0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x0E,0x00,0x00,
0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0xFE,0x00,
0x0F,0xFF,0xFF,0x00,0x0F,0xFE,0x0F,0x00,0x07,0x06,0x0E,0x00,0x07,0x06,0x0E,0x00,
0x07,0x06,0x0C,0x00,0x03,0x06,0x1C,0x00,0x03,0x0F,0xFC,0x00,0x03,0xFF,0xFC,0x00,
0x03,0xF6,0x00,0x00,0x01,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,
0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,
0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x00,
0x00,0x06,0x00,0x00,

/*--  文字:  科  --*/
/*--  楷体_GB231222;  此字体下对应的点阵为：宽x高=30x29   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x29  --*/
"科",0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x0E,0x00,0x00,0x1C,0x0E,0x00,
0x00,0x3C,0x0E,0x00,0x00,0xF8,0x0E,0x00,0x03,0xE0,0x8E,0x00,0x06,0x61,0xEE,0x00,
0x00,0x60,0x7E,0x00,0x00,0x6C,0x0E,0x00,0x00,0x7E,0x0E,0x00,0x0F,0xF8,0xE6,0x00,
0x7F,0xE0,0x66,0x00,0x3D,0xF8,0x0E,0x60,0x01,0xFC,0x0F,0xF8,0x03,0xEC,0xFF,0xF8,
0x07,0x67,0xFE,0x00,0x0E,0x63,0x0E,0x00,0x1C,0x60,0x0E,0x00,0x38,0x60,0x0E,0x00,
0x70,0x60,0x0E,0x00,0x40,0xE0,0x0E,0x00,0x00,0xE0,0x0E,0x00,0x00,0xC0,0x0E,0x00,
0x00,0xC0,0x0E,0x00,0x00,0xC0,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x00,
0x00,0x00,0x04,0x00,

/*--  文字:  技  --*/
/*--  楷体_GB231222;  此字体下对应的点阵为：宽x高=30x29   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x29  --*/
"技",0x00,0x00,0x00,0x00,0x03,0x00,0xC0,0x00,0x03,0x80,0xF0,0x00,0x01,0xC0,0x70,0x00,
0x01,0x80,0x60,0x00,0x01,0x80,0x60,0x00,0x01,0x80,0x60,0x00,0x01,0x80,0x7F,0x00,
0x01,0xA1,0xFF,0x00,0x03,0xF7,0xF0,0x00,0x3F,0xE3,0xC0,0x00,0x3F,0x80,0xC0,0x00,
0x01,0x80,0xC8,0x00,0x01,0xA0,0xFC,0x00,0x01,0xCF,0xFC,0x00,0x03,0x8E,0x18,0x00,
0x07,0x8E,0x38,0x00,0x1F,0x87,0x30,0x00,0xFD,0x83,0xF0,0x00,0xF1,0x80,0xE0,0x00,
0x61,0x80,0xF0,0x00,0x01,0x83,0xBC,0x00,0x01,0x87,0x1F,0x00,0x0F,0x9E,0x07,0xE0,
0x07,0xB8,0x03,0xF8,0x03,0x00,0x01,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,

};

#endif

#ifdef CHAR14_SHOW 
// ------------------  汉字字模的数据结构定义 ------------------------ //
struct  Cn14CharTypeDef                   // 汉字字模数据结构 
{
	unsigned char  Index[2];            // 汉字内码索引,一个汉字占两个字节	
	unsigned char   Msk[57];            // 点阵码数据(19*24/8) 
};
/////////////////////////////////////////////////////////////////////////
// 汉字字模表,大小为：19X24                                                          //
// 汉字库: 宋体14,横向取模左高位,数据排列:从左到右从上到下           //
/////////////////////////////////////////////////////////////////////////
struct  Cn14CharTypeDef code CnChar19x24[]=
{
/*--  文字:  年  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"年",0x04,0x00,0x00,0x06,0x00,0x00,0x04,0x00,0x00,0x08,0x01,0x80,0x17,0xFE,0x00,0x10,
0x20,0x00,0x20,0x20,0x00,0x48,0x23,0x00,0x0F,0xFC,0x00,0x08,0x20,0x00,0x08,0x20,
0x00,0x08,0x20,0x80,0x77,0xFF,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,
0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x00,0x00,

/*--  文字:  月  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"月",0x00,0x00,0x00,0x00,0x02,0x00,0x03,0xFE,0x00,0x02,0x02,0x00,0x02,0x02,0x00,0x02,
0x02,0x00,0x03,0xFE,0x00,0x02,0x02,0x00,0x02,0x02,0x00,0x02,0x02,0x00,0x03,0xFE,
0x00,0x04,0x02,0x00,0x04,0x02,0x00,0x04,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,
0x10,0x1E,0x00,0x20,0x04,0x00,0x00,0x00,0x00,

/*--  文字:  日  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"日",0x00,0x00,0x00,0x00,0x02,0x00,0x0F,0xFE,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,
0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x0F,0xFE,0x00,0x08,0x02,0x00,0x08,0x02,
0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x0F,0xFE,0x00,
0x08,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  星  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"星",0x00,0x00,0x00,0x08,0x02,0x00,0x0F,0xFE,0x00,0x08,0x02,0x00,0x0F,0xFE,0x00,0x08,
0x02,0x00,0x08,0x02,0x00,0x0F,0xFE,0x00,0x08,0x60,0x00,0x0C,0x40,0x00,0x0F,0xFF,
0x00,0x10,0x40,0x00,0x20,0x40,0x00,0x4F,0xFE,0x00,0x00,0x40,0x00,0x00,0x40,0x80,
0x3F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  期  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"期",0x00,0x00,0x00,0x08,0x80,0x00,0x08,0x8F,0xC0,0x08,0xC8,0x80,0x3F,0x88,0x80,0x08,
0x88,0x80,0x0F,0x8F,0x80,0x08,0x88,0x80,0x08,0x88,0x80,0x0F,0x88,0x80,0x08,0x90,
0x80,0x08,0xDF,0x80,0x3F,0x30,0x80,0x0C,0x90,0x80,0x08,0xA0,0x80,0x10,0xA0,0x80,
0x20,0x43,0x80,0x01,0x81,0x00,0x00,0x00,0x00,

/*--  文字:  摄  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"摄",0x00,0x00,0x00,0x08,0x00,0x80,0x08,0xFF,0x00,0x08,0x42,0x00,0x7E,0x7E,0x00,0x08,
0x42,0x00,0x08,0x7E,0x00,0x0B,0xFF,0xC0,0x0C,0x02,0x00,0x38,0x02,0x00,0x4B,0xFD,
0x80,0x0A,0x29,0x00,0x09,0xC9,0x00,0x08,0x46,0x00,0x08,0xE6,0x00,0x09,0x2B,0x00,
0x3A,0x11,0xC0,0x14,0x60,0x00,0x00,0x00,0x00,

/*--  文字:  氏  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"氏",0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x1C,0x00,0x17,0xE0,0x00,0x18,0x40,0x00,0x10,
0x40,0x00,0x10,0x40,0x00,0x10,0x41,0x80,0x1F,0xFE,0x00,0x10,0x60,0x00,0x10,0x20,
0x00,0x10,0x20,0x00,0x10,0x30,0x00,0x10,0x58,0x00,0x11,0x8C,0x80,0x1E,0x06,0x80,
0x18,0x03,0x80,0x10,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  度  --*/
/*--  宋体14;  此字体下对应的点阵为：宽x高=19x19   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=24x19  --*/
"度",0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x20,0x80,0x1F,0xDF,0x00,0x10,0x84,0x00,0x10,
0x84,0x00,0x1F,0xFF,0x80,0x10,0x84,0x00,0x10,0x84,0x00,0x10,0xFC,0x00,0x10,0x04,
0x00,0x11,0xFE,0x00,0x20,0x8C,0x00,0x20,0x48,0x00,0x20,0x30,0x00,0x40,0x78,0x00,
0x41,0x87,0xC0,0x0E,0x00,0x80,0x00,0x00,0x00,
};

#endif

#ifdef USE_ASCII
/////////////////////////////////////////////////////////////////////////
// ASCII码表,大小为：16*24                                                         //
// 是从空格开始的（也就是32开始的，用ASII码查的时候减去32 ）           //
/////////////////////////////////////////////////////////////////////////

const unsigned char code ASCII16x24[96][48] =
{ 
	/*--  文字:     --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  !  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,
	0x07,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  "  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x06,0x60,0x06,0x60,0x0C,0xC0,0x19,0x80,0x11,0x00,0x22,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},		
	
	/*--  文字:  #  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x40,0x10,0x40,0x10,0x40,
	0x10,0x40,0x7F,0xE0,0x7F,0xE0,0x10,0x40,0x10,0x40,0x20,0x80,0x20,0x80,0x7F,0xE0,
	0x7F,0xE0,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  $  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x00,0x0F,0x80,0x1A,0xC0,0x32,0xC0,
	0x32,0xC0,0x32,0x00,0x1A,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x03,0x80,0x02,0xC0,
	0x32,0xC0,0x32,0xC0,0x32,0xC0,0x12,0x80,0x0F,0x00,0x02,0x00,0x02,0x00,0x00,0x00,},
	
	/*--  文字:  %  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0xC0,0x50,0x80,0x88,0x80,
	0x89,0x00,0x89,0x00,0x8B,0x00,0x8A,0x00,0x5F,0xC0,0x75,0x40,0x06,0x20,0x0A,0x20,
	0x0A,0x20,0x1A,0x20,0x12,0x20,0x11,0x40,0x21,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  &  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x00,0x26,0x00,0x26,0x00,
	0x26,0x00,0x26,0x00,0x25,0xC0,0x38,0x80,0x30,0x80,0x58,0x80,0x49,0x00,0xCD,0x00,
	0xC7,0x00,0xC6,0x00,0xC3,0x00,0x65,0x20,0x38,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  '  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x30,0x00,0x38,0x00,0x08,0x00,0x08,0x00,0x10,0x00,0x60,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  (  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x80,0x01,0x00,0x03,0x00,
	0x03,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x03,0x00,0x03,0x00,0x01,0x00,0x01,0x80,0x00,0x80,0x00,0x40,0x00,0x20,0x00,0x00,},
	
	/*--  文字:  )  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x40,0x00,0x20,0x00,0x10,0x00,0x18,0x00,0x08,0x00,0x0C,0x00,
	0x0C,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x0C,0x00,0x0C,0x00,0x08,0x00,0x18,0x00,0x10,0x00,0x20,0x00,0x40,0x00,0x00,0x00,},
	
	/*--  文字:  *  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x00,
	0x02,0x00,0x72,0x70,0x3A,0xE0,0x0F,0x80,0x0F,0x80,0x3A,0xE0,0x72,0x70,0x02,0x00,
	0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  +  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
	0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x7F,0xF0,0x02,0x00,0x02,0x00,0x02,0x00,
	0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  ,  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x30,0x00,0x38,0x00,0x08,0x00,0x08,0x00,0x10,0x00,0x60,0x00,},
	
	/*--  文字:  -  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  .  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x38,0x00,0x38,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  /  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x60,0x00,0x40,0x00,0xC0,0x00,0x80,0x00,0x80,
	0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x06,0x00,0x04,0x00,0x04,0x00,0x08,0x00,
	0x08,0x00,0x10,0x00,0x10,0x00,0x30,0x00,0x20,0x00,0x60,0x00,0x40,0x00,0x00,0x00,},
	
	/*--  文字:  0  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,
	0x30,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x30,0xC0,0x30,0xC0,0x19,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  1  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x06,0x00,0x3E,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  2  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x21,0x80,0x40,0xC0,
	0x60,0xC0,0x60,0xC0,0x00,0xC0,0x01,0x80,0x01,0x80,0x03,0x00,0x04,0x00,0x08,0x00,
	0x10,0x40,0x20,0x40,0x40,0x40,0x7F,0xC0,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  3  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x23,0x00,0x61,0x80,
	0x61,0x80,0x61,0x80,0x01,0x80,0x03,0x00,0x0E,0x00,0x01,0x80,0x00,0x80,0x00,0xC0,
	0x60,0xC0,0x60,0xC0,0x60,0xC0,0x21,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  4  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x03,0x80,0x03,0x80,
	0x05,0x80,0x09,0x80,0x09,0x80,0x11,0x80,0x21,0x80,0x21,0x80,0x41,0x80,0x7F,0xE0,
	0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  5  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0x20,0x00,
	0x20,0x00,0x20,0x00,0x20,0x00,0x2F,0x00,0x31,0x80,0x20,0xC0,0x00,0xC0,0x00,0xC0,
	0x60,0xC0,0x60,0xC0,0x41,0x80,0x21,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  },
	
	/*--  文字:  6  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x80,0x18,0xC0,0x30,0xC0,
	0x30,0x00,0x20,0x00,0x60,0x00,0x67,0x80,0x68,0xC0,0x70,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x20,0x60,0x30,0x40,0x18,0xC0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  7  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x3F,0xE0,0x30,0x40,
	0x20,0x80,0x20,0x80,0x00,0x80,0x01,0x00,0x01,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  8  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x30,0xC0,0x60,0x60,
	0x60,0x60,0x60,0x60,0x70,0x40,0x3C,0xC0,0x0F,0x00,0x33,0x80,0x20,0xC0,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x30,0xC0,0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  9  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x30,0x80,0x30,0xC0,
	0x60,0x40,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0x31,0x60,0x1E,0x60,0x00,0x60,
	0x00,0xC0,0x00,0xC0,0x30,0x80,0x31,0x80,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  :  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  ;  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x02,0x00,0x04,0x00,0x00,0x00, },
	
	/*--  文字:  <  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x00,
	0x02,0x00,0x04,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x10,0x00,0x08,0x00,0x04,0x00,
	0x02,0x00,0x01,0x00,0x00,0x80,0x00,0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  =  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  >  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x10,0x00,0x08,0x00,0x04,0x00,
	0x02,0x00,0x01,0x00,0x00,0x80,0x00,0x40,0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x00,
	0x02,0x00,0x04,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  ?  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,0x30,0xC0,0x40,0x60,0x40,0x60,
	0x60,0x60,0x60,0x60,0x00,0xC0,0x01,0x80,0x06,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
	0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  @  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x80,0x1C,0x60,0x10,0x20,
	0x33,0x50,0x26,0xD0,0x64,0x90,0x64,0x90,0x68,0x90,0x68,0x90,0x69,0x90,0x69,0xA0,
	0x66,0xC0,0x30,0x10,0x30,0x20,0x18,0x40,0x07,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  A  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x0E,0x00,
	0x0B,0x00,0x0B,0x00,0x13,0x00,0x11,0x80,0x11,0x80,0x11,0x80,0x1F,0x80,0x20,0xC0,
	0x20,0xC0,0x20,0xC0,0x20,0x60,0x60,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  B  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x61,0x80,0x60,0xC0,
	0x60,0xC0,0x60,0xC0,0x60,0xC0,0x61,0x80,0x7F,0x00,0x60,0xC0,0x60,0x40,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  C  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x18,0x60,0x30,0x20,
	0x30,0x20,0x20,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
	0x60,0x20,0x30,0x20,0x30,0x40,0x18,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  D  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x61,0x80,0x60,0xC0,
	0x60,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x60,0xC0,0x60,0xC0,0x63,0x80,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  E  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xC0,0x60,0x40,0x60,0x20,
	0x60,0x00,0x60,0x00,0x61,0x00,0x61,0x00,0x7F,0x00,0x61,0x00,0x61,0x00,0x60,0x00,
	0x60,0x00,0x60,0x20,0x60,0x20,0x60,0x40,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  F  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xC0,0x60,0xC0,0x60,0x20,
	0x60,0x20,0x60,0x00,0x61,0x00,0x61,0x00,0x7F,0x00,0x61,0x00,0x61,0x00,0x60,0x00,
	0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  G  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x40,0x18,0xC0,0x30,0x40,
	0x30,0x40,0x20,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x63,0xF0,0x60,0xC0,
	0x60,0xC0,0x30,0xC0,0x30,0xC0,0x18,0xC0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  H  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x60,0x60,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x7F,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  I  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  J  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x01,0x80,0x01,0x80,
	0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
	0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x61,0x80,0x63,0x00,0x3E,0x00,},
	
	/*--  文字:  K  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0xE0,0x60,0x80,0x61,0x00,
	0x62,0x00,0x62,0x00,0x64,0x00,0x6C,0x00,0x7C,0x00,0x76,0x00,0x67,0x00,0x63,0x00,
	0x63,0x80,0x61,0x80,0x60,0xC0,0x60,0xE0,0xF1,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  L  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0x60,0x00,0x60,0x00,
	0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
	0x60,0x00,0x60,0x20,0x60,0x20,0x60,0x40,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  M  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x70,0xE0,0x70,0xE0,
	0x70,0xE0,0x59,0x60,0x59,0x60,0x59,0x60,0x59,0x60,0x5A,0x60,0x4E,0x60,0x4E,0x60,
	0x4E,0x60,0x4E,0x60,0x44,0x60,0x44,0x60,0xE4,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  N  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x70,0x70,0x20,0x70,0x20,
	0x58,0x20,0x58,0x20,0x4C,0x20,0x46,0x20,0x46,0x20,0x43,0x20,0x43,0x20,0x41,0xA0,
	0x40,0xE0,0x40,0xE0,0x40,0x60,0x40,0x60,0xE0,0x20,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  O  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,
	0x30,0x40,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x30,0x40,0x30,0xC0,0x19,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  P  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x60,0xC0,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0x7F,0x80,0x60,0x00,0x60,0x00,
	0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  Q  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,
	0x30,0x40,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
	0x6E,0x60,0x32,0x40,0x31,0xC0,0x11,0x80,0x0F,0x80,0x01,0xE0,0x00,0xC0,0x00,0x00,},
	
	/*--  文字:  R  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x60,0xC0,0x60,0x60,
	0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0x7F,0x00,0x66,0x00,0x63,0x00,0x63,0x00,
	0x61,0x80,0x61,0x80,0x60,0xC0,0x60,0xC0,0xF0,0x70,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  S  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x30,0xE0,0x60,0x20,
	0x60,0x20,0x60,0x00,0x70,0x00,0x3C,0x00,0x0F,0x00,0x03,0xC0,0x00,0xC0,0x00,0x60,
	0x40,0x60,0x40,0x60,0x60,0x60,0x70,0xC0,0x4F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  T  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,0x46,0x20,0x86,0x10,
	0x86,0x10,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  U  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x70,0x60,0x20,0x60,0x20,
	0x60,0x20,0x60,0x20,0x60,0x20,0x60,0x20,0x60,0x20,0x60,0x20,0x60,0x20,0x60,0x20,
	0x60,0x20,0x60,0x20,0x60,0x20,0x30,0x40,0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  V  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF0,0x70,0x60,0x30,0x40,
	0x30,0x40,0x30,0x40,0x30,0x80,0x18,0x80,0x18,0x80,0x18,0x80,0x18,0x80,0x0D,0x00,
	0x0D,0x00,0x0D,0x00,0x0F,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  W  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF7,0x70,0x66,0x20,0x66,0x20,
	0x66,0x40,0x66,0x40,0x67,0x40,0x37,0x40,0x37,0x40,0x3B,0x80,0x3B,0x80,0x3B,0x80,
	0x3B,0x80,0x39,0x80,0x11,0x00,0x11,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  X  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0xE0,0x30,0xC0,0x18,0x80,
	0x18,0x80,0x19,0x00,0x0D,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,0x07,0x00,0x0B,0x00,
	0x0B,0x80,0x19,0x80,0x11,0x80,0x30,0xC0,0x79,0xE0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  Y  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF0,0x70,0x60,0x30,0x40,
	0x30,0x80,0x18,0x80,0x18,0x80,0x0D,0x00,0x0D,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  Z  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xE0,0x20,0xC0,0x41,0xC0,
	0x01,0x80,0x03,0x80,0x03,0x00,0x03,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,
	0x18,0x00,0x18,0x20,0x38,0x20,0x30,0x40,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  [  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x07,0xE0,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
	0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
	0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x07,0xE0,0x00,0x00,},
	
	/*--  文字:  \  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
	0x08,0x00,0x08,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x02,0x00,0x02,0x00,0x01,0x00,
	0x01,0x00,0x01,0x00,0x00,0x80,0x00,0x80,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x20, },

	/*--  文字:  ]  --*/
    /*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
    {0x00,0x00,0x00,0x00,0x3F,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,
    0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0x00,0x00,0x00,},
	
	/*--  文字:  ^  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x07,0x00,0x0D,0x80,0x10,0x40,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  _  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF0, },
	
	/*--  文字:  '  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x30,0x00,0x38,0x00,0x08,0x00,0x08,0x00,0x10,0x00,0x60,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  a  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x80,0x30,0xC0,0x30,0xC0,0x07,0xC0,0x1C,0xC0,0x30,0xC0,
	0x60,0xC0,0x60,0xC0,0x60,0xC0,0x71,0xD0,0x3E,0xF0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  b  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x70,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x30,0x00,0x33,0x80,0x3C,0xC0,0x38,0x60,0x30,0x60,0x30,0x60,0x30,0x60,
	0x30,0x60,0x30,0x60,0x30,0x40,0x38,0xC0,0x2F,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  c  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x00,0x31,0x80,0x31,0x80,0x61,0x80,0x60,0x00,0x60,0x00,
	0x60,0x00,0x60,0x40,0x30,0x40,0x30,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  d  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x01,0xC0,0x00,0xC0,0x00,0xC0,
	0x00,0xC0,0x00,0xC0,0x1E,0xC0,0x31,0xC0,0x30,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
	0x60,0xC0,0x60,0xC0,0x20,0xC0,0x31,0xE0,0x1E,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  e  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x80,0x18,0xC0,0x10,0x60,0x30,0x60,0x3F,0xE0,0x30,0x00,
	0x30,0x00,0x30,0x00,0x18,0x20,0x1C,0x40,0x07,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  f  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x06,0x60,0x0C,0x60,
	0x0C,0x00,0x0C,0x00,0x7F,0xC0,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,
	0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x3F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  g  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x70,0x19,0x90,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x19,0x80,
	0x1F,0x00,0x30,0x00,0x3E,0x00,0x1F,0xC0,0x60,0x60,0x60,0x60,0x70,0xE0,0x1F,0x80,},
	
	/*--  文字:  h  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x70,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x30,0x00,0x37,0x80,0x38,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,
	0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x79,0xE0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  i  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3E,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  j  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
	0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x33,0x00,0x3E,0x00,  },
	
	/*--  文字:  k  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x70,0x00,0x30,0x00,0x30,0x00,
	0x30,0x00,0x30,0x00,0x33,0xC0,0x31,0x00,0x33,0x00,0x32,0x00,0x36,0x00,0x3E,0x00,
	0x3B,0x00,0x33,0x00,0x31,0x80,0x31,0x80,0x79,0xE0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  l  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x3E,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
	0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  m  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xEE,0xE0,0x77,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,
	0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0xF7,0x70,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  n  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x73,0x80,0x3C,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,
	0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x79,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  o  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,
	0x60,0x60,0x60,0x60,0x30,0xC0,0x30,0xC0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  p  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x77,0x80,0x38,0xC0,0x30,0x60,0x30,0x60,0x30,0x60,0x30,0x60,
	0x30,0x60,0x30,0x60,0x30,0xC0,0x38,0xC0,0x37,0x80,0x30,0x00,0x30,0x00,0x7C,0x00,},
	
	/*--  文字:  q  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1E,0x40,0x31,0xC0,0x30,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
	0x60,0xC0,0x60,0xC0,0x20,0xC0,0x31,0xC0,0x1E,0xC0,0x00,0xC0,0x00,0xC0,0x03,0xE0,},
	
	/*--  文字:  r  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF9,0xE0,0x1A,0x60,0x1C,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
	0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  s  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0xE0,0x18,0x60,0x30,0x20,0x30,0x00,0x1C,0x00,0x0F,0x80,
	0x01,0xC0,0x20,0x60,0x20,0x60,0x30,0xC0,0x3F,0x80,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  t  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x04,0x00,
	0x0C,0x00,0x0C,0x00,0x7F,0x80,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,
	0x0C,0x00,0x0C,0x00,0x0C,0x40,0x0C,0x40,0x07,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  u  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x10,0x40,0x71,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,
	0x30,0xC0,0x30,0xC0,0x30,0xC0,0x39,0xE0,0x1E,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  v  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7C,0xF0,0x38,0x60,0x18,0x40,0x18,0x40,0x0C,0x80,0x0C,0x80,
	0x0C,0x80,0x07,0x00,0x07,0x00,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  w  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF7,0xB0,0x63,0x20,0x63,0x20,0x67,0x20,0x37,0x20,0x35,0xC0,
	0x39,0xC0,0x39,0xC0,0x39,0xC0,0x10,0x80,0x10,0x80,0x00,0x00,0x00,0x00,0x00,0x00,},
	
	/*--  文字:  x  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7D,0xE0,0x18,0x80,0x19,0x80,0x0D,0x00,0x0E,0x00,0x06,0x00,
	0x07,0x00,0x0B,0x00,0x19,0x80,0x11,0xC0,0x7B,0xE0,0x00,0x00,0x00,0x00,0x00,0x00, },
	
	/*--  文字:  y  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7D,0xE0,0x38,0x80,0x18,0x80,0x18,0x80,0x0D,0x00,0x0D,0x00,
	0x0D,0x00,0x06,0x00,0x06,0x00,0x02,0x00,0x04,0x00,0x04,0x00,0x28,0x00,0x38,0x00, },
	
	/*--  文字:  z  --*/
	/*--  宋体18;  此字体下对应的点阵为：宽x高=12x24   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x24  --*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0xC0,0x21,0x80,0x23,0x80,0x03,0x00,0x07,0x00,0x06,0x00,
	0x0E,0x00,0x0C,0x20,0x1C,0x20,0x18,0x60,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,},

} ;
#endif

#endif
