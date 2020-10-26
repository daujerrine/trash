; loops.
%define CR 0x0D
%define LF 0x0A
[org 0x7C00]

mov ah, 0x0E ; Teletype mode
mov bp, 0x8000
mov sp, bp

_start:
    push 0
    push 10
    push 8
    push 6
    push 4
    push 2
    call pa
    jmp $
    
pa:
    mov bx, 2
    loop:
        pop dx
        cmp dx, 0
        je pa_end
        div bx
        mov al, dl
        add al, '0'
        mov ah, 0x0E    ; Setting teletype mode again due to div overwriting it.
        int 0x10
        jmp loop
        
    pa_end:
        ret
        
times 510 - ($ - $$) db 0 ; pad 510 bytes, minus size of prev instructions (current address - starting address)

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

