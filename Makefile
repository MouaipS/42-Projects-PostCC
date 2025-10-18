C       = gcc
CFLAGS  = -Wall -Werror -Wextra -fPIC
ASM     = nasm
ASMFLAGS= -f elf64
NAME    = libasm.a
OBJ_DIR = objs

SRC = main.c \
	ft_read.s \
	ft_strcmp.s \
	ft_strcpy.s \
	ft_strdup.s \
	ft_strlen.s \
	ft_write.s

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
OBJS := $(OBJS:.s=.o)

GREEN    = \033[0;32m
YELLOW   = \033[0;33m
RED      = \033[0;31m
BLUE     = \033[0;34m
RESET    = \033[0m
OK       = ✅
CLEAN    = 🧹
BUILD    = 🔨


all: $(NAME)

$(NAME): $(OBJS) | $(OBJ_DIR)
	@echo "$(BLUE)$(BUILD) Creating archive...$(RESET)"
	@ar rcs $(NAME) $(OBJS)
	@echo "$(GREEN)$(OK) Archive created: $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "$(YELLOW)Compiling C file: $< $(RESET)"
	@$(C) $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	@echo "$(YELLOW)Assembling ASM file: $< $(RESET)"
	@$(ASM) $(ASMFLAGS) $< -o $@


$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(CLEAN) $(YELLOW)Objects removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CLEAN) $(RED)Archive removed$(RESET)"

re: fclean all


run: $(NAME)
	@echo "$(BLUE)🚀 Running program...$(RESET)"
	@./test_prog

ccproject :
	$(CC) $(CFLAGS) main.c $(NAME_LIB)

.PHONY: all clean fclean re run ccproject
