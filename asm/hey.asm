; A hello world, in assembly.
; compile using nasm

; In the .data section, you declare your program's *data*. That is, your
; variables etc.

section .data
; This is our message, "hey", statically stored in a variable named
; "hwrld". The "db" means "define byte", which is what we are doing: defining a
; sequence of bytes. The "10" is added into hwrld after the "hey" is the ascii 
; character for a newline.
hwrld db 'hey', 10 

; The dollar sign here refers to the "current address".
; When we passed the previous instruction to our assembler, it shifted our
; instruction pointer to the end of hey, which is 4 bytes counting the newline.
; we then minus the address of the start of hwrld, giving us the difference 
; between the start and the end of hwrld, i.e. the length of hwrld (4 bytes)
len equ $ - hwrld 

; This is our text section, which is our actual program. Here we can execute 
; instructions and do computation.
section .text
; This tells us to *start* the program from "_start", a label which you can see
; below.
global _start

; Our _start label.
_start:

; We have written this program for linux. To do anything to the system in linux
; , we must call the system's kernel, or essentialy the operating system itself 
; to do it for us. Direct manipulation of hardware components isn't possible 
; unless we write the system from the ground up.

; Linux defines a series of system calls which allows us to do stuff to the 
; system. In this case, we want to show our message. Linux has a "write" system
; call which writes a string to a device/file descriptor, and in our case,
; device 0, or standard output (our screen).

; The write call takes 4 args, the length of the string (number), the string 
; itself, and the device to write it to.
; The call looks for these values in temprorary storage locations known as 
; registers. The write call looks for:
; string length in edx
; string in ecx
; device in ebx

; we *move* these values to our registers using the mov instruction:
mov edx, 1
mov ecx, len
mov ebx, 1

; Now this is the number of the system call write, which the system looks up
; for figuring out which call to execute.
mov eax, 4
; Now, we call our system to carry our operation, or in other words,
; *Interrupt* our system to handle our job of writing to the screen.
int 0x80

; We are now going to terminate the program after it has done its job.
; The syscall (system call) exit accepts one argument, which is the return
; value of the program. It provides the return value to the system and then
; terminates the program. Its syscall number is 1.
mov ebx, 0
mov eax, 1
int 0x80

; You can lookup more syscalls in the linux source code or here:
; http://syscalls.kernelgrok.com/
; for a nice list.

