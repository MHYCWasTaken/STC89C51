#include <REG51.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit  DS1302_CLK = P2^4;              //实时时钟时钟线引脚 
sbit  DS1302_IO  = P2^5;              //实时时钟数据线引脚 
sbit  DS1302_RST = P2^6;              //实时时钟复位线引脚
sbit  DQ = P2^7;                      //温度传送数据IO口
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
uchar hide_sec,hide_min,hide_hour,hide_day,hide_week,hide_month,hide_year;  //秒,分,时到日,月,年位闪的计数
sbit Set = P1^0;       //模式切换键 对应实验板SW19
sbit Up  = P1^1;        //加法按钮 对应实验板SW20
sbit Down= P1^2;      //减法按钮  对应实验板SW21
sbit out = P1^3;       //立刻跳出调整模式按钮 对应实验板SW22
uchar done,count,temp,up_flag,down_flag;
uchar temp_value,temp1_value;      //temp_value温度值正数部分 temp1_value温度值小数部分 
uchar TempBuffer[8],week_value[3];
void show_time();   //液晶显示程序
//****************************************
//12864液晶显示部分子程序模块
//****************************************
sbit rs		= P3^0;
sbit rw		= P3^1;
sbit e  	= P3^2;
#define lcddata P0
sbit busy=P0^7;   //lcd busy bit
void wr_d_lcd(uchar content);
void wr_i_lcd(uchar content);
void clrram_lcd (void);
void init_lcd(void);
void busy_lcd(void);
void rev_row_lcd(uchar row);
void rev_co_lcd(uchar row,uchar col,uchar mode);
void clr_lcd(void);
void wr_co_lcd(uchar row,uchar col,uchar lcddata1,uchar lcddtta2);
void wr_row_lcd(uchar row,char *p);
//**********************************
//液晶初始化
//**********************************
void init_lcd(void)
{
	wr_i_lcd(0x06);  /*光标的移动方向*/
	wr_i_lcd(0x0c);  /*开显示，关游标*/
}
//***********************************
//填充液晶DDRAM全为空格
//**********************************
void clrram_lcd (void)
{
	wr_i_lcd(0x30);
	wr_i_lcd(0x01);
}
//***********************************
//对液晶写数据
//content为要写入的数据
//***********************************
void wr_d_lcd(uchar content)
{
	busy_lcd();
	rs=1;
    rw=0;
	lcddata=content;
	e=1;
	;
	e=0;
}
//********************************
//对液晶写指令
//content为要写入的指令代码
//*****************************
void wr_i_lcd(uchar content)
{
	busy_lcd();
	rs=0;
    rw=0;
	lcddata=content;
	e=1;
	;
	e=0;
}
//********************************
//液晶检测忙状态
//在写入之前必须执行
//********************************
void busy_lcd(void)
{
  lcddata=0xff;
  rs=0;
  rw=1;
  e =1;
  while(busy==1);
  e =0;
}
//********************************
//指定要显示字符的坐标
//*******************************
void gotoxy(unsigned char y, unsigned char x)
{
	if(y==1)
		wr_i_lcd(0x80|x);
	if(y==2)
        wr_i_lcd(0x90|x);
	if(y==3)
		wr_i_lcd((0x80|x)+8);
	if(y==4)
        wr_i_lcd((0x90|x)+8);
}
//**********************************
//液晶显示字符串程序
//**********************************
void print(uchar *str)
{
	while(*str!='\0')
	{
		wr_d_lcd(*str);
		str++;
	}
}






//***********************************
//DS1302时钟部分子程序模块
//***********************************
typedef struct __SYSTEMTIME__
{
	uchar Second;
	uchar Minute;
	uchar Hour;
	uchar Week;
	uchar Day;
	uchar Month;
	uchar  Year;
	uchar DateString[11];
	uchar TimeString[9];
}SYSTEMTIME;	//定义的时间类型
SYSTEMTIME CurrentTime;


#define AM(X)	X
#define PM(X)	(X+12)            	  // 转成24小时制
#define DS1302_SECOND	0x80          //时钟芯片的寄存器位置,存放时间
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84 
#define DS1302_WEEK		0x8A
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_YEAR		0x8C 
//**********************************
//实时时钟写入一字节(内部函数)
//**********************************
void DS1302InputByte(uchar d) 	
{ 
    uchar i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//相当于汇编中的 RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}
//*************************************
//实时时钟读取一字节(内部函数)
//*************************************
uchar DS1302OutputByte(void) 	
{ 
    uchar i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//相当于汇编中的 RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}
//**************************************
//ucAddr: DS1302地址, ucData: 要写的数据
//**************************************
void Write1302(uchar ucAddr, uchar ucDa)	
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// 地址，命令 
    DS1302InputByte(ucDa);       	// 写1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 
//**************************************
//读取DS1302某地址的数据
//**************************************
uchar Read1302(uchar ucAddr)	
{
    uchar ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // 地址，命令 
    ucData = DS1302OutputByte();         // 读1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucData);
}

//******************************************
//获取时钟芯片的时钟数据到自定义的结构型数组
//******************************************
void DS1302_GetTime(SYSTEMTIME *Time)  
{
	uchar ReadValue;
	ReadValue = Read1302(DS1302_SECOND);
	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//转换为相应的10进制数
	ReadValue = Read1302(DS1302_MINUTE);
	Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_HOUR);
	Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_DAY);
	Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
	ReadValue = Read1302(DS1302_WEEK);
	Time->Week = ((ReadValue&0x10)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_YEAR);
	Time->Year = ((ReadValue&0xf0)>>4)*10 + (ReadValue&0x0F);	
}
//******************************************
//将时间年,月,日,星期数据转换成液
//晶显示字符串,放到数组里DateString[]
//******************************************
void DateToStr(SYSTEMTIME *Time)    
{   
   uchar tab[ ]={0XD2,0XBB,0XB6,0XFE,0XC8,0XFD,0XCB,0XC4,0XCE,0XE5,0XC1,0XF9,0XC8,0XD5};
   if(hide_year<2)          //这里的if,else语句都是判断位闪烁,<2显示数据,>2就不显示,输出字符串为 2007/07/22
    {                               
	  Time->DateString[0] = '2';
	  Time->DateString[1] = '0';	 
	  Time->DateString[2] = Time->Year/10 + '0';
	  Time->DateString[3] = Time->Year%10 + '0';
	}
	  else
	    { 
	      Time->DateString[0] = ' ';
	      Time->DateString[1] = ' ';		 
	      Time->DateString[2] = ' ';
	      Time->DateString[3] = ' ';
		}
    Time->DateString[4]='-';
	if(hide_month<2)
	{
	  Time->DateString[5] = Time->Month/10 + '0';
	  Time->DateString[6] = Time->Month%10 + '0';
	}
	  else
	  {
	    Time->DateString[5] = ' ';
	    Time->DateString[6] = ' ';
	  }
    Time->DateString[7]='-';
	if(hide_day<2)
	{
	  Time->DateString[8] = Time->Day/10 + '0';
	  Time->DateString[9] = Time->Day%10 + '0';
	}
	  else
	  {
	    Time->DateString[8] = ' ';
	    Time->DateString[9] = ' ';	    
	  }
	if(hide_week<2)
	{
	  week_value[0] =tab[2*(Time->Week%10)-2];  //星期的数据另外放到 week_value[]数组里,跟年,月,日的分开存放,因为等一下要在最后显示
	  week_value[1] =tab[2*(Time->Week%10)-1];
	}
	  else
	  {
	    week_value[0] = ' ';
		week_value[1]=' ';
	  }
	  week_value[2] = '\0';

	Time->DateString[10] = '\0'; //字符串末尾加 '\0' ,判断结束字符
}
//******************************************
//将时,分,秒数据转换成液晶
//显示字符放到数组 TimeString[]
//*****************************************
void TimeToStr(SYSTEMTIME *Time) 
{   if(hide_hour<2)
    {
	  Time->TimeString[0] = Time->Hour/10 + '0';
	  Time->TimeString[1] = Time->Hour%10 + '0';
	}
	  else
	    {
	      Time->TimeString[0] = ' ';
	      Time->TimeString[1] = ' ';
		}
	Time->TimeString[2] = ':';
    if(hide_min<2)
	{
	  Time->TimeString[3] = Time->Minute/10 + '0';
	  Time->TimeString[4] = Time->Minute%10 + '0';
	}
	  else
	    {
	      Time->TimeString[3] = ' ';
	      Time->TimeString[4] = ' ';
   	    }
	Time->TimeString[5] = ':';
    if(hide_sec<2)
    {
	  Time->TimeString[6] = Time->Second/10 + '0';
	  Time->TimeString[7] = Time->Second%10 + '0';
    }
      else
       {
         Time->TimeString[6] = ' ';
	     Time->TimeString[7] = ' ';
       }
	Time->TimeString[8] = '\0';
}

//******************************
//时钟芯片初始化
//******************************
void Initial_DS1302(void)   
{   
	uchar Second=Read1302(DS1302_SECOND);
	if(Second&0x80)	      //判断时钟芯片是否关闭	  
    {
	Write1302(0x8e,0x00); //写入允许
	Write1302(0x8c,0x07); //以下写入初始化时间 日期:07/07/25.星期: 3. 时间: 23:59:55
	Write1302(0x88,0x07);
	Write1302(0x86,0x25);
	Write1302(0x8a,0x07);
	Write1302(0x84,0x23);
	Write1302(0x82,0x59);
	Write1302(0x80,0x55);
	Write1302(0x8e,0x80); //禁止写入
	}

}

//****************************************
//ds18b20子程序模块
//****************************************


/***********ds18b20子程序*************************/

/***********ds18b20延迟子函数（晶振12MHz ）*******/ 

void delay_18B20(unsigned int i)
{
	while(i--);
}

/**********ds18b20初始化函数**********************/

void Init_DS18B20(void) 
{
	 unsigned char x=0;
	 DQ = 1;          //DQ复位
	 delay_18B20(8);  //稍做延时
	 DQ = 0;          //单片机将DQ拉低
	 delay_18B20(40); //精确延时 大于 480us
	 DQ = 1;          //拉高总线
	 delay_18B20(7);
	 x=DQ;            //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	 delay_18B20(10);
}

/***********ds18b20读一个字节**************/  

uchar ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	 {
		  DQ = 0; // 给脉冲信号
		  dat>>=1;
		  DQ = 1; // 给脉冲信号
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(4);
	 }
 	return(dat);
}

/*************ds18b20写一个字节****************/  

void WriteOneChar(uchar dat)
{
 	uchar i=0;
 	for (i=8; i>0; i--)
 	{
  		DQ = 0;
 		DQ = dat&0x01;
    	delay_18B20(5);
 		DQ = 1;
    	dat>>=1;
 }
}

/**************读取ds18b20当前温度************/

void ReadTemp(void)
{
	uchar a=0;
	uchar b=0;
	uchar t;

	Init_DS18B20();
	WriteOneChar(0xCC);    	// 跳过读序号列号的操作
	WriteOneChar(0x44); 	// 启动温度转换

	delay_18B20(100);       // this message is wery important

	Init_DS18B20();
	WriteOneChar(0xCC); 	//跳过读序号列号的操作
	WriteOneChar(0xBE); 	//读取温度寄存器等（共可读9个寄存器） 前两个就是温度

	delay_18B20(50);

	a=ReadOneChar();    	//读取温度值低位
	b=ReadOneChar();   		//读取温度值高位
	t=b&0xf8;
	if(t)
	{
	  TempBuffer[0]=':';
	  TempBuffer[1]='-';
  	  temp_value=b<<4;
	  temp_value+=(a&0xf0)>>4;  
	  temp_value=~temp_value+1;
	  temp1_value=~a&0x0f;
	}
	else
	{  
        temp_value=b<<4;
        temp_value+=(a&0xf0)>>4; 
		 temp1_value=a&0x0f;
	    TempBuffer[0]=':';
	    TempBuffer[1]=temp_value/100+'0';  //百位
       if(TempBuffer[1]=='1')
       {
            TempBuffer[1]='1';
       }
       else
       {
           TempBuffer[1]=' ';
       } 
    }
}
void temp_to_str()   //温度数据转换成液晶字符显示
{		

  TempBuffer[2]=temp_value%100/10+'0';  //十位
  TempBuffer[3]=temp_value%10+'0';  //个位
  TempBuffer[4]='.';
  TempBuffer[5]=temp1_value*625/1000%10+'0';
  TempBuffer[6]=temp1_value*625/100%10+'0';
  TempBuffer[7]=temp1_value*625/10%10+'0';
  //TempBuffer[8]=temp1_value*625%10+'0';
  TempBuffer[8]='\0';
}
//**********************************************
//延时子程序模块
//**********************************************
void mdelay(uint delay)
{	uint i;
 	for(;delay>0;delay--)
   		{for(i=0;i<80;i++) //1ms延时.
       		{;}
   		}
}

//************************************************
//按键设置程序模块
//************************************************

//************************************
//跳出调整模式,返回默认显示
//************************************
void outkey()                    
{ uchar Second;
  if(out==0)         
  { mdelay(5); 
	count=0;
	hide_sec=0,hide_min=0,hide_hour=0,hide_day=0,hide_week=0,hide_month=0,hide_year=0;
	Second=Read1302(DS1302_SECOND);
    Write1302(0x8e,0x00); //写入允许
	Write1302(0x80,Second&0x7f);
	Write1302(0x8E,0x80);          //禁止写入
	done=0;           
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*************************
//升序按键
//*************************
void Upkey()
{	   
		Up=1;
	            if(Up==0)
		          {
				   mdelay(5);
				       switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //读取秒数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //秒数加1
                                  up_flag=1;    //数据调整后更新标志
								  if((temp)>59)   //超过59秒,清零
                                  temp=0;	
							  	  temp=temp/10*16+temp%10;
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //读取分数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //分数加1
                                  up_flag=1;
								  if(temp>59)          //超过59分,清零
								  temp=0;
								  temp=temp/10*16+temp%10;
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //读取小时数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //小时数加1
                                  up_flag=1;
								  if(temp>23)   //超过23小时,清零
								  temp=0;
								  temp=temp/10*16+temp%10;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //读取星期数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //星期数加1
                                  up_flag=1;
								  if(temp>7)  
								  temp=1;
								  temp=temp/10*16+temp%10;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //读取日数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //日数加1
                                  up_flag=1;
								  if(temp>31)
								  temp=1;
								  temp=temp/10*16+temp%10;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //读取月数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //月数加1
                                  up_flag=1;
								  if(temp>12)
								  temp=1;
								  temp=temp/10*16+temp%10;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //读取年数
								  temp=((temp&0xf0)>>4)*10 + (temp&0x0F);
								  temp=temp+1;  //年数加1
                                  up_flag=1;
								  if(temp>99)
								  temp=0;
								  temp=temp/10*16+temp%10;
								  break;
					       default:break;
				          }
					  
				 //  while(Up==0);
				  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//************************
//降序按键
//************************
void Downkey()
{	    
		Down=1;
	    if(Down==0)
		          {
				   mdelay(5);
				     switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //读取秒数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;						    //秒数减1
                                  down_flag=1;       //数据调整后更新标志
								  if(temp==-1)     //小于0秒,返回59秒
								  temp=59;
								  temp=temp/10*16+temp%10;
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //读取分数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //分数减1
                                  down_flag=1;
								  if(temp==-1)
								  temp=59;      //小于0秒,返回59秒
								  temp=temp/10*16+temp%10;
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //读取小时数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //小时数减1
                                  down_flag=1;
								  if(temp==-1)
								  temp=23;
								  temp=temp/10*16+temp%10;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //读取星期数;
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //星期数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=7;
								  temp=temp/10*16+temp%10;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //读取日数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //日数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=31;
								  temp=temp/10*16+temp%10;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //读取月数
								  temp=((temp&0x70)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //月数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=12;
								  temp=temp/10*16+temp%10;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //读取年数
								  temp=((temp&0xf0)>>4)*10 + (temp&0x0F);
								  temp=temp-1;  //年数减1
                                  down_flag=1;
								  if(temp==-1)
								  temp=99;
								  temp=temp/10*16+temp%10;
								  break;
					      default:break;
				         }
					 
				 //  while(Down==0);
				  }
}

//**************************
//模式选择按键
//**************************
void Setkey()
{
		Set=1;
		if(Set==0)
	    {
           mdelay(5);
           count=count+1;	 //Setkey按一次,count就加1
		   done=1;			 //进入调整模式
           while(Set==0);
		 }

}

//*************************
//按键功能执行
//*************************
void keydone()
{        uchar Second;
		/* if(flag==0)    //关闭时钟,停止计时
         { Write1302(0x8e,0x00); //写入允许
           temp=Read1302(0x80);
           Write1302(0x80,temp|0x80);
	       Write1302(0x8e,0x80); //禁止写入
           flag=1;
         }*/
         Setkey();				            //扫描模式切换按键
		 switch(count)
		 {case 1:do					        //count=1,调整秒
		          {
                   
                   outkey();			   //扫描跳出按钮
				   Upkey();                //扫描加按钮
				   Downkey();              //扫描减按钮
				   if(up_flag==1||down_flag==1)  //数据更新，重新写入新的数据
				   {
				     Write1302(0x8e,0x00); //写入允许
				     Write1302(0x80,temp); //写入新的秒数
				     Write1302(0x8e,0x80); //禁止写入
				     up_flag=0;
			    	 down_flag=0;
				  }
                     if(Down!=0&&Up!=0)
                     {
						hide_sec++;
				        if(hide_sec>3)
				        hide_sec=0;
                     }
					 else hide_sec=0;
					 show_time();         //液晶显示数据
				  }while(count==2);break;  
		  case 2:do					        //count=2,调整分
		          {
				   hide_sec=0;
				   outkey();
				   Upkey();
				   Downkey();
				   if(temp>0x60)
				     temp=0;
				   if(up_flag==1||down_flag==1)
				   {
				     Write1302(0x8e,0x00); //写入允许
				     Write1302(0x82,temp); //写入新的分数
				     Write1302(0x8e,0x80); //禁止写入
				     up_flag=0;
				     down_flag=0;
				  }
                     if(Down!=0&&Up!=0)
                     {
						hide_min++;
				        if(hide_min>3)
				        hide_min=0;
                     }
					 else hide_min=0;
					 show_time();
				  }while(count==3);break;
		  case 3:do					        //count=3,调整小时
		          {
                   hide_min=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				      Write1302(0x8e,0x00); //写入允许
				      Write1302(0x84,temp); //写入新的小时数
				      Write1302(0x8e,0x80); //禁止写入
				      up_flag=0;
				      down_flag=0;
				   }
                      if(Down!=0&&Up!=0)
                     {
						hide_hour++;
				        if(hide_hour>3)
				        hide_hour=0;
                     }
					 else  hide_hour=0;
					  show_time();
				  }while(count==4);break;
		  case 4:do					        //count=4,调整星期
		          {
                   hide_hour=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				     Write1302(0x8e,0x00); //写入允许
				     Write1302(0x8a,temp); //写入新的星期数
				     Write1302(0x8e,0x80); //禁止写入
				     up_flag=0;
				     down_flag=0;
				   }
                     if(Down!=0&&Up!=0)
                     {
					   hide_week++;
				       if(hide_week>3)
				       hide_week=0;
                     }
					 else hide_week=0;
					 show_time();
				  }while(count==5);break;
		  case 5:do					        //count=5,调整日
		          {
				   hide_week=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				     Write1302(0x8e,0x00); //写入允许
				     Write1302(0x86,temp); //写入新的日数
				     Write1302(0x8e,0x80); //禁止写入
				     up_flag=0;
				     down_flag=0;
				   }
                     if(Down!=0&&Up!=0)
				     {
						hide_day++;
				        if(hide_day>3)
				        hide_day=0;
					 }
				    else hide_day=0;
                     show_time();
				  }while(count==6);break;
		  case 6:do					        //count=6,调整月
		          {
                   hide_day=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				      Write1302(0x8e,0x00); //写入允许
				      Write1302(0x88,temp); //写入新的月数
				      Write1302(0x8e,0x80); //禁止写入
				      up_flag=0;
				      down_flag=0;
				   }
                     if(Down!=0&&Up!=0)
				     {
						 hide_month++;
				         if(hide_month>3)
				         hide_month=0;
					 }
					else hide_month=0;
                     show_time();
				  }while(count==7);break;
		  case 7:do					        //count=7,调整年
		          {
                   hide_month=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				      Write1302(0x8e,0x00); //写入允许
				      Write1302(0x8c,temp); //写入新的年数
				      Write1302(0x8e,0x80); //禁止写入
				      up_flag=0;
				      down_flag=0;
				  }
                      if(Down!=0&&Up!=0)
				      {
					 	 hide_year++;
				         if(hide_year>3)
				         hide_year=0;
                      }
					  else hide_year=0;
                      show_time();
				  }while(count==8);break;
		  case 8: count=0;hide_year=0;  //count8, 跳出调整模式,返回默认显示状态
	              Second=Read1302(DS1302_SECOND);
                  Write1302(0x8e,0x00); //写入允许
	              Write1302(0x80,Second&0x7f);
	              Write1302(0x8E,0x80);          //禁止写入
				  done=0;
		  break; //count=7,开启中断,标志位置0并退出
		  default:break;

		 }

}


//***************************************
//液晶显示主程序模块
//***************************************
void show_time()   
{
  DS1302_GetTime(&CurrentTime);  //获取时钟芯片的时间数据
  TimeToStr(&CurrentTime);       //时间数据转换液晶字符
  DateToStr(&CurrentTime);       //日期数据转换液晶字符
  ReadTemp();                    //开启温度采集程序
  temp_to_str();                 //温度数据转换成液晶字符
  gotoxy(4,0);
  print("温度");
  gotoxy(4,2);                  //液晶字符显示位置
  print(TempBuffer);             //显示温度
  gotoxy(4,6);
  print("℃");
  gotoxy(3,0);
  print("时间:");
  gotoxy(3,3);
  print(CurrentTime.TimeString); //显示时间
  gotoxy(2,3);
  print(CurrentTime.DateString); //显示日期
  gotoxy(2,0);
  print("星期");
  gotoxy(2,2);
  print(week_value);             //显示星期
  gotoxy(1,1);
  print("【锐志电子】");
  mdelay(500);                 //扫描延时
}


//************************************
//主程序
//*************************************
main()
{
  //  flag=1;           //时钟停止标志
	init_lcd();
	clrram_lcd();
	Init_DS18B20( ) ;      //DS18B20初始化
	Initial_DS1302(); //时钟芯片初始化
	up_flag=0;
	down_flag=0;
	done=0;           //进入默认液晶显示
	while(1)
	{   
        while(done==1)
          keydone();    //进入调整模式
		while(done==0)
 	    {  
            show_time();                //液晶显示数据
           // flag=0;                  
  		    Setkey();				 //扫描各功能键
 		}		
	}
}




