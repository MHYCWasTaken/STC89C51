/*************************************************************************************
*  标题:  步进电机试验四(采用2细分)											 *
* 																					 *
*    通过本例程了解步进马达使用及驱动程序编写										 *
*;   单相四拍通电驱动时序：                                                          *
*;   正转： A/  B  A  B/															 *
*    反转： B/  A  B  A/ 								                             *
*    																				 *
*    UDN2916电流控制真值表：														 *
*	  I0    I1        输出电流														 *
*	  L	    L		  最大															 *
*	  H		L		  最大*2/3														 *
*	  L		H		  最大*1/3														 *
*     H     H         0	                                                             *
*   请学员一定要消化掉本例程                       	                            	 *
*   注意：J14短路冒需断开                 	                          				 *
**************************************************************************************/  
#include "reg52.h"

#define speed 38
 //Motor
sbit PH1 = P1^0;	  //定义管脚
sbit PH2 = P1^1;
sbit I01 = P1^2;
sbit I11 = P1^3;
sbit I02 = P1^4;
sbit I12 = P1^5;
				     
unsigned char TableA[] = { 0XF7,0XFB,0XF3};	 //A线圈细分表
unsigned char TableB[] = { 0XeF,0XdF,0XcF};	 //B线圈细分表

void delay(int time);

/***************************************
函数功能：产生单相四拍脉冲控制步进机 2细分
**************************************/
void Go()
{	char i ,temp;
     //A
    PH1 = 0;  //PH1为0 则A线圈为反向电流
	for(i = 0; i<3; i++)
	{  temp = P1;
	   P1 = TableA[i]; 
	   P1 = P1&temp; 
	   delay(1);
	 }
	

	PH2 = 0;  //PH2为0 则B线圈为反向电流
	I02 = 1;
	I12 = 1;   //输出0

	delay(speed);
	//0
	PH1 = 0;  //PH1为0 则A线圈为反向电流
    I01 = 1;  //输出0
	I11 = 1;
	

	PH2 = 1;  //PH2为1 则B线圈为正电流
	for(i = 0; i<3; i++)
	{  temp = P1;
	   P1 = TableB[i]; 
	   P1 = P1&temp; 
	   delay(1);
	 }

	delay(speed);
	//B
	PH1 = 1;   //PH1为1 则A线圈为正向电流
	for(i = 0; i<3; i++)
	{  temp = P1;
	   P1 = TableA[i]; 
	   P1 = P1&temp; 
	   delay(1);
	 }

	PH2 = 1;  //PH2为1 则B线圈为正向电流
	I02 = 1;  //输出0
	I12 = 1;
	           ////
	delay(speed);
	//0
	PH1 = 1;   //PH1为1 则A线圈为正向电流
	I01 = 1;
	I11 = 1;
	

	PH2 = 0;   //PH2为0 则B线圈为反向电流
	for(i = 0; i<3; i++)
	{  temp = P1;
	   P1 = TableB[i]; 
	   P1 = P1&temp; 
	   delay(1);
	 }
	delay(speed);
               

	
}
/*******************延时函数****************************/
void delay(int time)
{ 
  int i,j;
  
  for(j=0; j <= time; j++)
   for(i =0 ; i <= 120; i++);
}


void main()
{

  while(1)
  {
    
    Go();  //步进电机运行
	
  }

}
