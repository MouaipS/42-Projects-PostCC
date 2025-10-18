section .text
global ft_write
extern __errno_location

ft_write: ;
	mov rax, 1
	syscall
	cmp rax, 0
	jl .error
	ret

.error:
	call __errno_location
	neg rax
	mov [rax], eax
	mov rax, 1
	ret