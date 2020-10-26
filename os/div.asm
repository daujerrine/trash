%define CR 0x0D
%define LF 0x0A
[org 0x7C00]
;                    Quotient    Remainder  Dividend
; reg / 8bit reg -->    al          ah         AX
; reg / 16bit reg -->   ax          dx       DX:AX 
;

mov ah, 0x0E ; Teletype mode

_start:
    mov bl, 2
    mov ax, 3
    div bl
    
    mov ah, 0x0E ; setting teletype mode again
    
    add al, '0'
    int 0x10
    
    mov al, 'k'
    int 0x10
    jmp $
    


times 510 - ($ - $$) db 0 ; pad 510 bytes, minus size of prev instructions (current address - starting address)

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

