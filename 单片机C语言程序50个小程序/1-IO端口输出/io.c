/*-----------------------------------------------
  名称：IO口高低电平控制
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：点亮P1口的一个LED灯
        该程序是单片机学习中最简单最基础的，
		通过程序了解如何控制端口的高低电平
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{

P1 = 0xFF;        //P1口全部为高电平，对应的LED灯全灭掉，ff换算成二进制是 1111 1111

P1 = 0xfe;        //P1口的最低位点亮，可以更改数值是其他的灯点亮
                  //0xfe是16进制，0x开头表示16进制数，fe换算成二进制是 1111 1110 
while (1)         //主循环
  {
                  //主循环中添加其他需要一直工作的程序
  }
}