NAME     = Grace
ASM      = nasm
ASMFLAGS = -f elf64
CC       = gcc
CFLAGS   = -no-pie -Wall -Werror -Wextra

all: $(NAME)

$(NAME): Grace.o
	$(CC) $(CFLAGS) Grace.o -o $(NAME)

Grace.o: Grace.s
	$(ASM) $(ASMFLAGS) Grace.s -o Grace.o

clean:
	rm -f Grace.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re