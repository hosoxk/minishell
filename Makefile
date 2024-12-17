# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yde-rudd <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 15:08:03 by yde-rudd          #+#    #+#              #
#    Updated: 2024/12/17 17:33:52 by yde-rudd         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#project name and compilation settings
NAME = minishell
CC = cc
CFLAGS = -g3 -Wall -Werror -Wextra
PROJECTFLAGS = -lreadline
MAKEFLAGS += -s

#directories
SRC_DIR = ./sources
LIBFT_DIR = ./sources/libft

#source and object files
SRC_FILES = $(SRC_DIR)/main.c \
		$(SRC_DIR)/lexer.c \
		$(SRC_DIR)/handler.c \
		$(SRC_DIR)/utils.c
		#$(SRC_DIR)/pwd_builtin.c \
		$(SRC_DIR)/echo_builtin.c \
		$(SRC_DIR)/cd_builtin.c \
		$(SRC_DIR)/exit_builtin.c

OBJ_FILES = $(SRC_FILES:.c=.o)

#libraries
LIBFT = $(LIBFT_DIR)/libft.a

#define colors
MAGENTA = \033[1;35m
YELLOW = \033[1;33m
GREEN = \033[1;32m
RESET = \033[0m

#build targets
all: $(LIBFT) $(NAME)

#link executables
$(NAME): $(OBJ_FILES) $(LIBFT)
	@echo "$(MAGENTA)Using compiler: $(CC)$(RESET)"
	@echo "$(MAGENTA)Using flags: $(CFLAGS) $(PROJECTFLAGS) $(RESET)"
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(PROJECTFLAGS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Compilation successfull!$(RESET)"

%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(YELLOW)Building libft...$(RESET)"
	$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)Libft compiled succesfully!$(RESET)"

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	rm -fr $(OBJ_FILES)
	$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Removing $(NAME) executable...$(RESET)"
	rm -fr $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)Executable $(NAME) removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
