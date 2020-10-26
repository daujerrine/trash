; This program prints something on the screen, repeteadly
mov ah, 0x0E
mov al, 'e'
int 0x10
jmp cool

; To make this stop after printing once and preventing it going into a possible restart
; loop, we can make the jump instruction jump to itself indefinitely:
; jmp $

times 510 - ($ - $$) db 0

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

cool:
    
    mov al, 'w'  ; Push char into ah, then call the interrupt to print it.
    int 0x10
    mov al, 'e'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'a'
    int 0x10
    mov al, 'r'
    int 0x10
    mov al, 'e'
    int 0x10
    mov al, ' '
    int 0x10
    mov al, 'o'
    int 0x10
    mov al, 'u'
    int 0x10
    mov al, 't'
    int 0x10
    mov al, '!'
    int 0x10
    
    ;CR, LF
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    
    jmp $
