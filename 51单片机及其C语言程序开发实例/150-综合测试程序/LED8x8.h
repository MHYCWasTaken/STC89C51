/*-----------------------------------------------
  名称：双色点阵
  论坛：www.doflye.net
  编写：shifang
  修改：无
  内容：使用hc595驱动双色点阵  
------------------------------------------------*/
#ifndef __LED8X8_H__
#define __LED8X8_H__

void Send1Byte(unsigned char dat);
void Send2Byte(unsigned char dat1,unsigned char dat2);
void Out595(void);
void SendSeg(unsigned char dat);
void LED8x8Display(void);

#endif
