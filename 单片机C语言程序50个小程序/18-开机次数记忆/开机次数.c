   
/*-----------------------------------------------
  名称：IIC协议 EEPROM24c02
  公司：上海浩豚电子科技有限公司
  编写：师访
  日期：2009.5
  修改：无
  内容：  每次开机都将次数加1 并存储到EEPROM。这样就能直观的看到机器的使用次数
          用P1口 LED做为显示，次数大于256是将溢出，按复位模拟开机 或者直接通过开关开机
------------------------------------------------*/
 #include<reg52.h>    //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
 #include <intrins.h> //包含NOP空指令函数_nop_();

 #define AddWr 0xae   //写数据地址，需要参考24c02芯片文档
 #define AddRd 0xaf   //读数据地址

 
 sbit Sda=P1^2;       //定义总线连接端口
 sbit Scl=P1^1;
 sbit WP=P1^0;        //写保护，这里不使用

/*------------------------------------------------
                    延时程序
------------------------------------------------*/
 void mDelay(unsigned char j)
 {
  unsigned int i;
  for(;j>0;j--)
     {
	  for(i=0;i<125;i++)
	     {;}
	  }
  }

/*------------------------------------------------
                    启动IIC总线
------------------------------------------------*/
  void Start(void)
  {
   Sda=1;
   _nop_();_nop_();
   Scl=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Sda=0;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Scl=0;
  }


/*------------------------------------------------
                    停止IIC总线
------------------------------------------------*/
  void Stop(void)
  {
   Sda=0;
   _nop_();
   Scl=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Sda=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   Scl=0;
   }


/*------------------------------------------------
                   应答IIC总线
------------------------------------------------*/
   void Ack(void)
   {
    Sda=0;
	_nop_();_nop_();_nop_();
	Scl=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	Scl=0;
	_nop_();_nop_();
	}


/*------------------------------------------------
                 非应答IIC总线
------------------------------------------------*/
	void NoAck(void)
	{
	 Sda=1;
	 _nop_();_nop_();_nop_();
	 Scl=1;
	 _nop_();_nop_();_nop_();_nop_();_nop_();
	 Scl=0;
	 _nop_();_nop_();
	 }


/*------------------------------------------------
              发送一个字节
------------------------------------------------*/
	 void Send(unsigned char Data)
	 { 
	  unsigned char BitCounter=8;
	  unsigned char temp;

	  do
	    {
		 temp=Data;
		 Scl=0;
		 _nop_();_nop_();_nop_();_nop_();_nop_();
		 if((temp&0x80)==0x80)
		    Sda=1;
		 else
		    Sda=0;


			Scl=1;
			temp=Data<<1;
			Data=temp;
			BitCounter--;
		  }
	  while(BitCounter);
	      Scl=0;
	  }


/*------------------------------------------------
               读入一个字节并返回
------------------------------------------------*/
	  unsigned char Read(void)
	  {
	   unsigned char temp=0;
	   unsigned char temp1=0;
	   unsigned char BitCounter=8;

	   Sda=1;
	   do
	     {
		  Scl=0;
          _nop_();_nop_();_nop_();_nop_();_nop_();
		  Scl=1;
		  _nop_();_nop_();_nop_();_nop_();_nop_();
		  if(Sda)
		     temp=temp|0x01;
		  else
		     temp=temp&0xfe;

		  if(BitCounter-1)
		     {
			  temp1=temp<<1;
			  temp=temp1;
			  }
			  BitCounter--;
			 }
		while(BitCounter);
		return(temp);
	  }
 
/*------------------------------------------------
                    写入数据
------------------------------------------------*/
	  void WrToROM(unsigned char Data[],unsigned char Address,unsigned char Num)
	  {
	   unsigned char i;
	   unsigned char *PData;
	   PData=Data;
	   for(i=0;i<Num;i++)
	      {
		   Start();
		   Send(AddWr);    //写入芯片地址
		   Ack();
		   Send(Address+i);//写入存储地址
		   Ack();
		   Send(*(PData+i));//写数据
		   Ack();
		   Stop();
		   mDelay(20);
		   }
	   }

/*------------------------------------------------
                 读出数据
------------------------------------------------*/
	  void RdFromROM(unsigned char Data[],unsigned char Address,unsigned char Num)
	  {
	   unsigned char i;
	   unsigned char *PData;
	   PData=Data;
       for(i=0;i<Num;i++)
	      {
		   Start();        //写入芯片地址
		   Send(AddWr);
		   Ack();
 		   Send(Address+i);//写入存储地址
		   Ack();
		   Start();
		   Send(AddRd);    //读入地址
		   Ack();
		   *(PData+i)=Read();//读数据
		   Scl=0;
		   NoAck();
		   Stop();
		   }
	  }
//以上程序是EEPROM24C02程序部分，IIC协议Scl=0;
/*------------------------------------------------
                   主程序
------------------------------------------------*/

	void main()
	{
	 unsigned char Number[1];
	 unsigned char i;
	 WP=0;//写保护关掉
	 
	 RdFromROM(Number,0,1);//调用存储数据 从地址0开始写一个数据
	 Number[0]++;
	 WrToROM(Number,0,1);//写入24c02
     P1 = ~ Number[0];//在P1口上显示 反向显示 LED亮代表1
	 
	 
	 while(1)
	      { 
		  }

}
//用户可以自己把程序移植到数码管显示或者液晶显示 ，可以直观的看到数字 ，也可以定义int类型 用于计数万次以上 

