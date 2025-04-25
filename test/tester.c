#include "../malloc.h"
#include "../printf/ft_printf.h"

int main()
{
    // ft_printf("Hello\n");
    // a = malloc(1024);
    // ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = malloc(2147483649 * 9.6);

    int *a;
    a = malloc(1024);
    free(a);
    // for (int i = 0; i < 10; i++)
    // {
    //     a = malloc(1);
    //     ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // }
    // ft_printf("Hello\n");

    show_alloc_mem();
}