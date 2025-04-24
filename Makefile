LIB_NAME = libft_malloc_$(HOSTTYPE).so
PRINTF = printf/libftprintf.a
LIBS = $(PRINTF)

CFLAGS = -g #-Wall -Werror -Wextra

SRC = src/malloc.c src/utils.c src/show_alloc_mem.c
TEST = test/test1.c
TESTER = tester
TESTER_STD = tester_std

OBJ = $(SRC:.c=.o)
TESTOBJ = $(TEST:.c=.o)

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

compile_malloc_test: $(TESTOBJ)
	@cc -g $(CFLAGS) -o $(TESTER_STD) $(TESTOBJ)

compile_test: all $(TESTOBJ)
	@cc -g $(CFLAGS) -o $(TESTER) $(TESTOBJ) $(LIBS)

test: compile_test
#	@./$(TESTER)
	LD_PRELOAD=./libft_malloc.so ./$(TESTER)

test_page: compile_test
#	LD_PRELOAD=./libft_malloc.so /usr/bin/time -v ./$(TESTER)
	LD_PRELOAD=./libft_malloc.so /usr/bin/time -v ./$(TESTER) 2>&1| grep -E "Page|CPU|faults|time"

test_std: compile_malloc_test
#	/usr/bin/time -v ./$(TESTER_STD)
	/usr/bin/time -v ./$(TESTER_STD) 2>&1| grep -E "Page|CPU|faults|time"

clean:
	rm -f $(OBJ) $(TESTOBJ) libft_malloc.so

fclean: clean
	rm -f $(LIB_NAME) $(TESTER)

deepclean: fclean
	make fclean -C printf
	rm -f $(LIB_NAME) $(TESTER)

re: fclean all
