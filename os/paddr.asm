; print number and/or an address.
%define CR 0x0D
%define LF 0x0A
[org 0x7C00]

mov ah, 0x0E ; Teletype mode
mov bp, 0x8500
mov sp, bp


_start:
    mov ax, 0xbd
    call c_paddr
    mov ah, 0x0E
    mov al, 'k'
    int 0x10
    jmp $
    
c_paddr:
    pusha
    mov dx, 0
    mov cx, 0
    mov bl, 16
    c_paddr_divloop: 
        div bl ; !!! REMEMBER, div OVERWRITES AX
        mov dl, ah
        push dx
        inc cx
        mov ah, 0
        cmp al, 0
        jg c_paddr_divloop
    
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
times 510 - ($ - $$) db 0 ; pad 510 bytes, minus size of prev instructions (current address - starting address)

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

