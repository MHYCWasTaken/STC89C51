/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : LED二进制加法显示实验
* 实验说明       : LED灯做二进制加1显示
* 连接方式       : 见接线图
* 注    意		 : 
*******************************************************************************/

	ORG 0000H	 ;程序从000H地址开始运行
	AJMP MAIN	 ;跳转到 MAIN程序

	ORG 030H	  ;MAIN程序从030H开始运行
MAIN:	
    MOV P0,#0FEH
	ACALL DEL	   ;调用延时子程序
	MOV P0,#0FCH
	ACALL DEL	   ;调用延时子程序
	MOV P0,#0F8H
	ACALL DEL
	MOV P0,#0F0H
	ACALL DEL
	MOV P0,#0E0H
	ACALL DEL
	MOV P0,#0C0H
	ACALL DEL
	MOV P0,#080H
	ACALL DEL
	MOV P0,#000H
	ACALL DEL
	MOV P0,#0FFH
	AJMP MAIN	   ;跳转到MAIN程序

; 延时子程序
DEL:	MOV R5,#02H
DEL1:	MOV R6,#0F0H
DEL2:	MOV R7,#0F0H
DEL3:	DJNZ R7, DEL3
	DJNZ R6, DEL2
	DJNZ R5, DEL1
	RET
	END
