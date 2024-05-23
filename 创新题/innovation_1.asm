.486
porta equ 3c0h
portb equ 3c4h
portc equ 3c8h
ctrl equ  3cch
code segment use16
assume cs:code
        org 2000h
        jmp start
disptab db 3fh,06h,5bh,4fh,66h,6dh,7dh,07h,7fh
        db  6fh,77h,7ch,39h,5eh,79h,71h
kettab  db  0eeh,0edh,0ebh,0e7h,0deh,0ddh
        db  0dbh,0d7h,0beh,0bdh,0bbh,0b7h
        db  07eh,07dh,07bh,077h
;待实现的功能：按下1-3 选择亮灯个数
;             按下4-7 选择小灯方向  
;              按下 8 9 A选择速度（延时实现）
;-------------pc0-pc3  :key_p0-p3
;---------------pa0-pa3:Q0-Q3
; di用于延时函数
;8255配置为 A 输出 B 输出 C低四位输入&高四位输出
start:  mov cx,0f000h ;设置默认延时时间为0f000h
        mov dx,ctrl
        mov al,81h
        out dx,al
        ;上面用于初始化8255以及延时器
        mov dx,portb
        mov al,00h
        out dx,al
t2:     mov al,00h
        mov dx,porta
        out dx,al
        mov dx,portc
loop1:  in  al,dx
        and al,0fh
        cmp al,0fh
        je  loop1
        mov bh,al
        call    delay
        mov al,0ffh
        mov bl,al
loop2:   mov dx,porta
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jne loop3
        rol bl,1
        mov al,bl   
        jmp loop2
loop3:  mov dx,porta
        mov al,3fh
        out dx,al
        mov cl,4
        sal bl,cl
        or  bl,bh
        call    delay
        lea si,kettab
       mov  bh,0
loop4:  mov al,[si]
        cmp al,bl
        je  loop5
        inc bh
        inc si
        jmp loop4
;至此完成了对输入按键的读取，输入按键保存在bh当中
;在loop5中可以实现对应功能:
;待实现的功能：按下1-3 选择亮灯个数   
;             按下4-7 选择小灯方向  
;              按下 8 9 A选择速度（延时实现） 8->慢 9->中  A->快
;               按下E结束
loop5:  cmp bh,01h ;按下1 
        jz  func_one   
        cmp bh,02h ;按下2
        jz  func_two  
        cmp bh,03h ;按下3 
        jz  func_three   
        cmp bh,04h ;按下4
        jz  func_four  
        cmp bh,05h ;按下5
        jz  func_five   
        cmp bh,06h ;按下6
        jz  func_six   
        cmp bh,07h ;按下7 
        jz  func_siven  
        cmp bh,08h ;按下8 
        jz  func_eight  
        cmp bh,09h ;按下9
        jz  func_nine  
        cmp bh,0ah ;按下A
        jz  func_a  
        cmp bh,0eh ;按下1 
        jz  func_e
        ;如果按到其他键，则回到开始重新读
        jmp t2
func_one:   call    f_one
func_two:   call    f_two
func_three:   call    f_three
func_four:   call    f_four
func_five:   call    f_five
func_six:   call    f_six
func_siven:   call    f_siven
func_eight:   call    f_eight
func_nine:   call    f_nine
func_a:   call    f_a
func_e:   call    f_e
       jmp t2
;下面为对应功能的函数   注：在函数最后需要回到t2循环
f_one   proc    near
        MOV     DX,portb
        MOV     AL,0feh
        OUT     DX,AL
        CALL    delay
ROLL1:                  ;如果出现bug有可能是延迟函数里面ax寄存器出问题了？
        ROL     AL,1
        OUT     DX,AL
        CALL    delay   ;检查
        
        JMP     ROLL1
        ret
f_one   endp

f_two  proc    near
        ret
f_two   endp

f_three  proc    near
        ret
f_three   endp

f_four  proc    near
        ret
f_four   endp


f_five  proc    near
        ret
f_five   endp


f_six  proc    near
        ret 
f_six   endp

f_siven  proc    near
        ret
f_siven   endp


f_eight  proc    near
        ret
f_eight   endp


f_nine  proc    near
        ret
f_nine   endp


f_a  proc    near
        ret
f_a   endp

;按下e；所有灯亮灭三次熄灭
f_e  proc    near
        MOV     DX,portb
        MOV     AL,00h
        OUT     DX,AL
        CALL    delay_mid

        mov     al,0ffh
        out     dx,al
        call    delay_mid

        mov     al,00h
        out     dx,al
        call    delay_mid
        
        mov     al,0ffh
        out     dx,al
        call    delay_mid

        mov     al,00h
        out     dx,al
        call    delay_mid
        
        mov     al,0ffh
        out     dx,al
        call    delay_mid

        MOV  AH, 4CH
        INT  21H
        ret
f_e   endp
;下面为延时函数  具体延时时间放在cx寄存器，方便进行修改
;在亮灯循环中的调用只会修改cx寄存器的值
;亮灯过程中只会调用delay，默认延时时间为0f000h
delay_long   proc    near
        mov cx,0f000h
        ret
delay_long  endp

delay_mid   proc    near
        mov cx,0f00h
        ret
delay_mid  endp

delay_short   proc    near
        mov cx,0f0h
        ret
delay_short  endp

delay   proc    near
        push    ax
        mov     ax,cx
dlp:    loop    dlp
        mov     cx,ax
        pop      ax
        ret
delay  endp


code    ends
end start

