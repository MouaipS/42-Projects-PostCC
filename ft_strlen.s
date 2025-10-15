section .text

global ft_strlen ; ft_strlen(char *s)
    ; s = rdi ret = rax

ft_strlen:
    mov eax, 0

.Lloop:
    mov bl, byte [rdi]

    cmp bl, 0
    je .Lend
    inc rdi
    inc eax
    jmp .Lloop 

.Lend:
    ret