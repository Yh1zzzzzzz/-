CODE SEGMENT USE16
    ASSUME CS:CODE,DS:CODE,ES:CODE
    ORG 2000H
START:  JMP BEG
BEG:    MOV    AX,CODE
        MOV    DS,AX
        MOV    ES,AX
        MOV    DX,303H  ;写计数器1
        MOV    AL,32H
        OUT    DX,AL
        MOV    DX,300H
        MOV    AX,47000
        OUT    DX,AL
        MOV    AL,AH    
        OUT    DX,AL    
         ;完成计数器一的初始化   
YHT:    NOP
        NOP
        JMP     YHT
        MOV AH,4CH
        INT  21H
CODE ENDS
END START 