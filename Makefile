# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/12 15:52:26 by rda-cunh          #+#    #+#              #
#    Updated: 2025/02/12 16:39:48 by rda-cunh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compile Options
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MSHFLAGS = -lreadline -ltinfo -lncurses -ltermcap

# Directories
LIBFT = includes/libft/libft.a
PRINTF = includes/ft_printf/libftprintf.a

# Sources
SRC =	sources/main/main.c sources/main/readline.c \
		sources/main/syntax_check.c sources/main/environ.c \
		sources/exit_free/free.c sources/exit_free/free2.c\
		sources/executer/heredoc.c sources/executer/heredoc_expander.c \
		sources/executer/executer.c \
		sources/executer/executer_pipes.c sources/executer/executer_pipes2.c \
		sources/executer/executer_utils.c sources/executer/redirections.c \
		sources/executer/redirections2.c \
		sources/signals/signals.c sources/signals/signals2.c \
		sources/parser_and_tokens/tokenizer.c \
		sources/parser_and_tokens/tokenizer_utils.c \
		sources/parser_and_tokens/tokenizer_utils2.c \
		sources/parser_and_tokens/token_type.c \
		sources/parser_and_tokens/split_tokens.c \
		sources/parser_and_tokens/split_tokens_utils.c \
		sources/parser_and_tokens/calculate_lengths.c \
		sources/parser_and_tokens/count_args.c \
		sources/parser_and_tokens/get_args.c \
		sources/parser_and_tokens/get_args2.c \
		sources/builtins/env.c sources/builtins/exit.c \
		sources/builtins/export.c sources/builtins/pwd.c \
		sources/builtins/unset.c sources/builtins/cd.c \
		sources/builtins/echo.c sources/builtins/builtins_utils.c \
		sources/builtins/builtins_utils2.c \
		sources/builtins/builtins_utils3.c

# Valgrind readline leaks ignore vars
RUN_MS 			= valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
					--suppressions=.ignore_readline -q ./$(NAME)
VAL_SUPP		= .ignore_readline

# Commands
RM = rm -rf
OBJ = $(SRC:.c=.o)

# Rules
all: $(NAME)

%.o : %.c
			$(CC) $(CFLAGS) -c $*.c -o $*.o

$(LIBFT):
			@make bonus --silent -C includes/libft

$(PRINTF):
			@make --silent -C includes/ft_printf

$(NAME) : $(OBJ) $(PRINTF) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(PRINTF) $(MSHFLAGS)

$(VAL_SUPP):
		@echo "{" > $(VAL_SUPP)
		@echo "   ignore_libreadline_conditional_jump_errors" >> $(VAL_SUPP)
		@echo "   Memcheck:Leak" >> $(VAL_SUPP)
		@echo "   ..." >> $(VAL_SUPP)
		@echo "   obj:*/libreadline.so.*" >> $(VAL_SUPP)
		@echo "}" >> $(VAL_SUPP)


leaks:	$(NAME) $(VAL_SUPP)
		$(RUN_MS)

clean:
			$(RM) $(OBJ)
			@make --silent -C includes/ft_printf clean
			@make --silent -C includes/libft clean
fclean: clean
			$(RM) $(NAME)
			$(RM) $(VAL_SUPP)
			@make --silent -C includes/ft_printf fclean
			@make --silent -C includes/libft  fclean

re: fclean all

.PHONY: leaks all clean fclean re