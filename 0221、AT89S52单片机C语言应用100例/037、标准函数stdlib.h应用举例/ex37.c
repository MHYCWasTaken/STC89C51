//实例37：标准函数stdlib.h应用举例
#include<reg51.h>   //包含51单片机寄存器定义的头文件
#include<stdlib.h>   //包含函数isalpha（）声明的头文件
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
函数功能：主函数
*************************************************/
void main(void)
 {
  unsigned char i;
  while(1)
   {
	  for(i=0;i<10;i++) //产生10个随机数
	    {
		   P3=rand()/160; //将产生的随机数缩小160倍后送P3显示
			delay();
		  }
	 }
 }
