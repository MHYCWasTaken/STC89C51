
/*1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK*/

/*-----------------------------------------------
  名称：LCD1602
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：通过标准程序动态显示字符
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

sbit RS = P2^4;   //定义端口 
sbit RW = P2^5;
sbit EN = P2^6;

#define RS_CLR RS=0 
#define RS_SET RS=1
#define RW_CLR RW=0 
#define RW_SET RW=1 
#define EN_CLR EN=0
#define EN_SET EN=1
/******************************************************************/
/*                    微秒延时函数                                */
/******************************************************************/
void delay_us(unsigned int n) //延时 如果需要高精度延时 请嵌入汇编
{ 
 if (n == 0) 
  {  
  return ;  
  } 
  while (--n);
 }
/******************************************************************/
/*                    毫秒函数声明                                */
/******************************************************************/
 void delay_ms(unsigned char i) 
 { 
 unsigned char a, b; 
 for (a = 1; a < i; a++) 
 {
   for (b = 1; b; b++) 
   {   ;   } 
    }
 }
/******************************************************************/
/*                   写入命令函数                                 */
/******************************************************************/
 void LCD_write_com(unsigned char com) 
 {  
 RS_CLR; 
 RW_CLR; 
 EN_SET; 
 P0 = com; 
 delay_us(5); 
 EN_CLR;
 }
/******************************************************************/
/*                   写入数据函数                                 */
/******************************************************************/
 void LCD_write_Data(unsigned char Data) 
 { 
 RS_SET; 
 RW_CLR; 
 EN_SET; 
 P0 = Data; 
 delay_us(5); 
 EN_CLR;
 }
/******************************************************************/
/*                   清屏函数                                     */
/******************************************************************/
 void LCD_clear(void) 
 { 
 LCD_write_com(0x01); 
 delay_ms(5);}
/******************************************************************/
/*                   写入字符串函数                               */
/******************************************************************/
 void LCD_write_str(unsigned char x,unsigned char y,unsigned char *s) 
 {     
 if (y == 0) 
 {     
 LCD_write_com(0x80 + x);     
 }
 else 
 {     
 LCD_write_com(0xC0 + x);     
 }        
 while (*s) 
 {     
 LCD_write_Data( *s);     
 s ++;     
 }
 }
/******************************************************************/
/*                   写入字节函数                                 */
/******************************************************************/
 void LCD_write_char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 {     
 LCD_write_com(0x80 + x);     
 }    
 else 
 {     
 LCD_write_com(0xC0 + x);     
 }        
 LCD_write_Data( Data);  
 }
/******************************************************************/
/*                   初始化函数                                   */
/******************************************************************/
 void LCD_init(void) 
 {
   LCD_write_com(0x38);    /*显示模式设置*/ 
   delay_ms(5); 
   LCD_write_com(0x38); 
   delay_ms(5); 
   LCD_write_com(0x38); 
   delay_ms(5); 
   LCD_write_com(0x38);  
   LCD_write_com(0x08);    /*显示关闭*/ 
   LCD_write_com(0x01);    /*显示清屏*/ 
   LCD_write_com(0x06);    /*显示光标移动设置*/ 
   delay_ms(5); 
   LCD_write_com(0x0C);    /*显示开及光标设置*/
   }
   
/******************************************************************/
/*                   主函数                                       */
/******************************************************************/   
void main(void) 
{ 
unsigned char i; 
unsigned char *p; 
delay_ms(100); 
LCD_init(); 
while (1) 
{  
i = 1;  
p = "www.doflye.cn";  
LCD_clear();  
LCD_write_str(2,0,"Welcome to");  
delay_ms(250);   
while (*p) 
{   
LCD_write_char(i,1,*p);   
i ++;   
p ++;   
delay_ms(250);   
}  
delay_ms(250);  
}
}
