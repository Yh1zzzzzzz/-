;编程实现以下功能，\
;   S1   IN 11111100   当开关K1、K2都扳向后时小灯全灭；
;   S2   IN 11111101   只有开关K1扳向前时小灯为流水灯状态（8个发光二极管先从两边到中间，然后从中间到两边循环亮灯）；
;   S3   IN 11111110  只有开关K2扳向前时小灯为霹雳灯状态（8个发光二极管全亮、全灭，亮灭闪烁）；
;   S4   IN 11111111  开关K1、K2都扳向前时小灯全亮程序结束。
CODE SEGMENT USE16
    ASSUME CS:CODE,DS:CODE,ES:CODE
    ORG 2000H
START:  JMP BEG
;通过8255A芯片的B口读入开关状态，C口控制发光二极管。

;电路连接：PB7--K8  PB6--K7 ......PB0-K1
          ;PC7--D1     ..........PC0-D8
          ;K8-K3全部向前搬


;
BEG:    MOV    AX,CODE
        MOV    DS,AX
        MOV    ES,AX
        MOV    DX,303H  ;写8255
        MOV     AL,92H
        OUT     DX,AL
        ;写好了8255
AGN:    MOV    DX,301H
        IN     AL,DX
        CMP    AL,0FCH
        JZ      STATE1
        CMP     AL,0FDH
        JZ      STATE2
        CMP     AL,0FEH
        JZ      STATE3
        CMP     AL,0FFH
        JZ      STATE4

STATE1: 
        MOV     DX,302H
        MOV     AL,00H
        OUT     DX,AL 
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FCH
        jnz     AGN
        JMP     STATE1
;@@@@       S2
STATE2: 
        MOV     DX,302H
        MOV     AL,7EH
        OUT     DX,AL
        CALL    delay
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FDH
        jnz     AGN


        MOV     DX,302H
        MOV     AL,0BDH
        OUT     DX,AL
        CALL    delay
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FDH
        jnz     AGN

        MOV     DX,302H
        MOV     AL,0DBH
        OUT     DX,AL
        CALL    delay
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FDH
        jnz     AGN

        MOV     DX,302H
        MOV     AL,0E7H
        OUT     DX,AL
        CALL    delay
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FDH
        jnz     AGN
        JMP     STATE2

;@@@        S3
AGN_TEMP:
        JMP    AGN
STATE3: 
        MOV     DX,302H
        MOV     AL,00H
        OUT     DX,AL
        CALL    delay
        MOV     DX,302H
        MOV     AL,0FFH
        OUT     DX,AL
        CALL    delay
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FEH
        jnz     AGN_TEMP
        JMP     STATE3
;     中转

;@@     S4
STATE4: 
        MOV     DX,302H
        MOV     AL,00H
        OUT     DX,AL
        CALL    delay
;@@@@@@@@CODE END@@@@@@@@@@
        MOV AH,4CH
        INT  21H
delay       proc
            mov     cx, 003H
    delRep: push    cx
            mov     cx, 0D090H
    delDec: dec     cx
            jnz     delDec
            pop     cx
            dec     cx
            jnz     delRep
            ret
delay       endp
CODE ENDS
END START 