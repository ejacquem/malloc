# CC = gcc
# CFLAGS     = -Wall -Wextra -Werror #-Ilibft #-no-pie

# NAME       = libftprintf.a

# SRCS       = ft_printf.c ft_print.c utils.c utils2.c
# OBJS       = $(SRCS:.c=.o)

# all: $(NAME)

# bonus: all

# $(NAME): $(OBJS)
# 	ar rcs --thin $@ $^ libft/libft.a
# #ar crsT libab.a liba.a libb.a

# clean:
# 	$(RM) $(OBJS)

# fclean: clean
# 	$(RM) $(NAME)

# re: fclean all

# .PHONY: all clean fclean re

NAME		= libftprintf.a

CFLAGS		= -fPIC -Wall -Werror -Wextra

SRCS		= ft_printf.c ft_print.c utils.c utils2.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	make -C libft
	cp libft/libft.a $(NAME)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re