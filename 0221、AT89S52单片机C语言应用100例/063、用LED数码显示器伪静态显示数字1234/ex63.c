//实例63：用LED数码显示器伪静态显示数字1234
#include<reg51.h>           //  包含51单片机寄存器定义的头文件
 void delay(void)        //延时函数，延时约0.6毫秒
{
   unsigned char i;
	for(i=0;i<200;i++)  
           ;
 }

void main(void)
{
   while(1)    //无限循环
   {
     P2=0xfe;   //P2.0引脚输出低电平，DS0点亮
	 P0=0xf9;  //数字1的段码
	 delay();
	 P2=0xfd ;  //P2.1引脚输出低电平，DS1点亮
	 P0=0xa4;  //数字2的段码
	 delay();	 
     P2=0xfb;   //P2.2引脚输出低电平，DS2点亮
	 P0=0xb0;  //数字3的段码
	 delay();
     P2=0xf7;   //P2.3引脚输出低电平，DS3点亮
	 P0=0x99;  //数字4的段码
	 delay();
     P2=0xff;	   
   } 
}


