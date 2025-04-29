# CFLAGS = -Wno-implicit-function-declaration
CC				= gcc
RM				= rm -f
CFLAGS			= -g -Wall -Wextra -Werror

SRC = memset bzero memcpy memmove memchr memcmp calloc \
	strdup substr strjoin strtrim split itoa strmapi \
	striteri putchar_fd putstr_fd putendl_fd putnbr_fd \
	strlen strchr strrchr strncmp strlcpy strlcat strnstr \
	tolower toupper atoi is
FT_SRC = $(addprefix ft_, $(addsuffix .c, $(SRC)))
LST = lstnew lstadd_front lstiter lstsize lstmap
FT_LST = $(addprefix ft_, $(addsuffix .c, $(LST)))
SRCS = $(FT_SRC)
SRCSALL = ${SRCS} ${FT_LST}
OBJS = $(SRCS:.c=.o)
OBJSALL = ${SRCSALL:.c=.o}
NAME = libft.a

all: ${NAME} 

${NAME}: $(OBJS)
	ar rcs $@ $^

bonus: ${OBJSALL}
	ar rsc libft.a $^

re: fclean all

clean:
	rm -f *.o

fclean: clean
	rm -f libft.a

# so:
# 	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCSALL)
# 	gcc -nostartfiles -shared -o libft.so $(OBJSALL)

.PHONY : all bonus clean fclean re
