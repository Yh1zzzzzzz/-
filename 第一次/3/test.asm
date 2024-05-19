.486
DATA  SEGMENT
    VAR_X  DB  0
    Y1     DW  12H
    Z      DD  0ABH
    W      DB  ?
    ARY    DB  0, 1, 5 DUP(2)
DATA  ENDS
CODE  SEGMENT
    ASSUME  CS:CODE, DS:DATA, ES:DATA
START:  
    MOV  AX, DATA
    MOV  DS, AX
    MOV  ES, AX

    MOV  AL, BX
    LEA  SI, Y1
    LEA  DI, Z
    LEA  BX, ARY
    MOV  AL, [SI]
    MOV  CX, [BX+1]

PRO_END:  
    MOV  AH, 4CH
    INT  21H
CODE  ENDS
    END  START