; A hello world, in assembly.
; compile using nasm

; In the .data section, you declare your program's *data*. That is, your
; variables etc.

section .data

; This is our text section, which is our actual program. Here we can execute 
; instructions and do computation.
section .text
; This tells us to *start* the program from "_start", a label which you can see
; below.
global _start

; Our _start label.
_start:

mov ebx, 0
mov eax, 1
int 0x80
