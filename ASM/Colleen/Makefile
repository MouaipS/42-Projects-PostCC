#NAME = Collen

#all: $(NAME)

#$(NAME): Colleen.s
#	nasm -f elf64 Colleen.s -o Colleen.o
#	ld Colleen.o -o $(NAME)

#clean:
#	rm -f Colleen.o

#fclean: clean
#	rm -f $(NAME)

#re: fclean all

#.PHONY: all clean fclean re

NAME     = Colleen
ASM      = nasm
ASMFLAGS = -f elf64
CC       = gcc
CFLAGS   = -no-pie -Wall -Werror -Wextra

all: $(NAME)

$(NAME): Colleen.o
	$(CC) $(CFLAGS) Colleen.o -o $(NAME)

Colleen.o: Colleen.s
	$(ASM) $(ASMFLAGS) Colleen.s -o Colleen.o

clean:
	rm -f Colleen.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re