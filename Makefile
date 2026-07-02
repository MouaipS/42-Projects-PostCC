NAME        = ft_ls
NAME_BONUS  = ft_ls_bonus

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I includes

SRC_DIR     = srcs
SRC_BONUS_DIR = srcs_bonus
OBJ_DIR     = obj
OBJ_BONUS_DIR = obj_bonus

LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)libft.a

SRC =   main.c \
        parse_flags.c \
        parse_paths.c

#SRC_BONUS = \

OBJS        = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
OBJS_BONUS  = $(addprefix $(OBJ_BONUS_DIR)/,$(SRC_BONUS:.c=.o))

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBFT) -o $@

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) $^ $(LIBFT) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_BONUS_DIR):
	mkdir -p $(OBJ_BONUS_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(LIBFT) $(NAME_BONUS)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re