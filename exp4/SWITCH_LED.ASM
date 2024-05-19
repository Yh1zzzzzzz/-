CODE SEGMENT USE16
    ASSUME CS:CODE,DS:CODE,ES:CODE
    ORG 2000H
START:  JMP BEG
;@FUNC:利用8个开关K1～K8控制8个小灯D1～D8的状态。
;要求：
;STATE1::   IN:01H      OUT:FEH 且延迟左移            当只有开关K1扳向前时，小灯从右到左逐个循环点亮   
;STATE2::   IN:03H          FCH 且延迟左移          当只有开关K1、K2扳向前时，小灯从右到左成对逐次循环点亮；
;STATE3::   IN:07H          F8H 且延迟左移        当只有开关K1、K2和K3扳向前时，小灯从右到左3个为一组逐次循环点亮；
;STATE4::   IN:0FH          F0H且延迟左移
;STATE5::   IN:1FH          E0H且延迟左移
;STATE6::   IN:3FH          C0H且延迟左移
;STATE7::   IN:7FH          80H且延迟左移
;STATE8::   IN:FFH          00H                       以此类推，当全部开关都扳向前时，8个小灯全亮；
;STATE9::   OTERS           FFH          当开关状态不满足上述情况时，小灯全部熄灭。任何时候扳动开关，小灯状态随之改变。
;通过8255A芯片的B口读入开关状态，C口控制发光二极管。

;电路连接：PB7--K8  PB6--K7 ......PB0-K1
          ;PC7--D1     ..........PC0-D8


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
        CMP    AL,01H
        JZ      STATE1
        CMP     AL,03H
        JZ      STATE2
        CMP     AL,07H
        JZ      STATE3
        CMP     AL,0FH
        JZ      STATE4
        CMP     AL,1FH
        JZ      state5_temp
        CMP     AL,3FH
        JZ      state6_temp
        CMP     AL,7FH
        JZ      state7_temp
        CMP     AL,0FFH
        JZ      state8_temp
        JMP     state9_temp
STATE1: 
        MOV     DX,302H
        MOV     AL,0FEH
        OUT     DX,AL
        CALL    delay
ROLL1:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,01H
        jnz     AGN
        pop     DX
        pop     AX
        JMP     ROLL1
;@@@@       S2
STATE2: 
        MOV     DX,302H
        MOV     AL,0FCH
        OUT     DX,AL
        CALL    delay
ROLL2:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,03H
        jnz     AGN
        pop     DX
        pop     AX
        JMP     ROLL2
;@@@        S3
STATE3: 
        MOV     DX,302H
        MOV     AL,0F8H
        OUT     DX,AL
        CALL    delay
ROLL3:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,07H
        jnz     AGN
        pop     DX
        pop     AX
        JMP     ROLL3
;     中转
state5_temp:
        JMP STATE5
state6_temp:
        JMP STATE6
state7_temp:
        JMP STATE7
state8_temp:
        JMP STATE8
state9_temp:
        JMP STATE9

AGN_TEMP:
        JMP AGN
;@@     S4
STATE4: 
        MOV     DX,302H
        MOV     AL,0F0H
        OUT     DX,AL
        CALL    delay
ROLL4:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FH
        jnz     AGN_TEMP
        pop     DX
        pop     AX
        JMP     ROLL4
;*********S5 S5
STATE5: 
        MOV     DX,302H
        MOV     AL,0E0H
        OUT     DX,AL
        CALL    delay
ROLL5:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,1FH
        jnz     AGN_TEMP
        pop     DX
        pop     AX
        JMP     ROLL5
;*****8 S6 S6
STATE6: 
        MOV     DX,302H
        MOV     AL,0C0H
        OUT     DX,AL
        CALL    delay
ROLL6:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,3FH
        jnz     AGN_TEMP
        pop     DX
        pop     AX
        JMP     ROLL6
;****** S7
STATE7: 
        MOV     DX,302H
        MOV     AL,80H
        OUT     DX,AL
        CALL    delay
ROLL7:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,7FH
        jnz     AGN_TEMP
        pop     DX
        pop     AX
        JMP     ROLL7
;***** S8
AGN_TEMP_TEMP:
        JMP AGN_TEMP
STATE8: 
        MOV     DX,302H
        MOV     AL,00H
        OUT     DX,AL
        CALL    delay
ROLL8:  
        ROL     AL,1
        OUT     DX,AL
        CALL    delay
        push    AX
        push    DX
        MOV     DX,301H
        IN      AL,DX
        CMP     AL,0FFH
        jnz     AGN_TEMP_TEMP
        pop     DX
        pop     AX
        JMP     ROLL8
;       S9
STATE9: 
        MOV     DX,302H
        MOV     AL,0FFH
        OUT     DX,AL
        CALL    delay
        JMP     AGN
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