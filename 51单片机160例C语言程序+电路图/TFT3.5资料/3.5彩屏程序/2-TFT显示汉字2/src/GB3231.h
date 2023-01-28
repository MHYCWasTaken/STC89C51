// ------------------  汉字字模的数据结构定义 ------------------------ //
struct  typFNT_GB16                 // 汉字字模数据结构 
{
       unsigned char  Index[3];               // 汉字内码索引	
       unsigned char   Msk[124];                        // 点阵码数据 
};

/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体16.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
 code struct  typFNT_GB16 codeGB_16[] =          // 数据表 
{

/*--  文字:  朗  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"朗",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x00,0x00,0xE0,0x60,0x30,
0x00,0xF0,0x70,0x78,0x18,0xE6,0x7F,0xF8,0x1F,0xFF,0x78,0x70,0x1E,0x0E,0x78,0x70,
0x1E,0x0E,0x78,0x70,0x1E,0x0E,0x78,0x70,0x1E,0x0E,0x78,0x70,0x1F,0xFE,0x7F,0xF0,
0x1E,0x0E,0x78,0x70,0x1E,0x0E,0x78,0x70,0x1E,0x0E,0x78,0x70,0x1E,0x0E,0x70,0x70,
0x1F,0xFE,0x70,0x70,0x1E,0x0E,0x70,0x70,0x1E,0x00,0x7F,0xF0,0x1E,0xC0,0x70,0x70,
0x1E,0x70,0x70,0x70,0x1E,0x3C,0x70,0x70,0x1E,0x3E,0xF0,0x70,0x1E,0xFE,0xE0,0x70,
0x1F,0xCF,0xE0,0x70,0x1F,0x01,0xC0,0x70,0x1E,0x03,0x80,0x70,0x0C,0x07,0x0F,0xF0,
0x00,0x1C,0x01,0xF0,0x00,0x38,0x00,0xC0,0x00,0x00,0x00,0x00,

/*--  文字:  译  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"译",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x03,0xC0,
0x07,0x3F,0xFF,0xE0,0x07,0x87,0x07,0x80,0x07,0x83,0x07,0x00,0x03,0x83,0x8F,0x00,
0x00,0x01,0xDE,0x00,0x00,0x00,0xFC,0x00,0x03,0x00,0x78,0x00,0x7F,0x80,0xFE,0x00,
0x37,0x83,0xCF,0xC0,0x07,0x07,0xF3,0xFC,0x07,0x1C,0x3C,0xF0,0x07,0xF0,0x38,0x00,
0x07,0xC0,0x39,0xE0,0x07,0x0F,0xFF,0xF0,0x07,0x00,0x38,0x00,0x07,0x00,0x38,0x00,
0x07,0x30,0x38,0x30,0x07,0x60,0x38,0x78,0x07,0xFF,0xFF,0xFC,0x07,0xC0,0x38,0x00,
0x07,0x80,0x38,0x00,0x07,0x80,0x38,0x00,0x03,0x00,0x38,0x00,0x00,0x00,0x78,0x00,
0x00,0x00,0x78,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  电  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"电",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x80,0x00,
0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x0C,0x07,0x01,0xC0,
0x0F,0xFF,0xFF,0xE0,0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,
0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,0x0F,0xFF,0xFF,0xC0,0x0E,0x07,0x03,0xC0,
0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,0x0E,0x07,0x03,0xC0,
0x0F,0xFF,0xFF,0xC0,0x0E,0x07,0x03,0xC0,0x0E,0x07,0x00,0x30,0x00,0x07,0x00,0x30,
0x00,0x07,0x00,0x30,0x00,0x07,0x00,0x38,0x00,0x07,0x00,0x38,0x00,0x07,0x80,0x7C,
0x00,0x03,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  子  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"子",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x01,0xC0,
0x07,0xFF,0xFF,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0x00,0x00,0x00,0x1C,0x00,
0x00,0x00,0x38,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xC0,0x00,0x00,0x03,0xE0,0x00,
0x00,0x03,0x80,0x30,0x00,0x03,0x80,0x78,0x7F,0xFF,0xFF,0xFC,0x00,0x03,0x80,0x0C,
0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,
0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,
0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x7F,0x80,0x00,
0x00,0x0F,0x80,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  科  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"科",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x03,0xC0,0x00,0x3C,0x03,0xE0,
0x03,0xFE,0x03,0xC0,0x3F,0xC0,0x03,0xC0,0x01,0xC0,0xE3,0xC0,0x01,0xC0,0x73,0xC0,
0x01,0xC0,0x7B,0xC0,0x01,0xC0,0x3B,0xC0,0x01,0xCE,0x33,0xC0,0x7F,0xFF,0x03,0xC0,
0x01,0xC3,0x03,0xC0,0x01,0xC1,0xC3,0xC0,0x03,0xC0,0xE3,0xC0,0x03,0xF0,0x73,0xC0,
0x07,0xFC,0x7B,0xC0,0x07,0xDE,0x33,0xF8,0x0F,0xCE,0x03,0xFC,0x0F,0xCC,0x07,0xE0,
0x1D,0xC0,0xFF,0xC0,0x39,0xCF,0x83,0xC0,0x31,0xC0,0x03,0xC0,0x61,0xC0,0x03,0xC0,
0x41,0xC0,0x03,0xC0,0x01,0xC0,0x03,0xC0,0x01,0xC0,0x03,0xC0,0x01,0xE0,0x03,0xC0,
0x01,0xE0,0x03,0xC0,0x01,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,

/*--  文字:  技  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"技",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x1C,0x00,0x03,0xC0,0x1E,0x00,
0x03,0x80,0x1C,0x00,0x03,0x80,0x1C,0x00,0x03,0x80,0x1C,0x00,0x03,0x98,0x1C,0x30,
0x03,0xBC,0x1C,0x78,0x7F,0xEF,0xFF,0xCC,0x03,0x80,0x1C,0x00,0x03,0x80,0x1C,0x00,
0x03,0x80,0x1C,0x00,0x03,0x8C,0x1C,0xC0,0x03,0xBF,0xFF,0xE0,0x03,0xE1,0x80,0xF0,
0x0F,0x81,0x81,0xE0,0x7F,0x81,0xC1,0xC0,0x7B,0x80,0xC3,0xC0,0x33,0x80,0xE3,0x80,
0x03,0x80,0x67,0x80,0x03,0x80,0x77,0x00,0x03,0x80,0x3E,0x00,0x03,0x80,0x3E,0x00,
0x03,0x80,0x3E,0x00,0x03,0x80,0xFF,0x00,0x03,0x81,0xE7,0xC0,0x3F,0x87,0x83,0xF8,
0x0F,0x9E,0x00,0xFC,0x07,0x78,0x00,0x30,0x00,0x00,0x00,0x00,

/*--  文字:  有  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"有",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0xC0,0x00,
0x00,0x0F,0x00,0x00,0x00,0x0F,0x00,0x70,0x00,0x1E,0x00,0xF8,0x7F,0xFF,0xFF,0x8C,
0x00,0x1C,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x78,0x03,0x00,0x00,0x70,0x07,0x80,
0x00,0xFF,0xFF,0x80,0x01,0xF0,0x07,0x00,0x03,0xF0,0x07,0x00,0x03,0xF0,0x07,0x00,
0x07,0xF0,0x07,0x00,0x1C,0xFF,0xFF,0x00,0x38,0xF0,0x07,0x00,0x70,0xF0,0x07,0x00,
0x60,0xF0,0x07,0x00,0x00,0xFF,0xFF,0x00,0x00,0xF0,0x07,0x00,0x00,0xF0,0x07,0x00,
0x00,0xF0,0x07,0x00,0x00,0xF0,0x07,0x00,0x00,0xF0,0x07,0x00,0x00,0xF0,0x7F,0x00,
0x00,0xF0,0x1F,0x00,0x00,0xE0,0x0E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  限  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"限",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x76,0x01,0xC0,
0x1F,0xFF,0xFF,0xE0,0x1C,0xF7,0x01,0xC0,0x1C,0xE7,0x01,0xC0,0x1C,0xE7,0x01,0xC0,
0x1C,0xC7,0x01,0xC0,0x1D,0xC7,0xFF,0xC0,0x1D,0x87,0x01,0xC0,0x1D,0x87,0x01,0xC0,
0x1D,0x87,0x01,0xC0,0x1C,0xC7,0x01,0xC0,0x1C,0xE7,0x01,0xC0,0x1C,0x67,0xFF,0xC0,
0x1C,0x77,0x70,0x60,0x1C,0x77,0x70,0xF0,0x1C,0x77,0x31,0xF8,0x1C,0x77,0x3B,0xC0,
0x1C,0x77,0x3F,0x00,0x1F,0xF7,0x1C,0x00,0x1D,0xE7,0x1E,0x00,0x1D,0xC7,0x0E,0x00,
0x1C,0x07,0x1F,0x80,0x1C,0x07,0x73,0xC0,0x1C,0x07,0xC1,0xF8,0x1C,0x07,0x80,0xFC,
0x1C,0x07,0x00,0x70,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  公  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"公",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x18,0x70,0x00,
0x00,0x3E,0x70,0x00,0x00,0x3C,0x70,0x00,0x00,0x38,0x30,0x00,0x00,0x78,0x38,0x00,
0x00,0x70,0x38,0x00,0x00,0xE0,0x1C,0x00,0x01,0xE0,0x1E,0x00,0x01,0xC0,0x0F,0x00,
0x03,0x80,0x07,0x80,0x07,0x03,0x83,0xC0,0x0F,0x07,0xC3,0xE0,0x0E,0x07,0x81,0xF8,
0x18,0x0F,0x00,0xFC,0x30,0x0E,0x00,0x20,0x00,0x1E,0x00,0x00,0x00,0x1C,0x00,0x00,
0x00,0x38,0x70,0x00,0x00,0x70,0x38,0x00,0x00,0x70,0x1C,0x00,0x00,0xE0,0x0E,0x00,
0x01,0xC0,0x0F,0x00,0x03,0x80,0x07,0x80,0x07,0xFF,0xFF,0x80,0x07,0xE0,0x03,0xC0,
0x03,0x00,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  司  --*/
/*--  宋体23;  此字体下对应的点阵为：宽x高=31x31   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=32x31  --*/
"司",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xC0,
0x0F,0xFF,0xFF,0xE0,0x00,0x00,0x01,0xE0,0x00,0x00,0x01,0xE0,0x00,0x00,0x01,0xE0,
0x00,0x00,0x19,0xE0,0x00,0x00,0x3D,0xE0,0x3F,0xFF,0xFF,0xE0,0x00,0x00,0x01,0xE0,
0x00,0x00,0x01,0xE0,0x07,0x00,0xC1,0xE0,0x07,0xFF,0xE1,0xE0,0x03,0x81,0xE1,0xE0,
0x03,0x81,0xE1,0xE0,0x03,0x81,0xE1,0xE0,0x03,0x81,0xE1,0xE0,0x03,0x81,0xE1,0xE0,
0x03,0x81,0xE1,0xE0,0x03,0xFF,0xE1,0xE0,0x03,0x81,0xE1,0xE0,0x07,0x81,0xC1,0xE0,
0x06,0x00,0x01,0xE0,0x00,0x00,0x01,0xE0,0x00,0x00,0x01,0xE0,0x00,0x00,0x3F,0xC0,
0x00,0x00,0x07,0xC0,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,



};
