/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : LED二进制加法显示实验
* 实验说明       : LED灯做二进制加1显示
* 连接方式       : 见接线图
* 注    意		 : 
*******************************************************************************/

     	ORG 	0000h	 ;程序从0000开始运行
     	AJMP	main
	    ORG	    0030h
main:	
		MOV		A  ,#000h
loop: 	
		MOV     P0,A	;全不亮
    	CALL    delay	;延时。
	    INC	    A	    ;每次加一。
	    JMP     loop

delay:
		MOV r5,#50	;延时。
d1: 
		MOV r6,#40
d2:	
		MOV r7,#248
	
		DJNZ r7,$
		DJNZ r6,d2
		DJNZ r5,d1
		RET
END
