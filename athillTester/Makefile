NAME = malloc_tester
NAME_STD = malloc_tester_std
LIBFT_PATH = ../printf
MALLOC_PATH = ../
LIBFT_A = ../printf/libftprintf.a
EXTR_FLAGS = -O3

CC = cc
CFLAGS = $(EXTR_FLAGS) -Wall -Wextra -Werror -Iinclude -I$(LIBFT_PATH) -I$(MALLOC_PATH)

SRCS = src/main.c \
	   src/tests.c \

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

OBJS_STD = $(SRCS:.c=_std.o)
DEPS_STD = $(SRCS:.c=_std.d)

LIBS = $(LIBFT_A) ../libft_malloc.so

all: $(NAME)

malloc:
	MALLOC_EXTRA_FLAGS="$(EXTR_FLAGS)" $(MAKE) -C ..

$(NAME): malloc $(OBJS) $(OBJS_STD)
	$(CC) $(OBJS) -o $(NAME) $(LIBS)
	$(CC) $(OBJS_STD) -o $(NAME_STD) $(LIBFT_A)

clean:
	rm -f $(OBJS) $(DEPS) $(OBJS_STD) $(DEPS_STD)

fclean: clean
	rm -f $(NAME) $(NAME_STD)

re: fclean all

-include $(DEPS)
-include $(DEPS_STD)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -D MY_MALLOC -MMD -MP -c $< -o $@

%_std.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
