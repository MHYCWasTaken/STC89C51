#include "reg52.h"
#include "intrins.h"
sbit io_LCD12864_RS = P2^5 ;
sbit io_LCD12864_RW = P2^6 ;
sbit io_LCD12864_EN = P2^7 ;

#define io_LCD12864_DATAPORT P0

#define SET_DATA  io_LCD12864_RS = 1 ;
#define SET_INC  io_LCD12864_RS = 0 ;
#define SET_READ  io_LCD12864_RW = 1 ;
#define SET_WRITE io_LCD12864_RW = 0 ;
#define SET_EN    io_LCD12864_EN = 1 ;
#define CLR_EN    io_LCD12864_EN = 0 ;

void v_Lcd12864CheckBusy_f( void )      //忙检测函数
{
    unsigned int nTimeOut = 0 ;
    SET_INC
    SET_READ
    CLR_EN
    SET_EN
    while( ( io_LCD12864_DATAPORT & 0x80 ) && ( ++nTimeOut != 0 ) ) ;
    CLR_EN
    SET_INC
    SET_READ
}

void v_Lcd12864SendCmd_f( unsigned char byCmd )      //发送命令
{
    v_Lcd12864CheckBusy_f() ;
    SET_INC
    SET_WRITE
    CLR_EN
    io_LCD12864_DATAPORT = byCmd ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}
void v_Lcd12864SendData_f( unsigned char byData )    //发送数据
{
    v_Lcd12864CheckBusy_f() ;
    SET_DATA
    SET_WRITE
    CLR_EN
    io_LCD12864_DATAPORT = byData ;
    _nop_();
    _nop_();
    SET_EN  
    _nop_();
    _nop_();
    CLR_EN
    SET_READ
    SET_INC
}

void v_DelayMs_f( unsigned int nDelay )              //延时
{
    unsigned int i ;
    for( ; nDelay > 0 ; nDelay-- )
    {
        for( i = 125 ; i > 0 ; i-- ) ;
    }
}

void v_Lcd12864Init_f( void )                  //初始化
{
    v_Lcd12864SendCmd_f( 0x30 ) ;              //基本指令集
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x01 ) ;              //清屏
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x06 ) ;              //光标右移
    v_DelayMs_f( 50 ) ;
    v_Lcd12864SendCmd_f( 0x0c ) ;              //开显示
}
void v_Lcd12864SetAddress_f( unsigned char x, y )  //地址转换
{
    unsigned char byAddress ;
    switch( y )
    {
        case 0 :    byAddress = 0x80 + x ;
            break;
        case 1 :    byAddress = 0x90 + x ;
            break ;
        case 2 :    byAddress = 0x88 + x ;
            break ;
        case 3 :    byAddress = 0x98 + x ;
            break ;
        default :
            break ;
    }
    v_Lcd12864SendCmd_f( byAddress ) ;
}
void v_Lcd12864PutString_f( unsigned char x, unsigned char y, unsigned char *pData )
{
    v_Lcd12864SetAddress_f( x, y ) ;
    while( *pData != '\0' )
    {
        v_Lcd12864SendData_f( *pData++ ) ;
    }
}
void main( void )
{
    v_Lcd12864Init_f() ;  
    v_Lcd12864PutString_f( 0,0, "  东流电子") ;
    v_Lcd12864PutString_f( 2,1, "开发板商城") ;
    v_Lcd12864PutString_f( 0,2, "*LCD12864ST7920*") ;
    v_Lcd12864PutString_f( 0,3, "★○◇◆※☆■△") ;
    while( 1 ) ;
}