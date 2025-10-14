section .text

global ft_strcmp 
	ft_strcmp : ;rdi = s1 rsi = s2 ret = eax (rax techniquement)

.Lloop :
	mov al, byte [rdi]
	mov dl, byte [rsi]
	cmp al, dl
	jne .Ldiff
	cmp al, 0
	je .Lequal
	inc rdi
	inc rsi
	jmp .Lloop

.Ldiff :
	movsx eax , al
	movsx edx , dl
	sub eax , edx
	ret 
  
.Lequal :
	mov eax, 0
	ret