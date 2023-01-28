#ifndef __TFT_H__
#define __TFT_H__

#define X_MAX 240
#define Y_MAX 400

/*------------------------------------------------
             		��������
------------------------------------------------*/
void ClearScreen(unsigned int bColor);

/*------------------------------------------------
             д8x16�ַ�����
------------------------------------------------*/

void LCD_PutChar8x16(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);

/*------------------------------------------------
              д�ַ�������
------------------------------------------------*/
void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);

/*------------------------------------------------
             д16x16���ֺ���
------------------------------------------------*/


void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor);
/*------------------------------------------------
                д�ַ�������
------------------------------------------------*/
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) ;
/*------------------------------------------------
              ָ��������ʾָ��RGB��ɫ
------------------------------------------------*/
void Show_RGB (unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1,unsigned int Color);

/*------------------------------------------------
                ��ʾ����
------------------------------------------------*/
void show_colour_bar (void);

/*------------------------------------------------
              д16λ���ݺ���
------------------------------------------------*/
void  Write_Data_U16(unsigned int y);
/*------------------------------------------------
                дָ���
------------------------------------------------*/
void LCD_Write_Command(unsigned char u);
/*------------------------------------------------
                 д���ݺ���
------------------------------------------------*/
void LCD_Write_Data(unsigned char u);
WriteCom(a,b);

WriteData(a,b);
/*------------------------------------------------
                ��ʱ����
------------------------------------------------*/
void delayms(unsigned int count);
/*------------------------------------------------
             	Һ����ʼ������
------------------------------------------------*/

void TFT_Initial(void);
/*------------------------------------------------
��������:LCD_DefineDispWindow
��    ��:������ʾ����
��    ��:x0:  ������X�����н�С��
	 x1:  ������X�����нϴ���
	 y0:  ������Y�����н�С��
	 y1:  ������Y�����нϴ��� 
�� �� ֵ:��
------------------------------------------------*/
static void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
/*----------------------------------------------------------------
                            ����
���������x��y ��Ҫ��������
          color �����ɫ
----------------------------------------------------------------*/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color);

/*----------------------------------------------------------------
                           ����Ļ�ϻ���
���������  ��ʼ����X0��Y0����ֹ����X1��Y1
			color ����ɫ
----------------------------------------------------------------*/
void Line(	unsigned int X0,
			unsigned int Y0,
			unsigned int X1,
			unsigned int Y1,
			unsigned int color);
/*---------------------------------------------------------------------------
                            ���ƾ��ο�
������������ε���ʼλ��left,top 
 		  ���εĽ���λ��right,bottom 
 		 ���ο����ɫcolor

-----------------------------------------------------------------------------*/
void Rectangle( unsigned int left,
				unsigned int top,
				unsigned int right,
				unsigned int bottom,
				unsigned int color);
				
#endif
