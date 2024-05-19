.486
DATA	SEGMENT
Interest   DW  7
Principal  DD  10000
Total      DD  0
Years      DW  0
final      DD  5000000
INFO_INPUTP    DB   'Input Principal(<=1000000): ', '$'
INFO_INPUTI    DB   0AH, 0DH, 'Input Interest(<=100): ', '$'
INFO_TOTAL     DB   0AH, 0DH, ' years later, you have ','$'
NEXTLINE	DB	0AH, 0DH, '$'
DATA	ENDS
CODE	SEGMENT
    ASSUME  CS:CODE,DS:DATA,ES:DATA
START:	MOV  AX,DATA
	MOV  DS,AX	   
	MOV  ES,AX 
    
     ;输入Principal
	LEA	DX, INFO_INPUTP
	CALL	DSPSTR		;显示输入提示信息
	CALL	INPUTNUM32	;要求用户输入数字，输入的数字存在AX中
     	MOV  Principal, EAX

    ;输入Interest
	LEA	DX, INFO_INPUTI
	CALL	DSPSTR		;显示输入提示信息
	CALL	INPUTNUM32	;要求用户输入数字，输入的数字存在EAX中
     MOV  Interest, AX

;***********************************************************************
	;添加代码计算收益, 提醒当总金额>=5000000时循环结束，事先不知道循环多少次。
	MOV ECX ,final
	MOV EAX, Principal
	MOV Total,EAX


begin:
    ADD Years,1
    MOV EAX,Total
	MUL Interest
	MOV EBX,100
	DIV EBX   ;计算新的利息,保存在EBX
	ADD Total,EAX
	CMP Total,ECX
	JNA begin





     

    
;***********************************************************************
    MOVZX  EAX, Years
    CALL   DSPEAX        
    LEA  DX, INFO_TOTAL
    CALL  DSPSTR        ;显示提示信息
    MOV   EAX, Total
    CALL  DSPEAX        ;显示总金额
PRO_END:
	MOV	AH,4CH
	INT	21H		;返回DOS，主程序到此结束


;***************************************************************
;  你不必了解以下函数是如何实现得
;  只需要阅读函数的说明了解函数功能，了解如何调用它！
;***************************************************************
; 函数：INPUTNUM32
; 功能：要求用户按十进制输入一个小于65535的数，以回车键结束输入。
;             用户输入的数据存放在EAX中，若用户直接输入回车，则EAX=0。
; 参数：无
; 返回值：EAX -- 用户输入的数据
; 例如若输入1234，则EAX=1234
;***************************************************************
INPUTNUM32	PROC
	PUSH	EBP
	PUSH	ESI
	PUSHF
	
	MOV	EAX, 0

	;输入一个数 --> BP
	MOV	EBP, 0
	MOV	ESI, 0
INPUTNUM1:
	MOV	AH, 01H
	INT	21H		;从键盘输入一个按键，AL中是按键的ASCII
	CMP	AL, 0DH
	JZ	INEXPR_EXIT
	SUB	AL, 30H
	MOVZX	ESI, AL
	MOV	EAX, 0AH
	MUL	EBP
	MOV	EBP, EAX
	ADD	EBP, ESI
	JMP	INPUTNUM1

INEXPR_EXIT:
	MOV	EAX, EBP

	POPF
	POP	ESI
	POP	EBP	
	RET
INPUTNUM32  ENDP

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

;************************************************************************
;函数： DSPSTR
;功能： 显示自定义的字符串，定义的字符串必须以'$'结束，但不会显示$。
;参数： DX -- 字符串首地址
;返回值：无
;************************************************************************
DSPSTR  PROC
	PUSH	AX

	MOV	AH, 09H
	INT	21H

	POP	AX
	RET
DSPSTR  ENDP

CODE	ENDS
	END	START
