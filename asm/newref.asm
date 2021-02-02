;; x86 ASM Reference File

;; Compilation:
; 1.  nasm                   \ # Assembler
;         -gdwarf            \ # Debugging symbols in the dwarf format
;         -f elf64           \ # Object Code Format
;         source.asm         \ # Source File
;         -l machinecode.lst \ # Equivalent machine code. Human readable.
;         -o object_file.o     # Output file (an object file)
;
; 2. ld -g object_file.o -o executable

;; Register Reference
; Accumulators:       (al, ah) -> ax, eax, rax
; Base:               (bl, bh) -> bx, ebx, rbx
; Counter:            (cl, ch) -> cx, ecx, rcx
; Data:               (dl, dh) -> dx, edx, rdx
; Source Index:       sil, si, esi, rsi
; Destination Index:  dil, di, edi, rdi
; Stack Pointer    :  spl, sp, esp, rsp
; Stack Base Pointer: bpl, bp, ebp, rbp
; Auxiliary:          [8 ... 15](none (64), 'd' (32), 'w' (16), 'b' (8))
; SIMD Registers:     xmm[0 ... 15]

;; Flags
; Carry:                   CF
; Parity (Even Parity):    PF
; Adjust (Used for BCD):   AF
; Zero (prev op res zero): ZF
; Sign (Sign bit 1)      : SF
; Direction              : DF
; Overflow               : OF

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::;

; Constant Values
; Similar to macros. Substituted for their actual value on assembling.
; Type is decided dynamically.
constval equ 10

;; Constants used in example

EXIT_SUCCESS equ 0  ; 'return 0;' Process exit code.
sys_exit     equ 60 ; Exit Syscall in linux


;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::;

; Pre-Initialised Variables are stored in .data
section .data

; Pre Initialised Values
; These tokens are by default pointers.
byte_var   db 255                  ; Byte (8 bits)
char_var   db "a"                  ; Byte (Used as a char)
string_var db "Hello"              ; Byte Array
short_var  dw 65535                ; Word (16 bits)
arr_var    dw 1, 2, 3, 4           ; Word Array
int_var    dd 4294967295           ; Double Word (32 bits)
float_var  dd 1.23456              ; Double Word (Used as float)
long_var   dq 18446744073709551615 ; Quad Word (64 bits)

;; Not Supported on supposedly 32 bit archs and others.

; longlong_var      ddq 12345        ; Double Quad Word (128 Bit Int)
; longlongfloat_var dt  1.2345       ; Double Prec. floating point

; Variables used in example 1

var_a dd 23
var_b dd 24
var_c dd 25

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::;

; Uninitialised Variables are stored in .bss ("Block Starting Symbol")
section .bss

byte_arr  resb 100 ; Byte Array of 100 elements.
word_arr  resw 200 ; Word Array. 200 elements.
dword_arr resd 300 ; Double Word Array. 300 elements.
qword_arr resq 400 ; Quad Word Array. 400 elements.

;; Not Supported on supposedly 32 bit archs and others.

; dqword_arr resdq 500 ; Double Quad Word Array. 500 elements.

;::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::;

; Actual Program Instructions
section .text
global _start

_start:
    ; MOV
    ; equivalent to dword [var_a] == *((dword *) var_a)
    mov eax, dword [var_a]
    add eax, dword [var_b]
    mov dword [var_c], eax

    ; LEA
    ; Load Effective Address
    ; equivalent to eax = &var_a;
    lea rax, [var_a + 1] ; You can do arithmetic here as well.

    ; MOVZX
    ; MOVe, Zero and eXtend. Casts a smaller type to a larger one.
    ; src must be smaller, dest must be larger
    ; equivalent to long int rax = *((byte *) byte_var)
    movzx rax, byte [byte_var]

    ; MOVSX
    ; Signed MOVZX
    ; For all lesser except dword -> qword conversion
    movsx rax, byte [byte_var]

    ; MOVSXD
    ; For dword -> qword conversion
    movsxd rax, dword [int_var]

    ; Convert <lreg> <greg>
    ; Operandless signed extenders
    ; ":" -> concatenation
    cbw  ; al  -> ax        b  -> w
    cwd  ; ax  -> dx:ax     w  -> dw
    cwde ; ax  -> eax       w  -> dw
    cdq  ; eax -> edx:eax   dw -> qw
    cdqe ; eax -> rax       dw -> qw
    cqo  ; eax -> rax:rdx   qw -> dqw

    ;; Arithmetic

    ; ADD
    ; x = x + y
    add rax, qword [long_var]

    ; INC
    ; x++
    inc rax

    ; ADC
    ; Add with carry
    ; x = x + y + carry
    adc rax, qword [long_var]

    ; SUB
    ; x = x - y
    sub rax, qword [long_var]

    ; ...
    ; Go see a proper reference book now.

program_exit:

    mov rax, sys_exit
    mov rdi, EXIT_SUCCESS
    syscall

