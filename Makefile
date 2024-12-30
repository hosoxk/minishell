# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 15:08:03 by yde-rudd          #+#    #+#              #
#    Updated: 2024/12/30 16:18:20 by kvanden-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# project name and compilation settings
NAME         = minishell
DEBUG_NAME   = minishell_debug

CC           = cc
CFLAGS       = -g3 -Wall -Werror -Wextra
# additional flags for debug build
DEBUG_FLAGS  = -DDEBUG=1

PROJECTFLAGS = -lreadline
MAKEFLAGS   += -s

# directories
SRC_DIR      = ./sources
LIBFT_DIR    = ./sources/libft

# source and object files
SRC_FILES = $(SRC_DIR)/main.c \
		$(SRC_DIR)/lexer.c \
		$(SRC_DIR)/handler.c \
		$(SRC_DIR)/utils.c \
		$(SRC_DIR)/parsing.c \
		$(SRC_DIR)/clean.c \
		$(SRC_DIR)/create_nodes.c \
		$(SRC_DIR)/validate_tokens.c \
		$(SRC_DIR)/executer/execute_build_in_cmd.c \
		$(SRC_DIR)/executer/execute_custom_cmd.c \
		$(SRC_DIR)/executer/executer.c \
		$(SRC_DIR)/executer/my_echo.c \
		$(SRC_DIR)/executer/my_env.c \
		$(SRC_DIR)/executer/my_unset.c \
		$(SRC_DIR)/executer/my_export.c \
		$(SRC_DIR)/executer/rediraction.c
		#$(SRC_DIR)/pwd_builtin.c \
		$(SRC_DIR)/echo_builtin.c \
		$(SRC_DIR)/cd_builtin.c \
		$(SRC_DIR)/exit_builtin.c

OBJ_FILES        = $(SRC_FILES:.c=.o)
DEBUG_OBJ_FILES  = $(SRC_FILES:.c=.debug.o)

# libraries
LIBFT         = $(LIBFT_DIR)/libft.a

# define colors
MAGENTA = \033[1;35m
YELLOW  = \033[1;33m
GREEN   = \033[1;32m
RESET   = \033[0m

# ---------------------------------------------------------------------------- #
#                              Build Targets                                   #
# ---------------------------------------------------------------------------- #

all: $(LIBFT) $(NAME)

# Normal (non-debug) executable
$(NAME): $(OBJ_FILES) $(LIBFT)
	@echo "$(MAGENTA)Using compiler: $(CC)$(RESET)"
	@echo "$(MAGENTA)Using flags: $(CFLAGS) $(PROJECTFLAGS) $(RESET)"
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(PROJECTFLAGS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Compilation successful!$(RESET)"

# Debug executable
debug: $(LIBFT) $(DEBUG_NAME)

$(DEBUG_NAME): $(DEBUG_OBJ_FILES) $(LIBFT)
	@echo "$(MAGENTA)Using compiler: $(CC) for DEBUG build$(RESET)"
	@echo "$(MAGENTA)Using flags: $(CFLAGS) $(DEBUG_FLAGS) $(PROJECTFLAGS)$(RESET)"
	@echo "$(YELLOW)Linking $(DEBUG_NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(DEBUG_OBJ_FILES) $(PROJECTFLAGS) $(LIBFT) -o $(DEBUG_NAME)
	@echo "$(GREEN)Debug build successful!$(RESET)"

# Rule to build object files for normal build
%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -I $(SRC_DIR) -c $< -o $@

# Rule to build object files for debug build
%.debug.o: %.c
	@echo "$(YELLOW)Compiling DEBUG $<...$(RESET)"
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

# Libft build
$(LIBFT):
	@echo "$(YELLOW)Building libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)Libft compiled successfully!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@rm -f $(OBJ_FILES) $(DEBUG_OBJ_FILES)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Removing $(NAME) and $(DEBUG_NAME) executables...$(RESET)"
	@rm -f $(NAME) $(DEBUG_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)Executables removed!$(RESET)"

re: fclean all

# Build both non-debug and debug
both: all debug

.PHONY: all clean fclean re debug both
