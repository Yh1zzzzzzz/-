.486
DATA	SEGMENT
INFO_INCOME	DB    'Input your income: ', '$'
INFO_TAX      DB    0AH, 0DH, 'The tax is ','$'
INFO_RLINCOME DB    'Your income after tax is ', '$'
NEXTLINE      DB    0AH, 0DH, '$'
INFO_EXIT     DB    0AH, 0DH, 'Thank you for using TAX calculator!', '$'

DATA	ENDS
CODE	SEGMENT
    ASSUME  CS:CODE,DS:DATA,ES:DATA
START:	MOV  AX,DATA
	MOV  DS,AX	   
	MOV  ES,AX 
      
	LEA	DX, INFO_INCOME
	CALL	DSPSTR		;显示输入提示信息

	CALL	INPUTNUM	;要求用户输入数字，输入的数字存在AX中
;***********************************************************************
	;添加代码计算个人所得税，将个人所得税存入AX
	CMP		AX,5000
	JNA		TAXFREE
	JA		TAXCMP_ONE
	TAXCMP_ONE:CMP AX,8000
			JNA	TAX_LVONE
			JA	TAXCMP_TWO
	TAXCMP_TWO:CMP	AX,17000
			JNA	TAX_LVTWO
			JA	TAXCMP_THREE
	TAXCMP_THREE:CMP AX,30000
			JNA	TAX_LVTHREE
			JA	TAX_LVMAX
	TAXFREE:MOV AX,0
			JMP	countine		;收入小于五千
	TAX_LVONE:
			SUB ax,5000
			MOV BX,3
			MOV cx,100
			MUL BX
			DIV	cx	
			JMP countine				;[5000,8000]
	TAX_LVTWO:
			SUB ax,8000
			MOV BX,10
			MOV cx,100
			MUL BX
			DIV	cx
			ADD ax,90	
			JMP countine		;[8000,17000]
	TAX_LVTHREE:
			SUB ax,17000
			MOV BX,20
			MOV cx,100
			MUL BX
			DIV	cx
			ADD AX,990
			JMP countine			;[17000,30000]
	TAX_LVMAX:	MOV ax,3590
				JMP countine				;>30000



;***********************************************************************
countine:        LEA  DX, INFO_TAX
        CALL  DSPSTR        ;显示提示信息
        CALL  DSPDEC        ;显示个人所得税
        LEA  DX, INFO_EXIT  
        CALL  DSPSTR        ;显示退出信息
		
PRO_END:
	MOV	AH,4CH
	INT	21H		;返回DOS，主程序到此结束



;***************************************************************
;  你不必了解以下函数是如何实现得
;  只需要阅读函数的说明了解函数功能，了解如何调用它！
;***************************************************************
; 函数：INPUTNUM
; 功能：要求用户按十进制输入一个小于65535的数，以回车键结束输入。
;             用户输入的数据存放在AX中，若用户直接输入回车，则AX=0。
; 参数：无
; 返回值：AX -- 用户输入的数据
; 例如若输入1234，则AX=1234
;***************************************************************
INPUTNUM	PROC
	PUSH	BP
	PUSH	SI
	PUSHF
	
	MOV	AX, 0

	;输入一个数 --> BP
	MOV	BP, 0
	MOV	SI, 0
INPUTNUM1:
	MOV	AH, 01H
	INT	21H		;从键盘输入一个按键，AL中是按键的ASCII
	CMP	AL, 0DH
	JZ	INEXPR_EXIT
	SUB	AL, 30H
	MOV	AH, 0
	MOV	SI, AX
	MOV	AX, 0AH
	MUL	BP
	MOV	BP, AX
	ADD	BP, SI
	JMP	INPUTNUM1

INEXPR_EXIT:
	MOV	AX, BP

	POPF
	POP	SI
	POP	BP	
	RET
INPUTNUM	ENDP

;***************************************************************
;函数：DSPHEX
;功能： 按十六进制显示AX寄存器的内容，没有回车换行。
;参数：AX -- 待显示数据
;返回值：无
;***************************************************************
DSPHEX    PROC
	PUSH	AX
	PUSH	BX
	PUSH	DX
	PUSHF	


	MOV	BX, AX

	;显示最高位十六进制
	MOV	DX, BX
	SHR	DX, 12
	CMP	DL, 09H
	JA	DSPHEX_1_CHAR
	ADD	DL, 30H
	JMP	DSPHEX_1
DSPHEX_1_CHAR:
	ADD	DL, 37H	
DSPHEX_1:
	MOV	AH, 02H
	INT	21H

	MOV	DX, BX
	SHR	DX, 8
	AND	DL, 0FH
	CMP	DL, 09H
	JA	DSPHEX_2_CHAR
	ADD	DL, 30H
	JMP	DSPHEX_2
DSPHEX_2_CHAR:
	ADD	DL, 37H	
DSPHEX_2:
	MOV	AH, 02H
	INT	21H

	MOV	DX, BX
	SHR	DX, 4
	AND	DL, 0FH
	CMP	DL, 09H
	JA	DSPHEX_3_CHAR
	ADD	DL, 30H
	JMP	DSPHEX_3
DSPHEX_3_CHAR:
	ADD	DL, 37H	
DSPHEX_3:
	MOV	AH, 02H
	INT	21H

	MOV	DX, BX
	AND	DL, 0FH
	CMP	DL, 09H
	JA	DSPHEX_4_CHAR
	ADD	DL, 30H
	JMP	DSPHEX_4
DSPHEX_4_CHAR:
	ADD	DL, 37H	
DSPHEX_4:
	MOV	AH, 02H
	INT	21H

	POPF
	POP	DX
	POP	BX
	POP	AX
	RET
DSPHEX    ENDP

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

;***************************************************************
;函数：DSPEAX
;功能： 按十进制显示EAX寄存器的内容，没有回车换行。
;参数：EAX -- 待显示数据
;返回值：无
;***************************************************************
DSPEAX    PROC
	PUSH	EAX
	PUSH	EBX
	PUSH	EDX
	PUSHF
	
	CMP 	EAX, 9
	JBE	DSPEAX_DSP
	;递归算法，显示EAX的值
	MOV	EDX, 0
	MOV	EBX, 10
	DIV	EBX
	PUSH	EDX
	CALL	DSPEAX
	POP	EDX
	ADD	DL, 30H
	MOV	AH, 02H
	INT	21H
	JMP	DSPEAX__END

DSPEAX_DSP:	
	ADD	AL, 30H
	MOV	DL, AL
	MOV	AH, 02H
	INT	21H

DSPEAX__END:
	POPF
	POP	EDX
	POP	EBX
	POP	EAX	
	RET
DSPEAX  ENDP

;***********************************************************************************************
;函数： DSPSTR
;功能： 显示自定义的字符串，定义的字符串必须以'$'结束，但不会显示$。
;参数： DX -- 字符串首地址
;返回值：无
;***********************************************************************************************
DSPSTR  PROC
	PUSH	AX

	MOV	AH, 09H
	INT	21H

	POP	AX
	RET
DSPSTR  ENDP


;***********************************************************************************************
;函数：DSPFLAG
;功能： 显示6个状态标志的值：CF、ZF、OF、SF、AF、PF，显示后光标回到下一行首列(即有回车换行)
;参数：无
;返回值：无
;************************************************************************************************
DSPFLAG    PROC
	PUSH	AX
	PUSH	BX
	PUSH	DX
	PUSHF

	;显示'CF='
	MOV 	DL, 'C'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	;显示CF标志的值
	JC	DSPCF1
DSPCF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP	DSPZF
DSPCF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H
;************显示ZF标志****************
DSPZF:
	MOV 	DL, '	'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'Z'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	JZ	DSPZF1
DSPZF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP 	DSPOF
DSPZF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H
;************显示OF标志****************
DSPOF:
	MOV 	DL, '	'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'O'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	JO	DSPOF1
DSPOF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP 	DSPSF
DSPOF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H
;************显示SF标志****************
DSPSF:
	MOV 	DL, '	'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'S'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	JS	DSPSF1	
DSPSF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP 	DSPAF

DSPSF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H
;************显示AF标志****************
DSPAF:
	MOV 	DL, '	'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'A'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	
	PUSHF
	POP	BX
	BT	BX, 4
	JC	DSPAF1
DSPAF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP 	DSPPF
DSPAF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H
;************显示PF标志****************
DSPPF:
	MOV 	DL, '	'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'P'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, 'F'
	MOV	AH, 02H
	INT	21H
	MOV 	DL, '='
	MOV	AH, 02H
	INT	21H
	JP	DSPPF1
DSPPF0:
	MOV 	DL, '0'
	MOV	AH, 02H
	INT	21H
	JMP 	DSPFLAG_END

DSPPF1:
	MOV 	DL, '1'
	MOV	AH, 02H
	INT	21H

DSPFLAG_END:
	MOV 	DL, 0AH
	MOV	AH, 02H
	INT	21H	
	MOV 	DL, 0DH
	MOV	AH, 02H
	INT	21H

	POPF
	POP	DX
	POP	BX
	POP	AX
	RET
DSPFLAG    ENDP

CODE	ENDS
	END	START
