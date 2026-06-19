global main
extern printf
;Colleen in asm
section .data
code db "global main%1$cextern printf%1$c;Colleen in asm%1$csection .data%1$ccode db %2$c%3$s%2$c, 0%1$c%1$csection .text%1$cmain:%1$c    enter 0, 0%1$c    lea rdi, [rel code]%1$c    mov rsi, 10%1$c    mov rdx, 34%1$c    lea rcx, [rel code]%1$c    call printf%1$c    leave%1$c    ret%1$c", 0

section .text
main:
    enter 0, 0
    lea rdi, [rel code]
    mov rsi, 10
    mov rdx, 34
    lea rcx, [rel code]
    call printf
    leave
    ret
