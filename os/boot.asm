; The most basic program that can be booted from an x86 machine

; The program starts with the code to be executed. In this case, an endless loop
; that is executed indefinitely by the processor. 

; The total size of this program is always 512 bytes

_start:
    jmp _start

times 510 - ($ - $$) db 0 ; pad 510 bytes, minus size of prev instructions (current address - starting address)

dw 0xaa55 ; This tells the processor that this is actually a boot sector (2 Bytes)

