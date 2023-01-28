//ÊµÀý87£º¶ÔI2C×ÜÏßÉÏ¹Ò½Ó¶à¸öAT24C02µÄ¶ÁÐ´²Ù×÷
#include <reg51.h>         //  °üº¬51µ¥Æ¬»ú¼Ä´æÆ÷¶¨ÒåµÄÍ·ÎÄ¼þ
#include <intrins.h>       //°üº¬_nop_()º¯Êý¶¨ÒåµÄÍ·ÎÄ¼þ
#define	OP_READ1	0xa1		// Æ÷¼þ1µØÖ·ÒÔ¼°¶ÁÈ¡²Ù×÷,0xa1¼´Îª1010 0001B
#define	OP_WRITE1 0xa0		// Æ÷¼þ1µØÖ·ÒÔ¼°Ð´Èë²Ù×÷,0xa1¼´Îª1010 0000B
#define	OP_READ2	0xaf		// Æ÷¼þ2µØÖ·ÒÔ¼°¶ÁÈ¡²Ù×÷,0xa1¼´Îª1010 1111B
#define	OP_WRITE2 0xae		// Æ÷¼þ2µØÖ·ÒÔ¼°Ð´Èë²Ù×÷,0xa1¼´Îª1010 1110B
sbit SDA=P3^4;             //½«´®ÐÐÊý¾Ý×ÜÏßSDAÎ»¶¨ÒåÔÚÎªP3.4Òý½Å
sbit SCL=P3^3;             //½«´®ÐÐÊ±ÖÓ×ÜÏßSDAÎ»¶¨ÒåÔÚÎªP3.3Òý½Å
/*****************************************************
º¯Êý¹¦ÄÜ£ºÑÓÊ±1ms
(3j+2)*i=(3¡Á33+2)¡Á10=1010(Î¢Ãë)£¬¿ÉÒÔÈÏÎªÊÇ1ºÁÃë
***************************************************/
void delay1ms()
{
   unsigned char i,j;	
	 for(i=0;i<10;i++)
	  for(j=0;j<33;j++)
	   ;		 
 }
/*****************************************************
º¯Êý¹¦ÄÜ£ºÑÓÊ±Èô¸ÉºÁÃë
Èë¿Ú²ÎÊý£ºn
***************************************************/
 void delaynms(unsigned char n)
 {
   unsigned char i;
	for(i=0;i<n;i++)
	   delay1ms();
 }
/***************************************************
º¯Êý¹¦ÄÜ£º¿ªÊ¼Êý¾Ý´«ËÍ
***************************************************/
void start()
// ¿ªÊ¼Î»
{
	SDA = 1;    //SDA³õÊ¼»¯Îª¸ßµçÆ½¡°1¡±
	SCL = 1;    //¿ªÊ¼Êý¾Ý´«ËÍÊ±£¬ÒªÇóSCLÎª¸ßµçÆ½¡°1¡±
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	SDA = 0;    //SDAµÄÏÂ½µÑØ±»ÈÏÎªÊÇ¿ªÊ¼ÐÅºÅ
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	SCL = 0;    //SCLÎªµÍµçÆ½Ê±£¬SDAÉÏÊý¾Ý²ÅÔÊÐí±ä»¯(¼´ÔÊÐíÒÔºóµÄÊý¾Ý´«µÝ£©
	_nop_();    //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
}
/***************************************************
º¯Êý¹¦ÄÜ£º½áÊøÊý¾Ý´«ËÍ
***************************************************/
void stop()
// Í£Ö¹Î»
{
	SDA = 0;     //SDA³õÊ¼»¯ÎªµÍµçÆ½¡°0¡±
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	SCL = 1;     //½áÊøÊý¾Ý´«ËÍÊ±£¬ÒªÇóSCLÎª¸ßµçÆ½¡°1¡±
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	_nop_();     //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ
	SDA = 1;    //SDAµÄÉÏÉýÑØ±»ÈÏÎªÊÇ½áÊøÐÅºÅ	
}
/***************************************************
º¯Êý¹¦ÄÜ£º´ÓAT24Cxx¶ÁÈ¡Êý¾Ý
³ö¿Ú²ÎÊý£ºx
***************************************************/
unsigned char ReadData()
// ´ÓAT24CxxÒÆÈëÊý¾Ýµ½MCU
{
	unsigned char i;
	unsigned char x;           //´¢´æ´ÓAT24CxxÖÐ¶Á³öµÄÊý¾Ý
	for(i = 0; i < 8; i++)
	{
		SCL = 1;                //SCLÖÃÎª¸ßµçÆ½
		x<<=1;                  //½«xÖÐµÄ¸÷¶þ½øÎ»Ïò×óÒÆÒ»Î»
		x|=(unsigned char)SDA;  //½«SDAÉÏµÄÊý¾ÝÍ¨¹ý°´Î»¡°»ò¡°ÔËËã´æÈëxÖÐ
		SCL = 0;               //ÔÚSCLµÄÏÂ½µÑØ¶Á³öÊý¾Ý
	}
	return(x);                //½«¶ÁÈ¡µÄÊý¾Ý·µ»Ø
}
/***************************************************
º¯Êý¹¦ÄÜ£ºÏòAT24CxxµÄµ±Ç°µØÖ·Ð´ÈëÊý¾Ý
Èë¿Ú²ÎÊý£ºy (´¢´æ´ýÐ´ÈëµÄÊý¾Ý£©
***************************************************/
//ÔÚµ÷ÓÃ´ËÊý¾ÝÐ´Èëº¯ÊýÇ°ÐèÊ×ÏÈµ÷ÓÃ¿ªÊ¼º¯Êýstart(),ËùÒÔSCL=0
bit WriteCurrent(unsigned char y)
{
	unsigned char i;
	bit ack_bit;               //´¢´æÓ¦´ðÎ»
	for(i = 0; i < 8; i++)		// Ñ­»·ÒÆÈë8¸öÎ»
	{
    	SDA = (bit)(y&0x80);   //Í¨¹ý°´Î»¡°Óë¡±ÔËËã½«×î¸ßÎ»Êý¾ÝËÍµ½S
		                      //ÒòÎª´«ËÍÊ±¸ßÎ»ÔÚÇ°£¬µÍÎ»ÔÚºó
		_nop_();              //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ   
	   SCL = 1;              //ÔÚSCLµÄÉÏÉýÑØ½«Êý¾ÝÐ´ÈëAT24Cxx      
   	_nop_();              //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	  _nop_();               //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ       
		
	  	SCL = 0;              //½«SCLÖØÐÂÖÃÎªµÍµçÆ½£¬ÒÔÔÚSC£ÌÏßÐÎ³É´«ËÍÊý¾ÝËùÐèµÄ£¸¸öÂö³å
		y <<= 1;              //½«yÖÐµÄ¸÷¶þ½øÎ»Ïò×óÒÆÒ»Î»
	}
	SDA = 1;			  // ·¢ËÍÉè±¸£¨Ö÷»ú£©Ó¦ÔÚÊ±ÖÓÂö³åµÄ¸ßµçÆ½ÆÚ¼ä(SCL=1)ÊÍ·ÅSDAÏß£¬
	                //ÒÔÈÃSDAÏß×ªÓÉ½ÓÊÕÉè±¸(AT24Cxx)¿ØÖÆ
	_nop_();        //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	_nop_();        //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	SCL = 1;       //¸ù¾ÝÉÏÊö¹æ¶¨£¬SCLÓ¦Îª¸ßµçÆ½
	_nop_();       //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	_nop_();       //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	_nop_();       //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	_nop_();       //µÈ´ýÒ»¸ö»úÆ÷ÖÜÆÚ 
	ack_bit = SDA; //½ÓÊÜÉè±¸£¨AT24Cxx)ÏòSDAËÍµÍµçÆ½£¬±íÊ¾ÒÑ¾­½ÓÊÕµ½Ò»¸ö×Ö½Ú
	               //ÈôËÍ¸ßµçÆ½£¬±íÊ¾Ã»ÓÐ½ÓÊÕµ½£¬´«ËÍÒì³£
	SCL = 0;       //SCLÎªµÍµçÆ½Ê±£¬SDAÉÏÊý¾Ý²ÅÔÊÐí±ä»¯(¼´ÔÊÐíÒÔºóµÄÊý¾Ý´«µÝ£©
	return  ack_bit;// ·µ»ØAT24CxxÓ¦´ðÎ»
}
/***************************************************
º¯Êý¹¦ÄÜ£ºÏòµÚÒ»¸öAT24CxxÖÐµÄÖ¸¶¨µØÖ·Ð´ÈëÊý¾Ý
Èë¿Ú²ÎÊý£ºadd (´¢´æÖ¸¶¨µÄµØÖ·£©£»dat(´¢´æ´ýÐ´ÈëµÄÊý¾Ý£©
***************************************************/
void WriteSet1(unsigned char add, unsigned char dat)
// ÔÚÖ¸¶¨µØÖ·addr´¦Ð´ÈëÊý¾ÝWriteCurrent
{
	start();                  //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_WRITE1);  //Ñ¡ÔñÒª²Ù×÷µÄµÚÒ»¸öAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÔÆäÐ´ÈëÊý¾Ý
	WriteCurrent(add);        //Ð´ÈëÖ¸¶¨µØÖ·
	WriteCurrent(dat);        //Ïòµ±Ç°µØÖ·£¨ÉÏÃæÖ¸¶¨µÄµØÖ·£©Ð´ÈëÊý¾Ý
	stop();                   //Í£Ö¹Êý¾Ý´«µÝ
	delaynms(4);	           //1¸ö×Ö½ÚµÄÐ´ÈëÖÜÆÚÎª1ms, ×îºÃÑÓÊ±1msÒÔÉÏ
}
/***************************************************
º¯Êý¹¦ÄÜ£ºÏòµÚ¶þ¸öAT24CxxÖÐµÄÖ¸¶¨µØÖ·Ð´ÈëÊý¾Ý
Èë¿Ú²ÎÊý£ºadd (´¢´æÖ¸¶¨µÄµØÖ·£©£»dat(´¢´æ´ýÐ´ÈëµÄÊý¾Ý£©
***************************************************/
void WriteSet2(unsigned char add, unsigned char dat)
// ÔÚÖ¸¶¨µØÖ·addr´¦Ð´ÈëÊý¾ÝWriteCurrent
{
	start();                  //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_WRITE2);  //Ñ¡ÔñÒª²Ù×÷µÄAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÔÆäÐ´ÈëÊý¾Ý
	WriteCurrent(add);        //Ð´ÈëÖ¸¶¨µØÖ·
	WriteCurrent(dat);        //Ïòµ±Ç°µØÖ·£¨ÉÏÃæÖ¸¶¨µÄµØÖ·£©Ð´ÈëÊý¾Ý
	stop();                   //Í£Ö¹Êý¾Ý´«µÝ
	delaynms(4);	           //1¸ö×Ö½ÚµÄÐ´ÈëÖÜÆÚÎª1ms, ×îºÃÑÓÊ±1msÒÔÉÏ
}
/***************************************************
º¯Êý¹¦ÄÜ£º´ÓµÚÒ»¸öAT24CxxÖÐµÄµ±Ç°µØÖ·¶ÁÈ¡Êý¾Ý
³ö¿Ú²ÎÊý£ºx (´¢´æ¶Á³öµÄÊý¾Ý£© 
***************************************************/
unsigned char ReadCurrent1()
{
	unsigned char x;
	start();                   //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_READ1);   //Ñ¡ÔñÒª²Ù×÷µÄµÚÒ»¸öAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÁÆäÊý¾Ý
	x=ReadData();             //½«¶ÁÈ¡µÄÊý¾Ý´æÈëx
	stop();                   //Í£Ö¹Êý¾Ý´«µÝ
	return x;                 //·µ»Ø¶ÁÈ¡µÄÊý¾Ý
}

/***************************************************
º¯Êý¹¦ÄÜ£º´ÓµÚ¶þ¸öAT24CxxÖÐµÄµ±Ç°µØÖ·¶ÁÈ¡Êý¾Ý
³ö¿Ú²ÎÊý£ºx (´¢´æ¶Á³öµÄÊý¾Ý£© 
***************************************************/
unsigned char ReadCurrent2()
{
	unsigned char x;
	start();                   //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_READ2);    //Ñ¡ÔñÒª²Ù×÷µÄµÚ¶þ¸öAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÁÆäÊý¾Ý
	x=ReadData();              //½«¶ÁÈ¡µÄÊý¾Ý´æÈëx
	stop();                    //Í£Ö¹Êý¾Ý´«µÝ
	return x;                  //·µ»Ø¶ÁÈ¡µÄÊý¾Ý
}
/***************************************************
º¯Êý¹¦ÄÜ£º´ÓµÚÒ»¸öAT24CxxÖÐµÄÖ¸¶¨µØÖ·¶ÁÈ¡Êý¾Ý
Èë¿Ú²ÎÊý£ºset_addr
³ö¿Ú²ÎÊý£ºx 
***************************************************/
unsigned char ReadSet1(unsigned char set_addr)
// ÔÚÖ¸¶¨µØÖ·¶ÁÈ¡
{
	start();                      //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_WRITE1);       //Ñ¡ÔñÒª²Ù×÷µÄµÚÒ»¸öAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÔÆäÐ´ÈëÊý¾Ý
	WriteCurrent(set_addr);       //Ð´ÈëÖ¸¶¨µØÖ·
	return(ReadCurrent1());        //´ÓµÚÒ»¸öAT24CxxÐ¾Æ¬Ö¸¶¨µØÖ·¶Á³öÊý¾Ý²¢·µ»Ø
}
/***************************************************
º¯Êý¹¦ÄÜ£º´ÓµÚ¶þ¸öAT24CxxÖÐµÄÖ¸¶¨µØÖ·¶ÁÈ¡Êý¾Ý
Èë¿Ú²ÎÊý£ºset_addr
³ö¿Ú²ÎÊý£ºx 
***************************************************/
unsigned char ReadSet2(unsigned char set_addr)
// ÔÚÖ¸¶¨µØÖ·¶ÁÈ¡
{
	start();                       //¿ªÊ¼Êý¾Ý´«µÝ
	WriteCurrent(OP_WRITE2);       //Ñ¡ÔñÒª²Ù×÷µÄµÚ¶þ¸öAT24CxxÐ¾Æ¬£¬²¢¸æÖªÒª¶ÔÆäÐ´ÈëÊý¾Ý
	WriteCurrent(set_addr);        //Ð´ÈëÖ¸¶¨µØÖ·
	return(ReadCurrent2());        //´ÓµÚ¶þ¸öAT24CxxÐ¾Æ¬Ö¸¶¨µØÖ·¶Á³öÊý¾Ý²¢·µ»Ø
}
/***************************************************
º¯Êý¹¦ÄÜ£ºÖ÷º¯Êý
***************************************************/
main(void)
{
   unsigned char x;
   SDA = 1;           // SDA=1,SCL=1,Ê¹Ö÷´ÓÉè±¸´¦ÓÚ¿ÕÏÐ×´Ì¬
	SCL = 1;  	       
   WriteSet1(0x36,0xaa);   //½«Êý¾Ý"0xaa"Ð´ÈëµÚÒ»¸öAT24C02µÄÖ¸¶¨µØÖ·"0x36"
	x=ReadSet1(0x36);       //´ÓµÚ¶þ¸öAT24C02ÖÐµÄÖ¸¶¨µØÖ·"0x36"¶Á³öÊý¾Ý  
   WriteSet2(0x48,x);      //½«¶Á³öµÄÊý¾ÝÐ´ÈëµÚ¶þ¸öAT24C02µÄÖ¸¶¨µØÖ·"0x48"±
	P1=ReadSet2(0x48);      //½«´ÓµÚ¶þ¸öAT24C02µÄÖ¸¶¨µØÖ·¶Á³öµÄÊý¾ÝËÍP1¿ÚÏÔÊ¾ÑéÖ¤
}
