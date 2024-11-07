SRCS = 	ft_printf.c \
		print_char.c \
		print_hexa.c \
		print_base.c \
		print_nbr.c \
		print_str.c \
		rever_hexa.c \
		print_unsigned.c \

OBJS = $(SRCS:.c=.o)

NAME = libftprintf.a
LIBFTPATH = ../libft
LIBFT = ../libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
			$(CC) $(CFLAGS) -c $(SRCS)
			$(AR) $(NAME) $(OBJS)

$(LIBFT):
			make -C ../libft
			cp ../libft/libft.a .
			mv libft.a $(NAME)
			
clean:
			$(RM) $(OBJS)
			make -C ../libft clean

fclean: clean
		$(RM) $(NAME)
		make -C ../libft fclean

re: fclean all

.PHONY:	all clean fclean re
