global main
extern fopen
extern fprintf
extern fclose

%define NEWLINE 10
%define QUOTE 34
%define FN "Grace_kid.s"

;Grace in asm
section .data
fname db FN, 0
mode db "w", 0
code db "global main%1$cextern fopen%1$cextern fprintf%1$cextern fclose%1$c%1$c%%define NEWLINE 10%1$c%%define QUOTE 34%1$c%%define FN %2$cGrace_kid.s%2$c%1$c%1$c;Grace in asm%1$csection .data%1$cfname db FN, 0%1$cmode db %2$cw%2$c, 0%1$ccode db %2$c%3$s%2$c, 0%1$c%1$csection .text%1$cmain:%1$c    push rbx%1$c    lea rdi, [rel fname]%1$c    lea rsi, [rel mode]%1$c    call fopen%1$c    mov rbx, rax%1$c    mov rdi, rbx%1$c    lea rsi, [rel code]%1$c    mov rdx, NEWLINE%1$c    mov rcx, QUOTE%1$c    lea r8, [rel code]%1$c    xor eax, eax%1$c    call fprintf%1$c    mov rdi, rbx%1$c    call fclose%1$c    pop rbx%1$c    ret%1$c", 0

section .text
main:
    push rbx
    lea rdi, [rel fname]
    lea rsi, [rel mode]
    call fopen
    mov rbx, rax
    mov rdi, rbx
    lea rsi, [rel code]
    mov rdx, NEWLINE
    mov rcx, QUOTE
    lea r8, [rel code]
    xor eax, eax
    call fprintf
    mov rdi, rbx
    call fclose
    pop rbx
    ret
