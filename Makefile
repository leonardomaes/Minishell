#Compile Options
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MSHFLAGS = -lreadline -ltinfo -lncurses -ltermcap

# Directories
LIBFT = includes/libft/libft.a
PRINTF = includes/ft_printf/libftprintf.a

#
SRC =	sources/main.c sources/init.c sources/readline.c sources/trash.c
#

# Commands
RM = rm -rf
OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o : %.c
			$(CC) $(CFLAGS) -c $*.c -o $*.o

$(LIBFT):
			@make --silent -C includes/libft

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
