//实例32：用指针作函数参数控制P0口8位LED流水点亮
#include<reg51.h>
/*************************************************
函数功能：延时约150ms
*************************************************/
void delay(void)
{
  unsigned char m,n;
  for(m=0;m<200;m++)
    for(n=0;n<250;n++)
	        ;
}
/*************************************************
函数功能：流水点亮P0口8位LED
*************************************************/
void led_flow(unsigned char *p) //形参为无符号字符型指针
{
  unsigned char i;
  while(1)
   {
	  i=0;   //将i置为0，指向数组第一个元素
	  while(*(p+i)!='\0') //只要没有指向数组的结束标志
	   {
        P0=*(p+i);// 取的指针所指变量（数组元素）的值，送P0口
	     delay();   //调用延时函数
		  i++;    //指向下一个数组元素
	   }
	}
}


/*************************************************
函数功能：主函数
*************************************************/
void main(void)
{  
  unsigned  char code Tab[ ]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F, 
                              0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE,
										0xFF,0xFE,0xFC,0xFB,0xF0,0xE0,0xC0,0x80,
										0x00,0xE7,0xDB,0xBD,0x7E,0xFF,0xFF,0x3C,
										0x18,0x0,0x81,0xC3,0xE7,0xFF,	0xFF,0x7E};
										//流水灯控制码 
  unsigned char *pointer;
  pointer=Tab;
  led_flow(pointer);
  
}