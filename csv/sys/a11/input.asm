section .data
    num1 dd 10
    num2 dd 20
    result dd 0

MACRO
ADD_VAL &X, &Y, &Z
ADD &X, &Y
MOV &Z, &X
MEND

section .text
    global _start

_start:
    mov eax, [num1]    ; Load first number
    mov ebx, [num2]    ; Load second number
    ADD_VAL eax, ebx, [result]  ; Use macro to add and store
    
    ; Exit program
    mov eax, 1      ; system call number (sys_exit)
    mov ebx, 0      ; exit code 0 (success)
    int 0x80        ; call kernel
