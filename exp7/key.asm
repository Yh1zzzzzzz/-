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
;------------接线：PB0-PB7:段选a-dp；
;-------------pc0-pc3  :key_p0-p3
;---------------pa0-pa5:Q0-Q5&&LED0-LED6
start:  mov dx,ctrl
        mov al,89h
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
loop5:  cmp bh,0fh
        jz  exit
        call    ledxs
        jmp t2
exit:   mov dx,portb
        mov al,0
        out dx,al
        mov dx,porta
        out dx,al
wt:     jmp wt

ledxs   proc    near
        mov al,bh
        lea bx,disptab
        xlat    
        mov dx,portb
        out dx,al
        mov al,3fh
        mov dx,porta
        out dx,al
        ret
ledxs   endp

delay   proc    near
        mov cx,0f00h
dlp:    loop    dlp
        ret
        delay   endp


code    ends
end start

