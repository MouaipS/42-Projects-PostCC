section .text

global _start
_start:
	mov rax, 1
	mov rdi, 1
	mov rsi, my_text
	mov rdx, 4
	syscall
	mov rax, 60
	mov rdi, 0
	syscall


section .data
	my_text: db "Text", 


