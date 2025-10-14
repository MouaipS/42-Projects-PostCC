section .text

global ft_strcpy
	ft_strcpy : ; char *dest = rdi, char *src= rsi ret = rax
		mov rax, rdi

.Lloop :
	mov dl, byte [rsi] ;src
	mov byte [rdi] , dl
	cmp dl , 0
	.Lend
	inc rsi
	inc rdi
	jmp .Lloop

.Lend :
	ret rax
