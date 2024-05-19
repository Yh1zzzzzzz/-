DATA  SEGMENT
BUFFER DB 3,?,2 DUP(0)
SHOW  DB  "Please enter 2  word(number  +  letter)",0ah,'$' 
ER   DB  'Enter is wrong :fistly enter number lastly enter letter,please enter again',0ah,'$'  
rig   db  'right!','$'
DATA  ENDS
CODE  SEGMENT
    ASSUME  CS:CODE, DS:DATA, ES:DATA
START:  
    MOV  AX, DATA
    MOV  DS, AX
    MOV  ES, AX
    LEA  DX,SHOW
    MOV  AH,09H 
    int  21H  
ente:
    LEA  DX,BUFFER
    MOV  AH,0AH
    INT  21H 
    LEA  SI,BUFFER
    INC  SI
    INC  SI 
    mov al,[si]
    CMP  al,'0'
    JB   yht
    cmp  al,'9'
    ja   yht
    inc  si 
    mov  al,[si]    
    CMP  al,'A'
    JB   yht
    cmp  al,'Z'
    JBE  righ
    cmp  al,'a'
    JB    yht
    cmp  al,'z'
    ja    yht
    jz   yht 
    jmp  righ   
yht : 
    lea dx,er
    mov ah,09h
    int 21h 
    jmp ente     
righ :  
    lea dx,rig
    mov ah,09h
    int 21h 
        

PRO_END:  
    MOV  AH, 4CH
    INT  21H

CODE  ENDS
    END  START