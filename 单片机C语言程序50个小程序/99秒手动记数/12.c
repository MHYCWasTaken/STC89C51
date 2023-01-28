#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义


unsigned char const dofly[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};// 显示段码值01234567
unsigned char code  seg[]={0,1};//分别对应相应的数码管点亮,即位码

unsigned char datal[2];
unsigned char key,del;
void delay(unsigned int i)
{while(i--);
}
/*------------------------------------------------延时子程序------------------------------------------------*/

/*------------------------------------------------
              键盘扫描程序
------------------------------------------------*/
unsigned char keyscan(void)  //键盘扫描函数，使用行列反转扫描法
{
 unsigned char cord_h,cord_l;//行列值中间变量
 P3=0x0f;            //行线输出全为0
 cord_h=P3&0x0f;     //读入列线值
 if(cord_h!=0x0f)    //先检测有无按键按下
 {
  delay(100);        //去抖
  cord_h=P3&0x0f;     //读入列线值
  if(cord_h!=0x0f)
  {
    cord_h=P3&0x0f;  //读入列线值00001101
    P3=0xf0;  //输出当前列线值
    cord_l=P3&0xf0;  //读入行线值11010000
    while(P3!=0xf0);
    return(cord_h+cord_l);//键盘最后组合码值
   }
  }return(0xff);     //返回该值
}


/*------------------------------------------------ 主函数------------------------------------------------*/


/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer1_isr(void) interrupt 3
{int i;
 TH1=0xf8;//TH1=0x00;		 //重新赋值
 TL1=0x00;//TL1=0x00;
 P0=dofly[datal[i]];//取显示数据，段码
 P2=seg[i];  //取位码
 i++;
 if(2==i)    //检测8位扫描完全？
  i=0;
}

void Init_Timer1(void)
{
 TMOD|=0x10;
 TH1=0x00;
 TL1=0x00;
 EA=1;
 ET1=1;
 TR1=1;
 }


main()
{
Init_Timer1();

 while(1)
      {

key=keyscan();
if(key==0x7e)
{
del++;
if(del==100)
del=0;
}
if(key==0x7d)
{
if(del==0)
del=100;
del--;
}

datal[1]=del%10;
datal[0]=del/10;
      }
}