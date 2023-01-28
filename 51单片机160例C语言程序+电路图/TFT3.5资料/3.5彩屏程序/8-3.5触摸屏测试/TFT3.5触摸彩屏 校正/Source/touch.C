
#include"reg52.h"
#include"tft.h"
#include"touch.h"
#include<stdio.h>

sbit ads7846_CLK = P1^0;
sbit ads7846_CS  = P1^1;
sbit ads7846_DIN = P1^2;
//sbit ads7846_BUSY= P1^3;
sbit ads7846_DOUT = P1^4;
sbit ads7846_IRQ = P1^5;   

#define BColor 0x1f00 //定义背景色
#define FColor 0x001f //定义前景色


/*----------------------------------------------------------------
                       触摸函数
----------------------------------------------------------------*/

void Touch ()
{
    unsigned int row;   
    unsigned int line;
    float  x,y;
    float x1,y1,x2,y2;//temp1,temp2;
              
    ClearScreen(BColor); //清屏
    ADS7846_Initial();  //初始化触摸芯片
  
   Rectangle(1,9,19,11,FColor); //在这个坐标画点，矫正用,十字坐标
   Rectangle(9,1,11,19,FColor);
   while(ads7846_IRQ);       //等待触摸中断信号
    {
	 x1=GUI_TOUCH_X_MeasureX(); //读取该点的触摸值
	 y1=GUI_TOUCH_X_MeasureY(); 
	}
  Rectangle(1,9,19,11,BColor); 
  Rectangle(9,1,11,19,BColor); //清除这个点，并准备画下一个点
  while(!ads7846_IRQ);       //等待离开触摸屏，防止干扰下一个点
  Rectangle(X_MAX-19,Y_MAX-11,X_MAX-1,Y_MAX-9,FColor);
  Rectangle(X_MAX-11,Y_MAX-19,X_MAX-9,Y_MAX-1,FColor);//画第二个点
  while(ads7846_IRQ);        //等待触摸中断信号
    {
	 x2=GUI_TOUCH_X_MeasureX(); //读取该点触摸值
	 y2=GUI_TOUCH_X_MeasureY(); 
	}
  Rectangle(X_MAX-19,Y_MAX-11,X_MAX-1,Y_MAX-9,BColor);
  Rectangle(X_MAX-11,Y_MAX-19,X_MAX-9,Y_MAX-1,BColor);//清除第二个点
  delayms(200);               //延时
  TI=1;
  printf("%d %d %d %d\n",(int)x1,(int)x2,(int)y1,(int)y2);//通过串口打印结果，用于内部测试。
	while(1)
	{

	  while(!ads7846_IRQ)
	  {
		   do{
				x=GUI_TOUCH_X_MeasureX(); row=(X_MAX-20)*(x-x1)/(x2-x1)+10; //读取当前触摸值。并根据矫正点的值算出实际的显示坐标
				y=GUI_TOUCH_X_MeasureY(); line=(Y_MAX-20)*(y-y1)/(y2-y1)+10;//采样更多点，加强算法可以得到精确值，这里用简单
                                                                     //算法，不能做到绝对精确。长期使用触屏有失效的可能性
		
		     }

		   while((x>4095)||(x<0)||(y>4095)||(y<0));               

			//if((row>20)&&(row<=50)&&(line>330)&&(line<340))    CLR_Screen(0xf800); 
			//if((row>70)&&(row<=110)&&(line>330)&&(line<340))   CLR_Screen(0x07e0); 	
			//if((row>130)&&(row<=170)&&(line>330)&&(line<340))  CLR_Screen(0x0000);
        	//if((row>190)&&(row<=220)&&(line>330)&&(line<340))  CLR_Screen(0xffff);
           
            if((line>=0)&&(line<=Y_MAX))  Show_Touch(row,line,0x001f); //320x480坐标内直接画点
          
		 }
	 }    


}
/*----------------------------------------------------------------
                          触摸显示点函数
----------------------------------------------------------------*/
void Show_Touch(unsigned int x,unsigned int y,unsigned int Color)
{
 Put_pixel(x-1,y,Color);  	
 Put_pixel(x,y-1,Color); 
 Put_pixel(x-1,y-1,Color); 
 Put_pixel(x,y,Color);
 Put_pixel(x+1,y,Color);
 Put_pixel(x,y+1,Color);
 Put_pixel(x+1,y+1,Color);
 Put_pixel(x-1,y+1,Color);
 Put_pixel(x+1,y-1,Color);

}    

/*----------------------------------------------------------------
                        触摸屏初始化函数
----------------------------------------------------------------*/
void   ADS7846_Initial(void)
{
	ads7846_CS=0;
	ads7846_DIN=0;
	ads7846_CLK=0;
	Write_ADS7846(0xa0);//#10100000B
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	//while(ads7846_BUSY);
	Write_ADS7846(0xe0);//#11100000B
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	//while(ads7846_BUSY);
	Write_ADS7846(0xf0);//#11110000B
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	//while(ads7846_BUSY);
	Write_ADS7846(0x80);//#10000000B
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	//while(ads7846_BUSY);
	ads7846_CS=1;
}
/*----------------------------------------------------------------
                         读取触摸参数x值
----------------------------------------------------------------*/
unsigned int Read_ADS7846_x(void)
{
    unsigned int x;
	unsigned char i;
	bit dat;

	x=0;
	ads7846_CS=0;
	Write_ADS7846(0xd0);//#0x90
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	dat=ads7846_DOUT;
	x=x|dat;
	//while(ads7846_BUSY);
	for(i=0;i<11;i++)
	{
		x=x<<1;
		ads7846_CLK=1;
		ads7846_CLK=0;
		dat=ads7846_DOUT;
		x=x|dat;
	}
	ads7846_CS=1;
    return x;
}
/*----------------------------------------------------------------
                         读取触摸参数y值
----------------------------------------------------------------*/
unsigned int Read_ADS7846_y(void)
{
	unsigned int y;
	unsigned char i;
	bit dat;

	y=0;
	ads7846_CS=0;
	Write_ADS7846(0x90);//#0xd0
	ads7846_DIN=0;
	ads7846_CLK=1;
	ads7846_CLK=0;
	dat=ads7846_DOUT;
	y=y|dat;
	//while(ads7846_BUSY);
	for(i=0;i<11;i++)
	{
		y=y<<1;
		ads7846_CLK=1;
		ads7846_CLK=0;
		dat=ads7846_DOUT;
		y=y|dat;
	}
	ads7846_CS=1;
    return y;
}
/*----------------------------------------------------------------
                       写入触摸芯片指令
----------------------------------------------------------------*/
void   Write_ADS7846(unsigned char x)
{
    unsigned char i;
	bit dat;
	ads7846_CS=0;
	ads7846_DIN=0;
	ads7846_CLK=0;
	for(i=0;i<8;i++)
	{
		dat=x&0x80;
		ads7846_DIN=dat;
		ads7846_CLK=1;
		ads7846_CLK=0;
		x=x<<1;
	}
}

int  GUI_TOUCH_X_MeasureX(void) 
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,X=0;	
 	
	while(count<10)//循环读数10次
	{	   	  
		databuffer[count]= Read_ADS7846_x();
		count++; 
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		X=(databuffer[3]+databuffer[4]+databuffer[5])/3;	  

	}
	return(X);  
}

int  GUI_TOUCH_X_MeasureY(void) {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,Y=0;	
 
    while(count<10)	//循环读数10次
	{	   	  
		databuffer[count]= Read_ADS7846_y();
		count++;  
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		Y=(databuffer[3]+databuffer[4]+databuffer[5])/3;	  
   
	}
	return(Y); 
}

