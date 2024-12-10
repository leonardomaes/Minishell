#Compile Options
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MSHFLAGS = -lreadline -ltinfo -lncurses -ltermcap

# Directories
LIBFT = includes/libft/libft.a
PRINTF = includes/ft_printf/libftprintf.a

#
SRC =	sources/main.c sources/init.c sources/readline.c sources/environ.c sources/trash.c \
		sources/free.c sources/Parser_and_tokens/parser.c sources/Parser_and_tokens/tokenizer.c \
		sources/builtins/env.c sources/builtins/exit.c sources/builtins/export.c sources/builtins/pwd.c \
		sources/builtins/unset.c sources/builtins/cd.c sources/builtins/echo.c \
		sources/Parser_and_tokens/token_type.c sources/signals.c
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
