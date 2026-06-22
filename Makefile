ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

CC = gcc 
CFLAGS = -Wall -Werror -Wextra 

SRC_DIR = src
OBJ_DIR = obj

INCLUDES = -I includes

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFT) $(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS)
	ln -sf $(NAME) $(LINK)

clean:
	rm -rf $(OBJDIR)
	make -C clean

fclean: clean
	rm -f $(NAME) $(LINK)
	make -C fclean

re: fclean all

.PHONY: all clean fclean re