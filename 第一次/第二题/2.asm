STACKA  SEGMENT STACK

STACKA  ENDS
DATA    SEGMENT
        interest    dw  0
        principal   dw  10000
        total       dw  10000
        hundred     db  100
        rate        db  7
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,    DS:DATA ,    SS:STACKA
START:  MOV     AX,DATA
        MOV     DS,AX
        MOV     AX,STACKA
        MOV     SS,AX
        MOV     cx,18
yht:    MOV     AX,total
        DIV     hundred
        MUL     rate
        MOV  interest,AX    ;至此计算完利�
        ADD     AX,total
        MOV     total,AX
        LOOP    yht
        MOV     AH,4CH
        int     21H
CODE    ENDS
        END     START

