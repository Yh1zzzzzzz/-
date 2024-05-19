CODE SEGMENT USE16
    ASSUME CS:CODE,DS:CODE,ES:CODE
    ORG 2000H
;接线  k1-k8接 pb0-pb7   c口接位选DS   A口接段选  
START:  JMP BEG
BEG:    MOV    AX,CODE
        MOV    DS,AX
        MOV    ES,AX
        mov     DX,30CH
        MOV     AL,10000010B;PORT A:OUT  PORT B:IN   PORT C:OUT
        OUT     DX,AL

YHT:     
        MOV     BL,80H
        MOV     DX,304H
        IN      AL,DX
        AND     AL,01H
        TEST    AL,01H
        JZ      SHOWF    ;从左往右循环显示F ————》71H
        jnz     SHOW    ;所有LED显示"一""   01000000B
SHOWF:  MOV     DX,308H
        MOV     BL,10000000B
SHOWF_INSIDELOOP:
        MOV     DX,308H
        MOV     AL,BL
        OUT     DX,AL
        MOV     DX,300H
        MOV     AL,71H
        OUT     DX,AL
        ;完成第一次送F，下面检测状态是否改变
        MOV     DX,304H
        IN      AL,DX
        AND     AL,01H
        TEST    AL,01H
        jnz     YHT
        ROR     BL,1
        CALL    delay
        JMP     SHOWF_INSIDELOOP

SHOW:   MOV     DX,308H
        MOV     AL,0FFH
        OUT     DX,AL
        MOV     DX,300H
        MOV     AL,01000000B 
        OUT     DX,AL
        MOV     DX,304H
        IN      AL,DX
        AND     AL,01H
        TEST    AL,00H
        jnz     YHT
        JMP   SHOW


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