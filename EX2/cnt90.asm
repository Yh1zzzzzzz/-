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
       ;��д����, ͳ��SCR������>=90�ĸ���, �������NUM������.
    ;����ƽ���ɼ����������AVG������.
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
        CALL  DSPSTR    ;��ʾ��ʾ��Ϣ
        MOVZX  AX, NUM
        CALL  DSPDEC    ;��ʮ������ʾAX��ֵ
;************* ��д����, ��ʾƽ���ɼ� ***************************
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
	INT  21H 	;����DOS�������򵽴˽���

;***************************************************************
;  �㲻���˽����º��������ʵ�ֵ�
;  ֻ��Ҫ�Ķ�������˵���˽⺯�����ܣ��˽���ε�������
;***************************************************************
;������DSPDEC
;���ܣ� ��ʮ������ʾAX�Ĵ��������ݣ�û�лس����С�
;������AX -- ����ʾ����
;����ֵ����
;***************************************************************
DSPDEC    PROC
	PUSH	AX
	PUSH	BX
	PUSH	DX
	PUSHF
	
	CMP 	AX, 9
	JBE	DSP
	;�ݹ��㷨����ʾAX��ֵ
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
;������ DSPSTR
;���ܣ� ��ʾ�Զ�����ַ�����������ַ���������'$'��������������ʾ$��
;������ DX -- �ַ����׵�ַ
;����ֵ����
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