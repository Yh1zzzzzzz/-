
DATA  SEGMENT
X    DB  2,12,13,14,15,16,17,1,1,10,1,2
Y     DB  1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,0
abc   db 0AH,'The number of characters in common is','$'
they  db  0AH,'they are:' ,'$'       
findone db  0AH,'FIND ONE COMMON CHAR:'  ,'$'
amount dw 0
finds  db 12 DUP(0)
DATA  ENDS
CODE  SEGMENT
    ASSUME  CS:CODE, DS:DATA, ES:DATA
START:  
    MOV  AX, DATA
    MOV  DS, AX
    MOV  ES, AX
    LEA  si,Y
    LEA  DI,X
    MOV  CX,20
    MOV  DX,12;循环调用12次
START_CMP:

    MOV  AL,[DI]
    MOV  CX,20
    CALL strchar
    ;---------------------------下面是调用完函数后的数据处理----------------------------------------------
    CMP  AH,0FFH
    JNZ  NOEQU
    ADD  amount,1


        
        
NOEQU:   inc di
        DEC DX
       CMP DX,0
       JNZ START_CMP


PRINT:
    lea dx,abc
    mov ah,09h
    int 21h 
    mov ax,amount
    call dspeax





    


PRO_END:  
    MOV  AH, 4CH
    INT  21H
;------------------------------------------------------------------
;@function : strchar
;@para     : SI—数据首地址，CX—数据长度，AL—待查找数据
;@brief:     
;@return:   AH— 0表示没有，0FFH表示有。

strchar PROC NEAR
        MOV BX,si
        PUSH si
STRAT_A:CMP AL,[BX]
        JZ  FIND_IN
        INC BX
        LOOP STRAT_A
        MOV  AH,0
        JMP  END_func
FIND_IN:
        PUSH DX 
        LEA  DX,FINDONE 
        MOV  AH,09H
        INT  21H  
        POP  DX
        MOV  AL,[DI]
        MOV  AH,0
        CALL DSPEAX
        
        

         MOV  AH,0FFH
        JMP  END_func
END_func:
POP si
        RET
strchar ENDP
;---------------------------------------   
DSPEAX    PROC
	PUSH	AX
	PUSH	BX
	PUSH	DX
	PUSHF
	
	CMP 	AX, 9
	JBE	DSPEAX_DSP
	;递归算法，显示EAX的值
	MOV	DX, 0
	MOV	BX, 10
	DIV	BX
	PUSH	DX
	CALL	DSPEAX
	POP	DX
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
	POP	DX
	POP	BX
	POP	AX	
	RET
DSPEAX  ENDP

;--------------------------------------------------------------
CODE  ENDS
    END  START