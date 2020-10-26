; Function library

; Print null terminated string
c_print:
    pusha
    cmp [bx], byte 0 ; The 'byte' tells the assembler to interpret operand as a byte.
    je c_print_end
    mov al, [bx]
    int 0x10
    inc bx
    jmp c_print
    
    c_print_end:
        popa
        ret

c_divloop: ; Divides ax by bl until ax == 0, and pushes all remainders to stack.
           ; The amount of numbers pushed to the stack is kept in cx.
    c_divloop_loop:
        inc cx
        div bl 
        mov dl, ah
        push dx
        mov ah, 0
        cmp al, 0
    jg c_divloop_loop
    ret
    
;Print number in ax, in decimal
c_pnum:
    pusha
    mov dx, 0
    mov cx, 0
    mov bl, 16
    call c_divloop
    
    mov ah, 0x0E ; !!! REMEMBER, div OVERWRITES AX
    
    c_paddr_printloop:
        pop dx
        mov al, dl
        add al, '0'
        int 0x10
        dec cx
        cmp cx, 0
        jg c_paddr_printloop
    
    popa
    ret

; Print number in ax, in hexadecimal
c_phexnum:
    pusha
    mov dx, 0
    mov cx, 0
    mov bl, 16
    call divloop
    
    mov ah, 0x0E ; !!!
    
    c_paddr_printloop:
        pop dx
        mov bx, hexstr
        add bx, dx
        mov al, [bx]
        int 0x10
        dec cx
        cmp cx, 0
        jg c_paddr_printloop
    
    popa
    ret


hexstr:
    db "0123456789ABCDEF"
