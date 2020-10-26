; This defines a print function to print null-terminated string.
%define CR 0x0D
%define LF 0x0A
[org 0x7C00]

mov ah, 0x0E ; Teletype mode

_start:
    mov bx, string
    call c_print
    mov al, 'k'
    int 0x10
    jmp $
    
string:
    db 'hey', 0

c_print:
    pusha
    cmp [bx], byte 0
    je c_print_end
    mov al, [bx]
    int 0x10
    inc bx
    jmp c_print
    
    c_print_end:
        popa
        ret

times 510 - ($ - $$) db 0 ; pad 510 bytes, minus size of prev instructions (current address - starting address)

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

