LIB_NAME = libft_malloc_$(HOSTTYPE).so
PRINTF = printf/libftprintf.a
LIBS = $(PRINTF)

CFLAGS = -g #-Wall -Werror -Wextra

SRC = malloc.c
TEST = test/tester.c
TESTER = tester

OBJ = $(SRC:.c=.o)
TESTOBJ = $(OBJ) $(TEST:.c=.o)

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

all: $(LIB_NAME) sym

$(LIB_NAME): $(OBJ)
	make -C printf
	cc $(CFLAGS) -fPIC -shared -o $(LIB_NAME) $(OBJ) $(LIBS)

%.o: %.c
	@cc $(CFLAGS) -fPIC -c $< -o $@

sym:
	ln -sf $(LIB_NAME) libft_malloc.so

# TESTER -----------------------------------

test: all $(TESTOBJ)
	@cc -g $(CFLAGS) -o $(TESTER) $(TESTOBJ) $(LIBS)

runtest: test
#	@./tester
#	LD_PRELOAD=./libft_malloc.so ./tester
	LD_PRELOAD=./libft_malloc.so valgrind ./tester
#	/usr/bin/time -v ./tester
#	/usr/bin/time -v ./tester 2>&1| grep -E "Page|CPU|faults"

clean:
	rm -f $(OBJ) $(TESTOBJ) libft_malloc.so

fclean: clean
	rm -f $(LIB_NAME) $(TESTER)

deepclean: fclean
	make fclean -C printf
	rm -f $(LIB_NAME) $(TESTER)

re: fclean all
