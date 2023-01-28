//实例34：用指针数组作为函数的参数显示多个字符串
#include<reg51.h>   //包含51单片机寄存器定义的头文件
unsigned char code str1[ ]="Temperature is tested by DS18B20";//C语言中，字符串是作为字符数组来处理的
unsigned char code str2[ ]="Now temperature is:";    //所以，字符串的名字就是字符串的首地址        
unsigned char code str3[ ]="The Systerm is designed by Zhang San";
unsigned char code str4[ ]="The date is 2008-9-30";
unsigned char *p[ ]={str1,str2,str3,str4}; //定义p[4]为指向4个字符串的字符型指针数组                                        
/**************************************************************
函数功能：延时约150ms
**************************************************************/
 void delay(void)   
{
   unsigned char m,n;
	 for(m=0;m<200;m++)
	  for(n=0;n<250;n++)	   
           ;
 }
/**************************************************************
函数功能：流水点亮P0口8位LED
**************************************************************/
void led_display(unsigned char *x[ ])  //形参必须为指针数组
{
	unsigned char i,j;		
	  for(i=0;i<4;i++) //有4个字符串要显示
	   {
		  j=0;  //指向待显字符串的第0号元素
	     while(*(x[i]+j)!='\0') //只要第i个字符串的第j号元素不是结束标志
		   {
		      P0=*(x[i]+j); //取得该元素值送到P0口显示
				delay();  //调用延时函数
		  	   j++;   //指向下一个元素
		   }
		}			
}
/**************************************************************
函数功能：主函数
**************************************************************/ 
 void main(void)
 {
   unsigned char i;
   while(1)
	 {
	  for(i=0;i<4;i++)
	 	led_display(p); //将指针数组名作实际参数传递
	 }
}