# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/22 13:38:18 by aperez-b          #+#    #+#              #
#    Updated: 2021/11/30 16:14:22 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ECHO = echo

AR = ar rcs
CFLAGS = -Wall -Wextra -Werror -MMD
RM = rm -f
CC = gcc
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = libft/
LIBFT = libft/bin/libft.a
NAME = minishell

SRC = main.c builtins.c ft_strtrim_all.c exec.c			\
	  fill_node.c get_params.c ft_cmdtrim.c				\
	  expand.c heredoc.c error.c env.c custom_cmd.c		\
	  get_next_line.c get_next_line_utils.c prompt.c	\
	  ft_cmdsubsplit.c signal.c parse_args.c get_cmd.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): create_dirs $(OBJ)
	@make -C $(LIB_DIR)
	@$(CC) -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@
	@$(ECHO) "$(NAME) is up to date!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(ECHO) "Compiling $<..."
	@$(CC) -I ~/.brew/opt/readline/include $(CFLAGS) -c $< -o $@

create_dirs:
	@mkdir -p $(OBJ_DIR)

clean:
	@$(ECHO) "Cleaning up object files in $(NAME)..."
	@make -C $(LIB_DIR) clean
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(ECHO) "Removed $(NAME)"
	@make -C $(LIB_DIR) fclean
	@$(RM) $(NAME)

re: fclean all
	@$(ECHO) "Cleaned and Rebuilt Everything for $(NAME)!"


-include $(OBJ_DIR)/*.d

.PHONY: all clean fclean create_dirs re
