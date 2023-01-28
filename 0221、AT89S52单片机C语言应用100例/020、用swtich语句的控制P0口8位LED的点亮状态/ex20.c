//实例20：用swtich语句的控制P0口8位LED的点亮状态
#include<reg51.h>  //包含单片机寄存器的头文件
sbit S1=P1^4;     //将S1位定义为P1.4
/*****************************
函数功能：延时一段时间
*****************************/
void delay(void)
{
 unsigned int n;
 for(n=0;n<10000;n++)
        ;
}
/*****************************
函数功能：主函数
*****************************/
void main(void)
{  
   unsigned char i;
	i=0;    //将i初始化为0
	while(1)
	 {
		  if(S1==0)     //如果S1键按下
		   {
			  delay();  //延时一段时间
			  if(S1==0) //如果再次检测到S1键按下
			    i++;    //i自增1
			  if(i==9)  //如果i=9，重新将其置为1
			    i=1;
				
		   }
		 switch(i)   //使用多分支选择语句
				 {
				   case 1: P0=0xfe;  //第一个LED亮
							break;
					case 2: P0=0xfd;  //第二个LED亮
					      break;
					case 3:P0=0xfb;   //第三个LED亮
					      break;
					case 4:P0=0xf7;   //第四个LED亮
					      break;
					case 5:P0=0xef;   //第五个LED亮
					      break;
					case 6:P0=0xdf;   //第六个LED亮
					      break;
					case 7:P0=0xbf;   //第七个LED亮
				         break;
					case 8:P0=0x7f;   //第八个LED亮
					      break;
					default:   //缺省值，关闭所有LED
					      P0=0xff;
				 }
		 }	  
}