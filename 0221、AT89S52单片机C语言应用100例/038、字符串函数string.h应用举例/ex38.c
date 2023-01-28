//实例38：字符串函数string.h应用举例
#include<reg51.h>   //包含51单片机寄存器定义的头文件
#include<string.h>   //包含函数isalpha（）声明的头文件
void main(void)
 {
  unsigned char str1[ ]="Now, The temperature is :";
  unsigned char str2[ ]="Now, The temperature is 36 Centgrade:";
  unsigned char i;
  i=strcmp(str1,str2); //比较两个字符串，并将结果存入i
  if(i==0) //str1=str2
    P3=0x00;
  else
    if(i<0)   //str1<str2
       P3=0xf0;
	 else    //str1>str2
	    P3=0x0f;
  while(1)
    ;   //防止程序“跑飞”
 }
