section .text

global ft_strlen ; ft_strlen(char *s)
	; s = rdi ret = rax

ft_strlen :
	mov al, byte [rdi]
	cmp al, al
	je .Lend
	inc rdi
	jmp .Lloop 

.Lend :
	ret 