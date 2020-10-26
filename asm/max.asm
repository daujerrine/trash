section .data
numlist dd 1,3,3,9,6,81,0 ; 0 terminated list

section .text
global _start

_start:
mov edi, 0 ; we use the INDEX register to keep track of where we are on the numlist, i.e numlist[[i] i = 1,2,3, ...
mov eax, [numlist + 4*edi] ; numlist[4 * edi] -> eax i.e numlist[0] -> eax
cmp eax, 0
je exit
mov ebx, eax ; store the biggest number.

loop:
inc edi
mov eax, [numlist + 4*edi]
cmp eax, 0
je exit
cmp eax, ebx
jle loop
mov ebx, eax
jmp loop

exit:
mov eax, 1 
int 0x80
 
