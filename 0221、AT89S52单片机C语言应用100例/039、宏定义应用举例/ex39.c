//实例39：宏定义应用举例
#include<reg51.h>   //包含51单片机寄存器定义的头文件
# define F(a,b) (a)+(a)*(b)/256+(b)     //带参数的宏定义，a和b为形参
void main(void)
 {
	  unsigned char i,j,k;
	  i=40;
	  j=30;
	  k=20;
	  P3=F(i,j+k);  //i和j+k分别为实参，宏展开时，实参将替代宏定义中的形参
	  while(1)
	    ;
 }

