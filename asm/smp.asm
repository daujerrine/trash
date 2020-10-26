; The program returns 1 if eax <= 25 (I think), 2 otherwise
section .data
num db 34

section .text
global _start

_start:
mov ebx, num
add eax, 1 
mov ebx, num
mov eax, 1
int 0x80


