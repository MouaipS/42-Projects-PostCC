NAME = ft_nm
NAME_BONUS = ft_nm_bonus

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I include

SRC_DIR = src
SRC_BONUS_DIR = src_bonus

OBJ_DIR = obj
OBJ_BONUS_DIR = obj_bonus

SRC =	main.c \
		process_64.c \
		process_32.c \
		setup.c \
		utils.c

SRC_BONUS =	main_bonus.c \
			process_64_bonus.c \
			process_32_bonus.c \
			setup_bonus.c \
			utils_bonus.c

OBJS = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
OBJS_BONUS = $(addprefix $(OBJ_BONUS_DIR)/,$(SRC_BONUS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_BONUS_DIR):
	mkdir -p $(OBJ_BONUS_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(NAME_BONUS)

clean:
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re