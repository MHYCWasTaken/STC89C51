/******************************************************************
/*                                                                *
/* OK300C单片机开发系统演示程序 			          *
/*                                                                *
/* 版本： V2.0 (2008/6/20)                                       *
/* 作者： 高山流水 (Email: aoke999@sohu.com)                      *
/* 网站： www.aokemcu.cn(奥科电子)                                *
/*                                                                *
/*                                                                *
/******************************************************************/


        IRCOM  EQU  20H       ;20H-23H IR使用
        X     EQU  26H        ;LCD 地址变量

        IRIN   EQU  P3.2
        BEEP   EQU  P3.7
        RELAY  EQU  P3.6

        RS	EQU	P2.0
        RW	EQU	P2.1
        EN 	EQU	P2.2
        PSB	EQU	P2.3
        RST	EQU	P2.5
;------------------------------------------------
         ORG 0000H
         JMP  MAIN

         ORG 0003H         ;外部中断INT0入口地址
         JMP  IR_IN        ;中断服务程序
;------------------------------------------------
MAIN:
          MOV   SP,#40H
          MOV   A,#00H
          MOV   R0,#20H
LOOP0:    MOV   @R0,A          ;20H-26H清零
          INC   R0
          CJNE  R0,#27H,LOOP0
          MOV  IE,#81H       ;允许总中断中断,使能 INT0 外部中断
          MOV  TCON,#01H     ;触发方式为脉冲负边沿触发
          SETB  IRIN
          SETB	RST
	  NOP
	  SETB	PSB            ;8位数据，并口

          CALL  SET_LCD        ;初始化TS12864A-3
          CALL  MENU1
          CALL  MENU2
          CALL  MENU3
          CALL  MENU4
LOOP1:
          MOV  A,22H
          CJNE  A,#1DH,LOOP2    ;UP键按下
          CLR   RELAY           ;继电器吸合
LOOP2:    CJNE  A,#12H,LOOP3    ;DOWN键按下
          SETB  RELAY           ;继电器关闭
LOOP3:    JMP   LOOP1

;=============================================
;  LCD 初始化设置
;=============================================-
SET_LCD:
          CLR  EN
          MOV  A,#34H     ;34H--扩充指令操作
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#30H     ;30H--基本指令操作
          CALL  WCOM
          CALL  DELAY1

          MOV  A,#0CH     ;开显示，关光标，
          CALL  WCOM
          CALL  DELAY1
          MOV  A,#01H     ;清除 LCM 显示屏
          CALL  WCOM
          CALL  DELAY1
          RET
;===================================================
;在 LCM 各行显示信息字符
;===================================================
LCD_SHOW:

          CJNE  A,#1,LINE2  ;判断是否为第一行
  LINE1:  MOV  A,#80H       ;设置 LCD 的第一行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#80H       ;设置 LCD 的第一行地址
          CALL  WCOM        ;写入命令
          JMP  FILL

  LINE2:  CJNE  A,#2,LINE3  ;判断是否为第三行
          MOV  A,#090H      ;设置 LCD 的第三行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#090H      ;设置 LCD 的第三行地址
          CALL  WCOM
          JMP  FILL

  LINE3:  CJNE  A,#3,LINE4  ;判断是否为第三行
          MOV  A,#088H      ;设置 LCD 的第三行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#088H      ;设置 LCD 的第三行地址
          CALL  WCOM
          JMP  FILL

  LINE4:  CJNE  A,#4,LINE5  ;判断是否为第三行
          MOV  A,#098H      ;设置 LCD 的第三行地址
          CALL  WCOM        ;写入命令
          CALL  CLR_LINE    ;清除该行字符数据
          MOV  A,#098H      ;设置 LCD 的第三行地址
          CALL  WCOM

  FILL:   CLR  A            ;填入字符
          MOVC  A,@A+DPTR   ;由消息区取出字符
          CJNE  A,#0,LC1    ;判断是否为结束码
  LINE5:  RET
  LC1:    CALL  WDATA       ;写入数据
          INC  DPTR         ;指针加1
          JMP  FILL         ;继续填入字符
          RET
;=================================================
;清除该行 LCM 的字符
;=================================================
CLR_LINE:
          MOV  R0,#16      ;
   CL1:   MOV  A,#' '
          CALL  WDATA
          DJNZ  R0,CL1
          RET
;==================================================
;LCM 显示工作菜单信息
;==================================================
MENU1:
         MOV   DPTR,#MENU1A
         MOV   A,#1         ;在第一行显示信息
         CALL  LCD_SHOW
         RET
MENU1A:  DB  "红外遥控码读取器",0

MENU2:
         MOV   DPTR,#MENU2A
         MOV   A,#2         ;在第二行显示信息
         CALL  LCD_SHOW
         RET
MENU2A:  DB  " www.aokemcu.cn ",0

MENU3:
         MOV   DPTR,#MENU3A
         MOV   A,#3         ;在第三行显示信息
         CALL  LCD_SHOW
         RET
MENU3A:  DB  "键值编码：- - H",0

MENU4:
         MOV   DPTR,#MENU4A
         MOV   A,#4         ;在第四行显示信息
         CALL  LCD_SHOW
         RET
MENU4A:  DB  "键值反码：- - H",0

;==============================================
; 写指令使能子程序
;RS=L,RW=L,D0-D7=指令码，E=高脉冲
;==============================================
WCOM:
          MOV  P0,A
          CLR RS
          CLR RW
          SETB EN
          CALL  DELAY0
          CLR EN
          RET
;=============================================
;写数据使能子程序
;RS=H,RW=L,D0-D7=数据，E=高脉冲
;=============================================
WDATA:
          MOV   P0,A
          SETB  RS
          CLR   RW
          SETB  EN
          CALL  DELAY0
          CLR   EN
          RET

DELAY0:   MOV  R7,#250      ;延时500微秒
          DJNZ  R7,$
          RET
;===============================================
;在 LCM 第三行、第四行显示字符
;A=ASC DATA, B=LINE X POS
;===============================================
LCDP3:                    ;在LCD的第三行显示字符
         PUSH  ACC        ;
         MOV  A,B         ;设置显示地址
         ADD  A,#088H     ;设置LCD的第三行地址
         CALL  WCOM       ;写入命令
         POP  ACC         ;由堆栈取出A
         CALL  WDATA      ;写入数据
         RET

LCDP4:                    ;在LCD的第四行显示字符
         PUSH  ACC        ;
         MOV  A,B         ;设置显示地址
         ADD  A,#098H     ;设置LCD的第四行地址
         CALL  WCOM       ;写入命令
         POP  ACC         ;由堆栈取出A
         CALL  WDATA      ;写入数据
         RET
;=================================================
; IR 译码子程序
;中断服务程序
;=================================================
IR_IN:
          CLR EA         ;暂时关闭CPU的所有中断请求
          PUSH  ACC
          PUSH  PSW
          SETB  PSW.3     ;选择工作寄存器组1
          CLR   PSW.4

          MOV   R0,#IRCOM
          MOV  R1,#04H
I1:       JNB  IRIN,I2       ;等待 IR 信号出现
          LCALL CONV
          DJNZ  R1,I1
          JMP  IR_OUT
 I2:      MOV  R4,#20
 I20:     CALL  DEL
          DJNZ  R4,I20
          JB  IRIN,I1        ;确认IR信号出现
 I21:     JB  IRIN,I3        ;等 IR 变为高电平
          CALL  DEL
          JMP  I21
 I3:      MOV  R3,#0         ;8位数清为0
 LL:      JNB  IRIN,I4       ;等 IR 变为低电平
          CALL  DEL
          JMP  LL
 I4:      JB  IRIN,I5        ;等 IR 变为高电平
          CALL  DEL
          JMP  I4
 I5:      MOV  R2,#0         ;0.14ms 计数
 L1:      CALL  DEL
          JB  IRIN, N1       ;等 IR 变为高电平
                             ;IR=0，检查R2中的计数值
          MOV  A,#8
          CLR  C
          SUBB  A,R2         ;判断高低位
                             ;IF C=0  BIT=0
          MOV  A,@R0
          RRC  A
          MOV  @R0,A         ;处理完一位
          INC  R3
          CJNE  R3,#8,LL     ;需处理完8位
          MOV  R3,#0
          INC  R0
          CJNE  R0,#24H,LL   ;收集到4字节了
          JMP   IR_SHOW
 N1:      INC  R2
          CJNE  R2,#30,L1    ;0.14ms 计数过长则时间到自动离开

IR_OUT:   POP  PSW
          POP  ACC
          SETB  EA
          RETI

;------------------------------------------------------------------
IR_SHOW:
          MOV A,22H
          CPL A                ;将22H取反后和23H比较
          CJNE A,23H,IR_SHOW1  ;如果不等表示接收数据发生错误,放弃。
          CALL   CONV
          CALL  BEEP_BL        ;蜂鸣器鸣响表示解码成功
IR_SHOW1:
          JMP  IR_OUT
;===============================================
;编码转换为 ASCII 码并显示
;===============================================
CONV:
          MOV   X,#5        ;设置显示起始位置
          MOV   A,22H
          ANL   A,#0F0H      ;取出高四位二进制数
          SWAP  A            ;高四位与低四位互换
          PUSH  ACC          ;压入堆栈
          CLR   C            ;C=0
          SUBB  A,#0AH       ;减10
          POP   ACC          ;弹出堆栈
          JC    ASCII0       ;该数小于10，转ASCII0
          ADD   A,#07H       ;大于10的数加上37H
ASCII0:   ADD   A,#30H       ;小于10的数加上30H
          MOV   B,X
          CALL  LCDP3

          MOV   A,22H
          ANL   A,#0FH        ;取出低四位二进制数
          PUSH  ACC
          CLR   C
          SUBB  A,#0AH        ;减10
          POP   ACC
          JC    ASCII1        ;该数小于10，转ASCII0
          ADD   A,#07H        ;大于10的数加上37H
ASCII1:   ADD   A,#30H        ;小于10的数加上30H
          INC   X
          MOV   B,X
          CALL  LCDP3

          MOV   X,#5         ;设置显示起始位置
          MOV   A,23H        ;反码
          ANL   A,#0F0H      ;取出高四位二进制数
          SWAP  A            ;高四位与低四位互换
          PUSH  ACC          ;压入堆栈
          CLR   C            ;C=0
          SUBB  A,#0AH       ;减10
          POP   ACC          ;弹出堆栈
          JC    ASCII2       ;该数小于10，转ASCII0
          ADD   A,#07H       ;大于10的数加上37H
ASCII2:   ADD   A,#30H       ;小于10的数加上30H
          MOV   B,X
          CALL  LCDP4

          MOV   A,23H
          ANL   A,#0FH        ;取出低四位二进制数
          PUSH  ACC
          CLR   C
          SUBB  A,#0AH        ;减10
          POP   ACC
          JC    ASCII3        ;该数小于10，转ASCII0
          ADD   A,#07H        ;大于10的数加上37H
ASCII3:   ADD   A,#30H        ;小于10的数加上30H
          INC   X
          MOV   B,X
          CALL  LCDP4
          RET
;===================================================
;蜂鸣器响一声子程序
;===================================================
BEEP_BL:
         MOV  R6,#100
  BL1:   CALL  DEX1
         CPL  BEEP
         DJNZ  R6,BL1
         MOV  R5,#25
         CALL  DELAY
         RET
 DEX1:   MOV  R7,#180
 DEX2:   NOP
         DJNZ  R7,DEX2
         RET
 DELAY:                    ;延时R5×10MS
         MOV  R6,#50
  D1:    MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,D1
         DJNZ  R5,DELAY
         RET
;===============================================
; DELAY  R5*0.14MS
DEL:
          MOV  R5,#1       ;IR解码使用
DEL0:     MOV  R6,#2
DEL1:     MOV  R7,#32
DEL2:     DJNZ  R7,DEL2
          DJNZ  R6,DEL1
          DJNZ  R5,DEL0
          RET

DELAY1:                    ;延时5MS
         MOV  R6,#25
  DL2:   MOV  R7,#100
         DJNZ  R7,$
         DJNZ  R6,DL2
         RET
;================================================
        END
