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
num     db   1   ;   后续每次位移数
num_left     db   15   ;后续每次位移数（实际上并没有用上）
status       db   0  ; 左右移动状态标志  0右移  1左移  2 中间往两边  3  两边往中间 （用于延迟函数跳转）
delay_time   dw   0ffffh
mode  db    1   ;移动模式状态标志  1 2 3 4     用于个数函数跳转
;实现的功能：按下1-3 选择亮灯个数
;             按下4-7 选择小灯方向  
;              按下 8 9 A选择速度（延时实现）
;          A口接左边8小灯   B口接右边8 
;-------------pc0-pc3  :key_p0-p3
;---------------pc4-pc7:Q0-Q3
;8255配置为 A 输出 B 输出 C低四位输入&高四位输出
start:  mov cx,0f000h
        mov di,0ffffh ;设置默认延时时间为0f000h,默认小灯全灭,默认每次位移一个灯
        mov dx,ctrl
        mov si,1
        mov al,81h
        out dx,al
        ;上面用于初始化8255以及延时器
        mov dx,portb
        mov al,00h
        out dx,al
scan:   t2:     mov al,00h  ;这里为啥不封装成函数呢？
                        ;因为不知道为什么，一封装就有问题
                        ;而且难以debug，所以还是用循环吧
        mov dx,portc
        out dx,al
        mov dx,portc
loop1:  in  al,dx
        and al,0fh
        cmp al,0fh
        je  loop1
        mov bh,al          ;bh保存列
        call    delay      
        mov al,0efh
        mov bl,al
loop2:  mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh         ;bl高四位为行
        cmp al,0fh
        jne loop3
        rol bl,1
        mov al,bl   
        jmp loop2
loop3:  
        and bl,0f0h       
        or  bl,bh
        call    delay
        push  si
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
loop5:  
        cmp bh,01h ;按下1 
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
        
        jmp scan
;数量选择函数
func_one:   mov     num,1
            cmp     mode,1
            jz      func_four
                        cmp     mode,2
            jz      func_five
                        cmp     mode,3
            jz      func_six
                        cmp     mode,4
            jz      func_siven
            
 

func_two:   
                  mov     num,2
            cmp     mode,1
            jz      func_four
                        cmp     mode,2
            jz      func_five
                        cmp     mode,3
            jz      func_six
                        cmp     mode,4
            jz      func_siven
            
func_three:   
            mov     num,3
            cmp     mode,1
            jz      func_four
                        cmp     mode,2
            jz      func_five
                        cmp     mode,3
            jz      func_six
                        cmp     mode,4
            jz      func_siven
            
;下面为方向选择函数一（从左往右）  方向选择要根据数量给对应的初始值di
func_four:   cmp     num,1
            je      func_four_one
            cmp     num,2
            je      func_four_two
            cmp     num,3
            je      func_four_three
            jmp     func_five
func_four_one:   mov     di,0111111111111111b
			mov   status,1
            call    f_main_right
            mov   mode,1
            jmp scan
func_four_two:   mov     di,0011111111111111b
			mov   status,1

            call    f_main_right
                        mov   mode,1
            jmp scan
func_four_three:   mov     di,0001111111111111b
			mov   status,1

            call    f_main_right
                        mov   mode,1
            jmp scan
;下面为方向选择函数二（从右往左）   方向选择要根据数量给对应的初始值di
func_five:   cmp     num,1
            je      func_five_one
            cmp     num,2
            je      func_five_two
            cmp     num,3
            je      func_five_three
            jmp     func_six
func_five_one:   mov     di,1111111111111110b
			mov   status,0
                 mov     num_left,15
                 call    f_main_left
                             mov   mode,2
                 jmp scan
func_five_two:   mov     di,1111111111111100b
			mov   status,0
                 mov     num_left,14
                 call    f_main_left
                             mov   mode,2
                 jmp     scan
func_five_three:   mov     di,1111111111111000b
			mov   status,0
                   mov     num_left,13
                   call    f_main_left
                               mov   mode,2
                   jmp     scan
;下面为方向选择函数三（从中间往两边）即分别左移   方向选择要根据数量给对应的初始值di
func_six:   cmp     num,1
            je      func_six_one
            cmp     num,2
            je      func_six_two
            cmp     num,3
            je      func_six_three
            jmp     func_siven
func_six_one:   mov     di,1111111001111111b
                mov   status,2
            call    f_main_center_side
                        mov   mode,3
            jmp scan
func_six_two:   mov     di,1111110000111111b
                 			mov   status,2
            call    f_main_center_side
                        mov   mode,3
            jmp scan
func_six_three:   mov     di,1111100000011111b
                 			mov   status,2
            call    f_main_center_side
                        mov   mode,3
            jmp scan
;下面为方向选择函数四（从两边往中间）即分别右移   方向选择要根据数量给对应的初始值di
func_siven:   cmp     num,1
            je      func_siven_one
            cmp     num,2
            je      func_siven_two
            cmp     num,3
            je      func_siven_three
func_siven_one:   mov     di,0111111111111110b
			mov   status,3
            call    f_main_side_center
                        mov   mode,4
            jmp scan
func_siven_two:   mov     di,0011111111111100b
			mov   status,3
            call    f_main_side_center
                        mov   mode,4
            jmp scan
func_siven_three:   mov     di,0001111111111000b
			mov   status,3
            call    f_main_side_center
                        mov   mode,4
            jmp scan
;下面为速度选择函数
func_eight:   call    delay_short
		 cmp     status,0
            jz        eight_status0
        cmp     status,1
            jz        eight_status1
            cmp     status,2
            jz        eight_status2
            cmp     status,3
            jz        eight_status3
            jmp  scan
eight_status0 :call f_main_left
            jmp scan
eight_status1 :call f_main_right
            jmp scan
eight_status2 :call f_main_center_side
            jmp scan
eight_status3 :call f_main_side_center
            jmp scan

func_nine:   call    delay_mid
 		    cmp     status,0
            jz        nine_status0
            cmp     status,1
            jz        nine_status1
            cmp     status,2
            jz        nine_status2
            cmp     status,3
            jz        nine_status3
            jmp  scan
            
nine_status0 :call f_main_left
            jmp scan
nine_status1 :call f_main_right
            jmp scan
nine_status2 :call f_main_center_side
            jmp scan
nine_status3 :call f_main_side_center
            jmp scan

    
func_a:   call       delay_long
 		cmp     status,0
            jz        a_status0
        cmp     status,1
            jz        a_status1
            cmp     status,2
            jz        a_status2
            cmp     status,3
            jz        a_status3
            jmp  scan
a_status0 :call f_main_left
            jmp scan
a_status1 :call f_main_right
            jmp scan
a_status2 :call f_main_center_side
            jmp scan
a_status3 :call f_main_side_center
            jmp scan

;下面为结束函数
func_e:   call    f_e


;至此main函数结束，下为封装好的功能函数


;下面为对应功能的函数   注：在函数最后需要回到scan循环(在main里手动实现)
;@@@brief:  亮灯函数，每次按键按下都会进入该函数，这是右移的情况
;@param:   di:亮灯个数、初始状态   num:每次位移次数    
;@return:  无
;@note:    无 
f_main_right   proc    near
loop_one_right:       
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL
        call    delay
        ;上面完成了亮1个灯的功能,下面开始向右移位
        push     cx
        mov     cl,num
        ror     di,cl
        pop     cx
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL ;检测是否按键改变
        CALL    delay
        push     cx
        mov     cl,num
        ror     di,cl
        pop     cx    ;向右移位

        mov al,00h
        mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jz   loop_one_right                                   ;如果出现bug有可能是延迟函数里;面ax寄存器出问题了？
        ret
f_main_right  endp

f_main_left   proc    near
;@@@brief:  亮灯函数，每次按键按下都会进入该函数，这是左移的情况
;@param:   di:亮灯个数、初始状态   num:每次位移次数    
;@return:  无
;@note:    无 
loop_one_left:       
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL
        call    delay
        ;上面完成了亮1个灯的功能,下面开始向右移位
        push     cx
        mov     cl,num
        rol     di,cl
        pop     cx
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL ;检测是否按键改变
        CALL    delay
        push     cx
        mov     cl,num
        rol    di,cl
        pop     cx    ;向右移位

        mov al,00h
        mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jz   loop_one_left                                   ;如果出现bug有可能是延迟函数里;面ax寄存器出问题了？
        ret
f_main_left  endp
;@@@brief:  亮灯函数，每次按键按下都会进入该函数，这是中间往两边的情况
;@param:   di:亮灯个数、初始状态   num:每次位移次数    
;@return:  无
;@note:    无 
f_main_center_side   proc    near
loop_one_center_side:       
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL
        call    delay
        ;上面完成了亮di的功能,下面开始向移位
        push     cx
        mov     cl,num
        push     bx
        mov     bx,di
        ror     bl,cl
        rol     bh,cl
        pop     cx
        MOV     AX,bx
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL ;检测是否按键改变
        mov     di,bx  ;更新di
        pop     bx
        CALL    delay
        push     cx
        mov     cl,num
        push     bx
        mov     bx,di
        ror     bl,cl
        rol     bh,cl
        mov     di,bx  ;更新di
        pop     bx
        pop     cx    ;向右移位

        mov al,00h
        mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jz   loop_one_center_side                                   ;如果出现bug有可能是延迟;函数里面ax寄存器出问题了？
        ret
f_main_center_side  endp

;@@@brief:  亮灯函数，每次按键按下都会进入该函数，这是两边往中间的情况
;@param:   di:亮灯个数、初始状态   num:每次位移次数    
;@return:  无
;@note:    无 
f_main_side_center   proc    near
loop_one_side_center:       
        MOV     AX,di
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL
        call    delay
        ;上面完成了亮di的功能,下面开始向移位
        push     cx
        mov     cl,num
        push     bx
        mov     bx,di
        rol     bl,cl
        ror     bh,cl
        pop     cx
        MOV     AX,bx
        mov     DX,portb
        out     DX,AL
        SHR     AX,8
        mov     DX,porta
        out     DX,AL ;检测是否按键改变
        mov     di,bx  ;更新di
        pop     bx
        CALL    delay
        push     cx
        mov     cl,num
        push     bx
        mov     bx,di
        rol     bl,cl
        ror     bh,cl
        mov     di,bx  ;更新di
        pop     bx
        pop     cx    ;向右移位

        mov al,00h
        mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jz   loop_one_side_center                                   ;如果出现bug有可能是延迟;函数里面ax寄存器出问题了？
        ret
f_main_side_center  endp
;@@@brief:  结束函数，亮灭三次后函数结束
;@param:  none
;@return:  无
;@note:    无 
f_e  proc    near
        call    delay_long
        MOV     DX,portb
        MOV     AL,00h
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,00h
        OUT     DX,AL
        CALL    delay
        
        MOV     DX,portb
        MOV     AL,0ffh
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,0ffh
        OUT     DX,AL
        CALL    delay

        MOV     DX,portb
        MOV     AL,00h
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,00h
        OUT     DX,AL
        CALL    delay

        MOV     DX,portb
        MOV     AL,0ffh
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,0ffh
        OUT     DX,AL
        CALL    delay

        MOV     DX,portb
        MOV     AL,00h
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,00h
        OUT     DX,AL
        CALL    delay

        MOV     DX,portb
        MOV     AL,0ffh
        OUT     DX,AL
        MOV     DX,porta
        MOV     AL,0ffh
        OUT     DX,AL
        CALL    delay

        MOV  AH, 4CH   ;这里可能有问题，需要debug
        INT  21h
        ret
f_e   endp
;@@func delay_long
;@@param none
;@@brief 修改延时时间
;@@return none
delay_long   proc    near
        mov delay_time,0ffffh
        ret
delay_long  endp
;@@func delay_mid
;@@param none
;@@brief 修改延时时间
;@@return none
delay_mid   proc    near
        mov delay_time,7fffh
        ret
delay_mid  endp
;@@func delay_short
;@@param none
;@@brief 修改延时时间
;@@return none
delay_short   proc    near
        mov delay_time,0f00h
        ret
delay_short  endp
;@@@brief:  延时函数
;@param:   delay_time:延时时间    
;@return:  无
;@note:    无 
delay   proc    near
        push    cx
        mov    cx,delay_time
dlp:    loop    dlp
        pop     cx
        ret
delay  endp
;@@func key_scan
;@@param none
;@@brief 测试键盘输入
;@@return bh->按下的按键值（0-f）  没有按键按下时返回0
;@@封装好的函数，但是调用会出错，debug中
key_scan  proc  near
scan2:     mov al,00h
        mov dx,portc
        out dx,al
        mov dx,portc
loop12:  in  al,dx
        and al,0fh
        cmp al,0fh
        je  loop12
        mov bh,al          ;bh保存列
        call    delay      
        mov al,0efh
        mov bl,al
loop22:  mov dx,portc
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh         ;bl高四位为行
        cmp al,0fh
        jne loop33
        rol bl,1
        mov al,bl   
        jmp loop22
loop33:  
        and bl,0f0h       
        or  bl,bh
        call    delay
        lea si,kettab
       mov  bh,0
loop44:  mov al,[si]
        cmp al,bl
        je  backto
        inc bh
        inc si
        jmp loop44
backto:
        ret
key_scan endp
;@@func test_key
;@@param none
;@@brief 测试键盘输入
;@@return bh->按下的按键值（0-f）  没有按键按下时返回0
;@@封装好的函数，但是调用会出错，debug中
test_key proc    near
	push ax
	push cx
	push si
t20:      mov al,00h
        mov dx,porta
        out dx,al
        mov dx,portc
loop10:  in  al,dx
        and al,0fh
        cmp al,0fh
        je  nokey
        mov bh,al
        call    delay_short
        mov al,0feh
        mov bl,al
loop20:   mov dx,porta
        out dx,al
        mov dx,portc
        in  al,dx
        and al,0fh
        cmp al,0fh
        jne loop3
        rol bl,1
        mov al,bl   
        jmp loop2
loop30:  mov dx,porta
        mov al,3fh
        out dx,al
        mov cl,4
        sal bl,cl
        or  bl,bh
        call    delay
        lea si,kettab
       mov  bh,0
loop40:  mov al,[si]
        cmp al,bl
        je  loop50
        inc bh
        inc si
        jmp loop4
        
loop50:   
	pop si
	pop  cx
	pop  ax
         ret

nokey:	pop si
	pop  cx
	pop  ax  
       mov bh,0
        ret
test_key endp
code    ends
end start

