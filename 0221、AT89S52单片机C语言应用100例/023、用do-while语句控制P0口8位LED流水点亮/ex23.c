//实例23：用do-while语句控制P0口8位LED流水点亮
#include<reg51.h>  //包含单片机寄存器的头文件
/****************************************
函数功能：延时约60ms (3*100*200=60000μs)
****************************************/
void delay60ms(void)
{
 unsigned char m,n;
 for(m=0;m<100;m++)
   for(n=0;n<200;n++)
        ;
}
/****************************************
函数功能：主函数
****************************************/
void main(void)
{  
  do
    { 
	   P0=0xfe;     //第一个LED亮
		delay60ms();
	   P0=0xfd;     //第二个LED亮
		delay60ms();
		P0=0xfb;     //第三个LED亮
		delay60ms();
		P0=0xf7;     //第四个LED亮
		delay60ms();
		P0=0xef;     //第五个LED亮
		delay60ms();
		P0=0xdf;     //第六个LED亮
		delay60ms();
		delay60ms();
		P0=0xbf;     //第七个LED亮
		delay60ms();
		P0=0x7f;     //第八个LED亮
	   delay60ms();
	 }while(1);     //无限循环，使8位LED循环流水点亮
}
