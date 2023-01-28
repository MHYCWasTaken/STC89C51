#include<reg51.h>  //包含51单片机寄存器定义的头文件
#define MAX 100   //将MAX宏定义为字符串100
void main(void) 
{
	#if MAX>80   //如果字符串100大于80
	  P3=0xf0;   //P3口低四位LED点亮
	#else
	  P3=0x0f;   //否则，P3口高四位LED点亮
	#endif		 //结束本次编译
		
}



