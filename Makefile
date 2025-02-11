#Compile Options
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MSHFLAGS = -lreadline -ltinfo -lncurses -ltermcap

# Directories
LIBFT = includes/libft/libft.a
PRINTF = includes/ft_printf/libftprintf.a

#
SRC =	sources/main/main.c sources/main/init.c sources/main/readline.c sources/main/environ.c sources/trash.c \
		sources/main/free.c sources/main/heredoc.c \
		sources/signals/signals.c \
		sources/Parser_and_tokens/tokenizer.c sources/Parser_and_tokens/tokenizer_utils.c \
		sources/Parser_and_tokens/token_type.c sources/Parser_and_tokens/split_tokens.c sources/Parser_and_tokens/get_args.c \
		sources/builtins/env.c sources/builtins/exit.c sources/builtins/export.c sources/builtins/pwd.c \
		sources/builtins/unset.c sources/builtins/cd.c sources/builtins/echo.c sources/builtins/builtins_utils.c \
		sources/builtins/builtins_utils2.c sources/executer/executer.c sources/executer/redirections.c
#

# Commands
RM = rm -rf
OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o : %.c
			$(CC) $(CFLAGS) -c $*.c -o $*.o

$(LIBFT):
			@make bonus --silent -C includes/libft

$(PRINTF):
			@make --silent -C includes/ft_printf

$(NAME) : $(OBJ) $(PRINTF) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(PRINTF) $(MSHFLAGS)

clean:
			$(RM) $(OBJ)
			@make --silent -C includes/ft_printf clean
			@make --silent -C includes/libft clean
fclean: clean
			$(RM) $(NAME)
			@make --silent -C includes/ft_printf fclean
			@make --silent -C includes/libft  fclean

re: fclean all

.PHONY: all clean fclean re