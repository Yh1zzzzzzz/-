; multi-segment executable file template.

data segment
   ary   db  5ah  ;待拆字符
   ary_l db  0    ;待拆字符低位
   ary_h db  0     ;待拆字符高位
ends

code segment  
   assume ds:data ,cs:code
start: 
    mov ax, data
    mov ds, ax
    mov es, ax
    lea si,ary
    mov ax,[si]
    mov bx,ax
    and ax,000Fh
    and bx,00F0h 
    shr bx,4
    mov ary_l,al
    mov ary_h,bl

    ; wait for any key....    
    mov ah, 1
    int 21h
    
    mov ax, 4c00h ; exit to operating system.
    int 21h    
ends

end start ; set entry point and stop the assembler.
