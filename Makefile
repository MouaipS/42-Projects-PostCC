NAME_BASE = libft_malloc

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = $(NAME_BASE)_$(HOSTTYPE).so
LINK = $(NAME_BASE).so

CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC

SRC_DIR = src
OBJ_DIR = obj

INCLUDES = -I includes

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)
	ln -sf $(NAME) $(LINK)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LINK)

re: fclean all

.PHONY: all clean fclean re