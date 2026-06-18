NAME = Collen

all: $(NAME)

$(NAME): Colleen.s
	nasm -f elf64 Colleen.s -o Colleen.o
	ld Colleen.o -o $(NAME)

clean:
	rm -f Colleen.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re