/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : 跑马灯实验
* 实验说明       : LED灯做右移跑马灯
* 连接方式       : 见接线图
* 注    意		 : 
*******************************************************************************/

	ORG 0000H	  ;程序0000起始地址
	AJMP MAIN	  ;跳转到MAIN

	ORG 030H      ;MAIN 程序起始地址
MAIN:	
	MOV P0,#0FEH
	ACALL DEL	    ;调用子程序
	MOV P0,#0FDH	;将立即数传给P2
	ACALL DEL
	MOV P0,#0FBH
	ACALL DEL
	MOV P0,#0F7H
	ACALL DEL
	MOV P0,#0EFH
	ACALL DEL
	MOV P0,#0DFH
	ACALL DEL
	MOV P0,#0BFH
	ACALL DEL
	MOV P0,#07FH
	ACALL DEL
	MOV P0,#0FFH
	AJMP MAIN 

;延时子程序
DEL:	MOV R5,#04H
DEL1:	MOV R6,#0FFH
DEL2:	MOV R7,#0FFH
DEL3:	DJNZ R7, DEL3
	DJNZ R6, DEL2
	DJNZ R5, DEL1
	RET
	END
