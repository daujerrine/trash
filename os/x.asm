mov ah, 0x0E ; Start teletype mode

_loop:
    
    jmp $

; To make this stop after printing once and preventing it going into a possible restart
; loop, we can make the jump instruction jump to itself indefinitely:
; jmp $
msg:
db 'X'

times 510 - ($ - $$) db 0

dw 0xaa55
