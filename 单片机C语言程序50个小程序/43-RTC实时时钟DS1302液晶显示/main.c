/*-----------------------------------------------
  名称：LCD1602显示实时时钟
  公司：上海浩豚电子科技有限公司
  网站：www.doflye.cn
  编写：师访
  日期：2009.5
  修改：无
  内容：通过读取ds1302信息直接显示到液晶屏上。有备用电池，断电后时间仍可以正常运行。通过串口调试软件更新时间。更新方式如下
        在发送栏填写时间信息，格式如下 2009071210102005 表示2009年7月12日 10点10分20秒 星期五，然后通过串口发送，硬件连接方式和
		串口通信相同。注意中间不能有空格
  -----------------------------------------------*/
#include<reg52.h>
#include"ds1302.h"
#include"LCD1602.h"

sbit LACTCH1=P2^0;//锁存端口定义
sbit LACTCH2=P2^3;


bit SetFlag;      //更新时间标志位
unsigned char time_buf2[16];
void UART_Init(void)
{
    SCON  = 0x50;		        /* SCON: 模式 1, 8-bit UART, 使能接收         */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 0xFD;               /* TH1:  reload value for 9600 baud @ 11.0592MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    EA    = 1;                  /*打开总中断*/
    ES    = 1;                  /*打开串口中断*/

}
main()
{
  unsigned char i;

       UART_Init();
       P0=0;       //取显示数据,屏蔽数码管显示
       LACTCH1=1;  //锁存
       LACTCH1=0;
       
	   P0=0;       //取段码
       LACTCH2=1;  // 锁存
       LACTCH2=0;
	  
       ds1302_init(); /*DS302初始化函数*/
       //ds1302_write_time();
       InitLcd();     //初始化LCD
       DelayMs(15);   //延时用于稳定功能

  while(1)
       {
	    ds1302_read_time();//读取时钟信息
        for(i=1;i<8;i++)
		   TempData[i]=time_buf1[i];
           disp();         //液晶显示时间信息
           //KeyPro();
		   if(SetFlag)     //如果接收到串口信息则更新时钟
		     {
			 for(i=0;i<8;i++)
			 {
			 time_buf1[i]=time_buf2[2*i]*10+time_buf2[2*i+1];//数据整合，如2个数 1和5整合成15
			 }
			 ds1302_write_time();
			 SetFlag=0;     //时钟信息更新后标志位清零
			 }
	     }
 
  }

/******************************************************************/
/*                  串口中断程序                                  */
/******************************************************************/
void UART_SER (void) interrupt 4 //串行中断服务程序
{
    unsigned char Temp;          //定义临时变量 
    
    unsigned char i;
   if(RI)                        //判断是接收中断产生
     {
	  RI=0;                      //标志位清零
	  Temp=SBUF;                 //读入缓冲区的值
	  time_buf2[i]=Temp&0x0F;
	  i++;
	  if(i==16)                  //连续接收16个字符信息
	   {
	    i=0;
		SetFlag=1;               //接收完成标志位置1
	   }
      SBUF=Temp; //把接收到的值再发回电脑端
	  //P1=Temp&0x0F; //调试时候显示
	 }
   if(TI)  //如果是发送标志位，清零
     TI=0;
} 
