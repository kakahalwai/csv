; Macro definition
MACRO
ADD_VAL &X, &Y, &Z
ADD &X, &Y
MOV &Z, &X
MEND

; Program start
SECTION .text
    global _start

_start:
    ; Initialize registers
    MOV R1, 10      ; R1 = 10
    MOV R2, 20      ; R2 = 20
    
    ; Use the macro to add R1 and R2, store result in R3
    ADD_VAL R1, R2, R3    ; This will expand to:
                          ; ADD R1, R2
                          ; MOV R3, R1
    
    ; Use macro with different registers
    MOV R4, 30      ; R4 = 30
    ADD_VAL R3, R4, R5    ; This will expand to:
                          ; ADD R3, R4
                          ; MOV R5, R3
    
    ; Use macro with immediate values
    ADD_VAL R5, #5, R6    ; This will expand to:
                          ; ADD R5, #5
                          ; MOV R6, R5
    
    ; Exit program
    MOV R7, #1
    SWI 0
