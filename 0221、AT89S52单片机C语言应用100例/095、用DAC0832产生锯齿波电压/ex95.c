//实例95：用DAC0832产生锯齿波电压
#include<reg51.h>       //包含单片机寄存器的头文件
#include<absacc.h>     //包含对片外存储器地址进行操作的头文件
sbit CS=P2^7;          //将CS位定义为P2.7引脚
sbit WR12=P3^6;        //将WR12位定义为P3.6引脚
void main(void)
{
	unsigned char i;
   CS=0;    //输出低电平以选中DAC0832
   WR12=0;  //输出低电平以选中DAC0832
   while(1)
     {
       for(i=0;i<255;i++)
        XBYTE[0x7fff]=i; 	 //将数据i送入片外地址07FFFH ，实际上就是通过P0口将数据送入DAC0832 
	  }
}
