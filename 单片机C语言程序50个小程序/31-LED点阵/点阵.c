//流水灯的方式测试点阵
#include <reg52.h>
////////////////////////////////////////////////////////
void delay(unsigned int cnt)
{
 while(--cnt);
}
///////////////////////////////////////////////////////
main()
{
 P2=0x00;
 P0=0x55;
 P2=0xfe;//给初始化值
 while(1)
      {
	   delay(30000);//delay at crystal frequency in 12MHz
	   P2<<=1;//左移一位
	   P2|=0x01;//最后一位补1
	   
	   if(P2==0x7f)//检测是否移到最左端？
          { 
		     delay(30000);//delay
	         P2=0xfe;
			 P0=~P0;
          }
      }
}
