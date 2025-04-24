#include "my_malloc_tester.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    const char string[] = "Hello World!";
    const char string2[] = "12345";

    if (argc == 1) {
        ft_printf("Usage: ./malloc_tester [number of allocations]\n");
        ft_printf("    or ./malloc_tester [alloc size] [alloc count]\n");
        return 1;
    }
    if (argc == 3) {
        return run_test(atoi(argv[1]), atoi(argv[2]));
    }

    int alloced = 0;
    int n = atoi(argv[1]);
    for (int i = 0; i < n; ++i) {
        errno = 0;
        alloced += sizeof(string2);
        void *ptr2 = malloc(sizeof(string2));
        ft_printf("1 got ptr %p %s\n", ptr2, strerror(errno));
        if (ptr2 != NULL) {
            ft_memcpy(ptr2, string2, sizeof(string2));
            ft_printf("wrote '%s'\n", ptr2);
        }

        errno = 0;
        alloced += sizeof(string);
        void *ptr = malloc(sizeof(string));
        ft_printf("2 got ptr %p %s\n", ptr, strerror(errno));
        if (ptr != NULL) {
            ft_memcpy(ptr, string, sizeof(string));
            ft_printf("wrote '%s'\n", ptr);
        }

        //free(ptr2); alloced -= sizeof(string2);

        errno = 0;
        alloced += sizeof(string);
        ptr = malloc(sizeof(string));
        ft_printf("3 got ptr %p %s\n", ptr, strerror(errno));
        if (ptr != NULL) {
            ft_memcpy(ptr, string, sizeof(string));
            ft_printf("wrote '%s'\n", ptr);
        }

        // errno = 0;
        // alloced += sizeof(string);
        // ptr = realloc(ptr, sizeof(string) * 2);
        // ft_printf("3 realloc ptr %p %s\n", ptr, strerror(errno));
        // if (ptr != NULL) {
        //     ft_printf("still has '%s'\n", ptr);
        // }
    }
    printf("alloced mem: %i bytes\n", alloced);

// #ifdef MY_MALLOC
    // ft_printf("\n=== show_alloc_mem ===\n");
    // show_alloc_mem();
// #endif

    return 0;
}
