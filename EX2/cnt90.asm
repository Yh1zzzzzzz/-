.486
DATA	SEGMENT
NUM   DB  0      
AVG   DB  0    
TOL   DW  0
SCR   DB  20 DUP(78), 92, 98, 38 DUP(65), 10 DUP(90), 58, 84, 28 DUP(82)
INFO  DB  'Grade(>=90): ', '$'
INFO_1 DB ' Average Grade is','$'
INFO_YHT DB 'PRODUCED BY MR.YANGHAOTIAN =^_^=','$'
NINE  DB  90
HUNDRED DB 100
DATA	ENDS
CODE	SEGMENT
    ASSUME  CS:CODE,DS:DATA,ES:DATA
START:	MOV  AX,DATA
	MOV  DS,AX	   
	MOV  ES,AX 
      
;***********************************************************************
       ;编写程序, 统计SCR数组中>=90的个数, 结果存入NUM变量中.
    ;计算平均成绩，结果存入AVG变量中.
    MOV CX,100
	LEA SI,SCR
    AND AX,0
YHT:MOV BL,90
	CMP [SI],BL
	JAE MORE
LESS:	MOV Al,[SI]
	ADD TOL,Ax
	INC SI
	LOOP YHT
	JMP  ENDCOUNT
MORE: ADD NUM,1
	  MOV Al,[SI]
	  ADD TOL,Ax
	  INC SI
	  LOOP YHT 
	  JMP ENDCOUNT
ENDCOUNT:
	MOV AX,TOL
	DIV HUNDRED
	MOV AVG,AL

;***********************************************************************
        LEA  DX, INFO  
        CALL  DSPSTR    ;显示提示信息
        MOVZX  AX, NUM
        CALL  DSPDEC    ;按十进制显示AX数值
;************* 编写程序, 显示平均成绩 ***************************
		LEA  DX, INFO_1
		CALL DSPSTR
		MOVZX Ax,AVG
		CALL DSPDEC
		mov dl,10
		mov ah,02h
		int 21h
		LEA  DX,INFO_YHT
		CALL DSPSTR


	
PRO_END:
	MOV  AH,4CH
	INT  21H 	;返回DOS，主程序到此结束

;***************************************************************
;  你不必了解以下函数是如何实现得
;  只需要阅读函数的说明了解函数功能，了解如何调用它！
;***************************************************************
;函数：DSPDEC
;功能： 按十进制显示AX寄存器的内容，没有回车换行。
;参数：AX -- 待显示数据
;返回值：无
;***************************************************************
DSPDEC    PROC
	PUSH	AX
	PUSH	BX
	PUSH	DX
	PUSHF
	
	CMP 	AX, 9
	JBE	DSP
	;递归算法，显示AX的值
	MOV	DX, 0
	MOV	BX, 10
	DIV	BX
	PUSH	DX
	CALL	DSPDEC
	POP	DX
	ADD	DL, 30H
	MOV	AH, 02H
	INT	21H
	JMP	DSPDEC_END

DSP:	
	ADD	AL, 30H
	MOV	DL, AL
	MOV	AH, 02H
	INT	21H

DSPDEC_END:
	POPF
	POP	DX
	POP	BX
	POP	AX	
	RET
DSPDEC  ENDP

;**********************************************************************
;函数： DSPSTR
;功能： 显示自定义的字符串，定义的字符串必须以'$'结束，但不会显示$。
;参数： DX -- 字符串首地址
;返回值：无
;***********************************************************************
DSPSTR  PROC
	PUSH	AX

	MOV	AH, 09H
	INT	21H

	POP	AX
	RET
DSPSTR  ENDP
CODE	ENDS
	END	START
