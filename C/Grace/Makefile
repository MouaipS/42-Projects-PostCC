CC = cc
NAME = Grace

OBJ_DIR = obj/

SRC = Grace.c

OBJ = $(SRC:.c=.o)

SRCS = $(SRC)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

CFLAGS = -Wall -Wextra -Werror -I incl

all:$(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p obj

obj/%.o : %.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME)
	rm -rf -f $(OBJ_DIR)

clean:
	rm -f $(OBJS)
	rm -rf -f $(OBJ_DIR)

re: fclean all

.PHONY: all fclean clean re
