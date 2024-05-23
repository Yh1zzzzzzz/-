.486
porta equ 3c0h
portb equ 3c4h
portc equ 3c8h
ctrl equ  3cch


code segment use16
assume cs:code
        org 2000h
        jmp start
disptab db 3fh,06h,5bh,4fh,66h,6dh,7dh,07h,7fh,6fh;0-9字形表
kettab  db  0eeh,0edh,0ebh,0e7h,0deh,0ddh
        db  0dbh,0d7h,0beh,0bdh,0bbh,0b7h  ;键特征值
        db  07eh,07dh,07bh,077h
led0  db 0
led1  db 0
led2  db 0
led3  db 0
led4  db 0
led5  db 0
;待实现的功能：开机显示学号
;             启动、暂停、继续、复位、kill功能 
;-------------pc0-pc3  :key_p0-p3
;-------------pc4-pc7  :key_q0-q3
; di用于延时函数
;8254配置方式2   级联   地址300H
;gate0 接A口最高位

;8255配置为 A 输出 B 输出 C低四位输入（键盘）&高四位输入（定时器）  地址：3c0h
;显示学号时，A 位选  B段选
;只用一行键盘所以可以直接读入键特征值
;键盘列直接全接0
start:  mov si,0
        mov dx,ctrl
        mov al,81h
        out dx,al
        ;上面用于初始化8255以及延时器
        ;下面初始化8254
        MOV    DX,30CH  ;写通道0
        MOV    AL,3eH
        OUT    DX,AL
        MOV    DX,300H
        MOV    AX,940
        OUT    DX,AL
        MOV    AL,AH  
        OUT    DX,AL    
         ;完成计数器一的初始化   
        mov dx,portb
        mov al,00h
        out dx,al

        call    show_id
 keyin:       call    test_key
;至此完成了对输入按键的读取，输入按键保存在bh当中
;在loop5中可以实现对应功能:
;待实现的功能：按下1  启动(把A的)  2暂停 3继续  4复位  5kill功能
;             
;              
;               
      cmp bh,01h ;按下1 (A口最高位送1，开始计时)
        jz  oneone
   
        cmp bh,02h ;按下2
        jz  twotwo
        cmp bh,03h ;按下3 
        jz  threethree
        cmp bh,04h ;按下4
        jz  fourfour
        cmp bh,05h ;按下5
        jz  fivefive
        cmp  bh,00h
        jz  zerozero
        jmp  yht ;按下其他键直接进循环
        ;如果按到其他键，则回到开始重新读
oneone:        Call f_one
                 jmp  yht 
twotwo:        call f_two
                 jmp  yht
threethree:        call f_three
                 jmp  yht
fourfour:        call f_four
                 jmp  yht
fivefive:        call f_five
                 jmp  yht
zerozero:       jmp yht
yht:    mov dx,portc
        in al,dx
        or al,00001111b
        cmp al,00001111b
        jnz   downs ;相等说明达到了一次计数值
        add  si,1    ;每次上升沿si加1
        inc  led0  
downs: 
        cmp  led0,10
        jnz   unresetled0
        mov  led0,0
        inc  led1  
unresetled0:
        cmp led1,10
        jnz   unresetled1
        mov  led1,0
        inc  led2 
unresetled1:
        cmp led2,10
        jnz   unresetled2
        mov  led2,0
        inc  led3  
unresetled2:
        cmp led3,10
        jnz   unresetled3
        mov  led3,0
        inc  led4  
unresetled3:
        cmp led4,10
        jnz   unresetled4
        mov  led4,0
        inc  led5  
unresetled4:
        cmp led5,10
        jnz   unresetled5 ; 循环结束
        mov  led5,0
        call  f_five
unresetled5:;至此每次循环更新完成，调用显示函数（待完成）
            ;至此led0-5存储器中都是现在应该显示的数字，待完成显示函数
        
                           


;下为每次主循环调用结束后，返回按键读取处
jmp keyin
;下面为对应功能的函数   注：在函数最后需要回到t2循环
f_one   proc    near
        mov  dx,porta
        mov  al,0ffh
        out  dx,al
        ret
f_one   endp

f_two  proc    near
        mov  dx,porta
        mov  al,00111111b
        out  dx,al
        ret
f_two   endp

f_three  proc    near
        mov  dx,porta
        mov  al,0ffh
        out  dx,al
        ret
f_three   endp


f_four  proc    near
        mov  led0,0
        mov  led1,0
        mov  led2,0
        mov  led3,0
        mov  led4,0
        mov  led5,0
        ret
f_four   endp


f_five  proc    near
        mov  dx,porta
        mov  al,0ffh
        out  dx,al

        mov  dx,portb
        mov  al,0ffh
        out  dx,al
        call delay

        mov  dx,portb
        mov  al,00h
        out  dx,al
        call delay

        mov  dx,portb
        mov  al,0ffh
        out  dx,al
        call delay

        mov  dx,portb
        mov  al,00h
        out  dx,al
        call delay
        mov  dx,portb
        mov  al,0ffh
        out  dx,al
        call delay

        mov  dx,portb
        mov  al,00h
        out  dx,al
        call delay
        
wt:     nop
        jmp wt  
        ret
f_five   endp




;按下e；所有灯亮灭三次熄灭
f_e  proc    near
        MOV     DX,portb
        MOV     AL,00h
        OUT     DX,AL
        CALL    delay

        mov     al,0ffh
        out     dx,al
        call    delay

        mov     al,00h
        out     dx,al
        call    delay
        
        mov     al,0ffh
        out     dx,al
        call    delay

        mov     al,00h
        out     dx,al
        call    delay
        
        mov     al,0ffh
        out     dx,al
        call    delay

        MOV  AH, 4CH
        INT  21H
        ret
f_e   endp
;下面为延时函数  具体延时时间放在cx寄存器，方便进行修改
;在亮灯循环中的调用只会修改cx寄存器的值
;亮灯过程中只会调用delay，默认延时时间为0f000h


delay   proc    near
        mov     cx,0f00h
        push    ax
        mov     ax,cx
dlp:    loop    dlp
        mov     cx,ax
        pop      ax
        ret
delay  endp
;@@func show_id
;@@param   none
;@@brief 利用8255 A B口显示学号
show_id  proc    near

        MOV     DX,porta
        MOV     AL,10000000B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,5BH 
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,01000000B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,3FH 
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,00100000B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,5BH 
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,00010000B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,5BH 
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,00001000B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,06H
        OUT     DX,AL
        call    delay



        MOV     DX,porta
        MOV     AL,00000100B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,7DH
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,00000010B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,7FH
        OUT     DX,AL
        call    delay

        MOV     DX,porta
        MOV     AL,00000001B
        OUT     DX,AL
        MOV     DX,portb 
        MOV     AL,6FH
        OUT     DX,AL
        call    delay
            ret
show_id  endp
;@@func test_key
;@@param none
;@@brief 测试键盘输入
;@@return bh->按下的按键值（0-f）  没有按键按下时返回0
test_key proc    near
t2:      mov al,00h
        mov dx,porta
        out dx,al
        mov dx,portc
loop1:  in  al,dx
        and al,0fh
        cmp al,0fh
        je  nokey
        mov bh,al
        call    delay
        mov al,0feh
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
        
loop5:   
         ret

nokey:   mov bh,0
        ret
test_key endp
code    ends
end start

