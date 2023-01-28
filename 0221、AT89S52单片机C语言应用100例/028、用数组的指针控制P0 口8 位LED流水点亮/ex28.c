//实例28：用数组的指针控制P0 口8 位LED流水点亮
#include<reg51.h>
/*************************************************
函数功能：延时约150ms (3*200*250=150 000μs=150ms
*************************************************/
void delay150ms(void)
{
  unsigned char m,n;
  for(m=0;m<200;m++)
    for(n=0;n<250;n++)
	   ;
}
/*************************************************
函数功能：主函数
*************************************************/
void main(void)
{  
  unsigned char i;     
  unsigned char Tab[ ]={0xFF,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,
                        0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE,
						 	   0xFE,0xFC,0xFB,0xF0,0xE0,0xC0,0x80,0x00,
						      0xE7,0xDB,0xBD,0x7E,0x3C,0x18,0x00,0x81,
								0xC3,0xE7,0x7E,0xBD,0xDB,0xE7,0xBD,0xDB};
								//流水灯控制码
  unsigned char *p;   //定义无符号字符型指针
  p=Tab;              //将数组首地址存入指针p
  while(1)
    {
	   for(i=0;i<32;i++) //共32个流水灯控制码
		 {
		   P0=*(p+i);     //*（p+i)的值等于a[i]
			delay150ms();  //调用150ms延时函数
		 }
	 }
}