;************************************************
;8031并口连接间接访问方式程序
;        ST7920C   JCM12864M
;  CPU  ST7920C   内置英文字符库及汉字字符库
;2008-01-08该程序在液晶显示模块JCM12864M,JCM89S51上进行调试运行OK
;4行都显示:金聚宝电子科技
;*************************************************************
      RS  EQU P3.0
      RW  EQU P3.1
      E   EQU P3.3
      PSB EQU P3.4
      RST EQU P3.6
      LCD_X  EQU 30H
      LCD_Y  EQU 31H
      COUNT  EQU 32H
      COUNT1 EQU 33H
      COUNT2 EQU 34H
      COUNT3 EQU 35H
      LCD_DATA  EQU 36H
      LCD_DATA1 EQU 37H
      LCD_DATA2 EQU 38H
      STORE     EQU 39H
;********************************************************
         ORG 0000H
         LJMP MAIN
         ORG 0003H
         LJMP PAUSE
   MAIN:
        MOV SP,#60H
        MOV IE,#81H
        MOV IP,#01H
        MOV TCON,#00H
        CLR RST
        LCALL DELAY4        ;T=4084US=4MS
        SETB RST
        NOP
        SETB PSB
    INT:
         MOV A,#34H
         LCALL SEND_I
         MOV A,#30H
         LCALL SEND_I
         MOV A,#01H
         LCALL SEND_I
         MOV A,#06H
         LCALL SEND_I
         MOV A,#0CH
         LCALL SEND_I
DISPLAY:
         MOV A,#01H
         LCALL SEND_I
         MOV LCD_DATA1,#0CCH
         MOV LCD_DATA2,#0CCH
         LCALL DISP
         LCALL DELAY3
         LCALL DELAY3
         LCALL DELAY3
         LCALL DELAY3
DISPLAY1:
        LCALL CLEAR_P
DISPLAY1A:
        MOV DPTR,#TAB1A            ;第1行
        MOV COUNT,#10H
        MOV A,#80H
        LCALL SEND_I
        LCALL QUSHU
DISPLAY1B:
        MOV DPTR,#TAB1B            ;第2行
        MOV COUNT,#10H
        MOV A,#90H
        LCALL SEND_I
        LCALL QUSHU
DISPLAY1C:
        MOV DPTR,#TAB1C            ;第3行
        MOV COUNT,#10H
        MOV A,#88H
        LCALL SEND_I
        LCALL QUSHU
DISPLAY1D:
        MOV DPTR,#TAB1D            ;第4行
        MOV COUNT,#10H
        MOV A,#98H
        LCALL SEND_I
        LCALL QUSHU

        LCALL DELAY3
        LCALL DELAY3
        LCALL DELAY3
        LCALL DELAY3
        LCALL DELAY3

        LJMP DISPLAY
;****************************************************
    DISP:
         MOV COUNT3,#02H
         MOV LCD_X,#80H
    DISP1:
         MOV LCD_Y,#80H
         CLR F0
         MOV COUNT2,#20H
    DISP2:
         MOV COUNT1,#10H
         LCALL WR_ZB
    DISP3:
         JB F0,DISP32
         MOV LCD_DATA,LCD_DATA1
         AJMP DISP31
   DISP32:
         MOV LCD_DATA,LCD_DATA2
   DISP31:
         MOV A,LCD_DATA
         LCALL SEND_D
         DJNZ COUNT1,DISP31
         INC LCD_Y
         CPL F0
         DJNZ COUNT2,DISP2
         MOV LCD_X,#88H
         DJNZ COUNT3,DISP1
         MOV A,#36H
         LCALL SEND_I
         MOV A,#30H
         LCALL SEND_I
         RET
   WR_ZB:
         MOV A,#34H
         LCALL SEND_I
         MOV A,LCD_Y
         LCALL SEND_I
         MOV A,LCD_X
         LCALL SEND_I
         MOV A,#30H
         LCALL SEND_I
         RET
;***********************
CLEAR_P:
        MOV A,#01H
        LCALL SEND_I
        MOV A,#34H
        LCALL SEND_I
        MOV A,#30H
        LCALL SEND_I
        RET
;*************************
QUSHU:
        CLR A
        MOVC A,@A+DPTR
        LCALL SEND_D
        INC DPTR
        LCALL DELAY4
        DJNZ COUNT,QUSHU
        RET
;*************************
SEND_D:
        LCALL CHK_BUSY
        SETB RS
        CLR RW
        MOV P1,A
        SETB E
        NOP
        NOP
        CLR E
        RET
;**************************
SEND_I:
       LCALL CHK_BUSY
       CLR RS
       CLR RW
       MOV P1,A
       SETB E
       NOP
       NOP
       CLR E
       RET
;**************************
READ_D:
      LCALL CHK_BUSY
      SETB RS
      SETB RW
      SETB E
      NOP
      MOV A,P1
      CLR E
      MOV STORE,A
      RET
;*******************************
CHK_BUSY:
       MOV P1,#0FFH
       CLR RS
       SETB RW
       SETB E
       JB P1.7,$
       CLR E
       RET
;*******************************
DELAY3:
       MOV R5,#16H
DEL31: MOV R6,#100
DEL32: MOV R7,#0FFH
DEL33: DJNZ R7,DEL33
       DJNZ R6,DEL32
       DJNZ R5,DEL31
       RET
DELAY4:   ;延时80ms,
       MOV R6,#100
DEL41: MOV R7,#200
DEL42: DJNZ R7,DEL42
       DJNZ R6,DEL41
       RET
;********************************
 PAUSE:
      SETB P3.2
      SETB P3.2
      LCALL DELAY4
      MOV C,P3.2
      MOV C,P3.2
      JNC PAUSE
 PAUSE1:
      MOV C,P3.2
      MOV C,P3.2
      LCALL DELAY4
      JC PAUSE1
 PAUSE2:
      SETB P3.2
      SETB P3.2
      MOV C,P3.2
      MOV C,P3.2
      JNC PAUSE2
      NOP
      RETI
TAB1A:
DB 0BDH,0F5H,0B2H,0FDH,0B5H,0E7H,0D7H,0D3H,0B2H,0E2H,0CAH,0D4H,0B3H,0CCH,0D0H,0F2H ; 1	
TAB1B: 
DB 0BDH,0F5H,0B2H,0FDH,0B5H,0E7H,0D7H,0D3H,0B2H,0E2H,0CAH,0D4H,0B3H,0CCH,0D0H,0F2H ; 1
TAB1C: 
DB 0BDH,0F5H,0B2H,0FDH,0B5H,0E7H,0D7H,0D3H,0B2H,0E2H,0CAH,0D4H,0B3H,0CCH,0D0H,0F2H ; 1
TAB1D:  
DB 0BDH,0F5H,0B2H,0FDH,0B5H,0E7H,0D7H,0D3H,0B2H,0E2H,0CAH,0D4H,0B3H,0CCH,0D0H,0F2H ; 1
end
