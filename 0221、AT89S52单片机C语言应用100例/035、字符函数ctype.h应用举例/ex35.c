//实例35：字符函数ctype.h应用举例
#include<reg51.h>   //包含51单片机寄存器定义的头文件
#include<ctype.h>
void main(void)
 {
   while(1)
   {		
	  P3=isalpha('_')?0xf0:0x0f;//条件运算，若'_'是英文字母，P3=0xf0	  
	}
}
