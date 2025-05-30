LIB_NAME = libft_malloc_$(HOSTTYPE).so
PRINTF = printf/libftprintf.a
LIBS = $(PRINTF)

CFLAGS = -g #-Wall -Werror -Wextra 

SRC = src/malloc.c src/free.c src/realloc.c src/utils.c src/show_alloc_mem.c src/show_alloc_mem_ex.c

OBJ = $(SRC:.c=.o)

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

TEST_NAME := $(filter test%,$(MAKECMDGOALS))

SHORT_CMD := ./tester
ifeq ($(SHORT),1)
    SHORT_CMD := ./tester 2>&1 | grep -E "Page|CPU|faults|time"
endif

TIME_CMD := $(SHORT_CMD)
ifeq ($(TIME),1)
    TIME_CMD := /usr/bin/time -v $(SHORT_CMD)
endif

$(TEST_NAME): all
	@echo "Building and running $@"
ifeq ($(STD),1)
	cc -Wno-unused-result $(CFLAGS) -o tester test/$@.c
	$(TIME_CMD)
else
	cc -Wno-unused-result $(CFLAGS) -o tester test/$@.c $(LIBS) ./$(LIB_NAME)
	LD_PRELOAD=./libft_malloc.so $(TIME_CMD)
endif

help:
	@echo ""
	@echo "===================== Malloc Help ====================="
	@echo ""
	@echo "Usage:"
	@echo "  LD_PRELOAD=./libft_malloc.so [your_executable]     Run your program with custom malloc"
	@echo ""
	@echo "  make [testname] [STD=1] [TIME=1] [SHORT=1]         Run test with optional flags, tests must be in test/ folder"
	@echo ""
	@echo "Flags:"
	@echo "  STD=1        Use standard library malloc instead of custom one"
	@echo "  TIME=1       Prepend test execution with 'time' command"
	@echo "  SHORT=1      Short time output"
	@echo ""
	@echo "Env:"
	@echo "  MALLOC_DEBUG=1      Enable debug log"
	@echo "  MALLOC_HEXDUMP=1    Enable hexdump for the show_alloc_mem_ex() function"
	@echo ""
	@echo "Example:"
	@echo "  make test1 STD=1 TIME=1 SHORT=1"
	@echo "  make tester"
	@echo ""
	@echo "======================================================"

clean:
	rm -f $(OBJ) $(TESTOBJ) libft_malloc.so

fclean: clean
	rm -f $(LIB_NAME) tester

deepclean: fclean
	make fclean -C printf

re: fclean all
