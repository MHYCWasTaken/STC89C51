//实例27：用指针数组控制P0口8位LED流水点亮
#include<reg51.h>
/*************************************************
函数功能：延时约150ms (3*200*250=150 000μs=150ms
*************************************************/
void delay150ms(void)
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
  unsigned char code Tab[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
  unsigned char *p[ ]={&Tab[0],&Tab[1],&Tab[2],&Tab[3],&Tab[4],&Tab[5],
                 &Tab[6],&Tab[7]};

  unsigned char i;     //定义无符号字符型数据
  while(1)
    {
	   for(i=0;i<8;i++)
		 {
		   P0=*p[i];
			delay150ms();
		 }
	 }
}