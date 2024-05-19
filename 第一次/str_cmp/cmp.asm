STACKA  SEGMENT STACK

STACKA  ENDS
DATA    SEGMENT
        A db    12
        B db    12
        C db    12
        k   db  ?
DATA    ENDS
CODE    SEGMENT
        ASSUME  CS:CODE,    DS:DATA ,    SS:STACKA
START:  MOV     AX,DATA
        MOV     DS,AX
        MOV     AX,STACKA
        MOV     SS,AX
        MOV     AL,A
        cmp     AL,B
        JZ      AEQUB        ;A=B
        JNZ     ANEQUB        ;A!=B
AEQUB: MOV      BL,B
        cmp     BL,C
        JZ      allEQU;A=B=C
        JnZ      twoEQU;2QUE
ANEQUB: MOV     AL,A
        cmp     AL,C
        JZ      twoEQU
        JNZ     CMPBC
CMPBC:  MOV     BL,B
        CMP     BL,C
        JZ      twoEQU
        JNZ     noneEQU
allEQU:   MOV     K,2
        JMP     PROEND
twoEQU:   MOV     K,1
        JMP     PROEND
noneEQU:   MOV     K,0
        JMP     PROEND
PROEND:        MOV     AH,4CH
        int     21H
CODE    ENDS
        END     START

