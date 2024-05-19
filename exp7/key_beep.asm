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
;-----------------------------------------
;------------接线：PB0-PB7:段选a-dp；
;-------------pc0-pc3  :key_p0-p3
;c 口高4位接GATE   计数器1、2级联，2输出连灯和BEEP
;---------------pa0-pa5:Q0-Q5&&LED0-LED6
start: 
 
        MOV    DX,30CH  ;写计数器1
        MOV    AL,76h
        OUT    DX,AL
        MOV    DX,304H
        MOV    AX,11750
        OUT    DX,AL
        MOV    AL,AH    
        OUT    DX,AL  

        MOV    DX,30cH
        MOV    AL,96H
        OUT    DX,AL
        MOV    AL,4
        MOV    DX,308H
        OUT    DX,AL 
;写计数器
        mov dx,ctrl
        mov al,81h
        out dx,al
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
loop5:  cmp bh,0bh
        jz  exit
        cmp bh,0ah
        jnz  t2
        call  timerset
        jmp   t2
exit:   mov dx,portb
        mov al,0
        out dx,al
        mov dx,porta
        out dx,al
wt:     jmp wt

timerset  proc    near
        mov     dx,portc
        or      al,0f0h
        out     dx,al
        ret
timerset  endp

delay   proc    near
        mov cx,0f00h
dlp:    loop    dlp
        ret
        delay   endp


code    ends
end start

