.486
DATA  SEGMENT
dbuf    DB  20, 0, 20  DUP(0)
dstr     DB   'Input a number: $'
DATA  ENDS
CODE  SEGMENT
    ASSUME  CS:CODE, DS:DATA, ES:DATA
START:  
    MOV  AX, DATA
    MOV  DS, AX
    MOV  ES, AX

    ;display a character
    MOV  DL, 'A'   ;待显示字符
    MOV  AH, 02H  ;设置功能号
    INT  21H

    ;display a string
    LEA  DX, dstr
    MOV  AH, 09H
    INT  21H

    ;input a char
    MOV  AH, 01H
    INT   21H

    ;input a string
    LEA  DX, dbuf
    MOV  AH, 0AH
    INT  21H

PRO_END:  
    MOV  AH, 4CH
    INT  21H
CODE  ENDS
    END  START