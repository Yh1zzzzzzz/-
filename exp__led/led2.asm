CODE SEGMENT USE16
    ASSUME CS:CODE,DS:CODE,ES:CODE
    ORG 2000H
;接线  b口接位选(324H)    c口接段选(328H)   8255->320H  
;20221689
;2---5BH
;0---3FH
;1--06H
;6--7DH
;8--7FH
;9--6FH
START:  JMP BEG
BEG:    MOV    AX,CODE
        MOV    DS,AX
        MOV    ES,AX
        MOV    DX,32CH
        MOV    AL,80H
        OUT     DX,AL

        MOV     DX,324H
        MOV     AL,10000000B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,5BH 
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,01000000B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,3FH 
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00100000B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,5BH 
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00010000B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,5BH 
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00001000B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,06H
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00000100B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,7DH
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00000010B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,7FH
        OUT     DX,AL

                MOV     DX,324H
        MOV     AL,00000001B
        OUT     DX,AL
        MOV     DX,328H 
        MOV     AL,6FH
        OUT     DX,AL

        JMP     BEG
        


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