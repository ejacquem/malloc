#include "../ft_malloc.h"
#include "../printf/ft_printf.h"

int main()
{
    // ft_printf("Hello\n");
    // a = ft_malloc(1024);
    // ft_printf("ft_malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = ft_malloc(2147483649 * 9.6);

    int *a;
    a = ft_malloc(1024);
    ft_free(a);
    a = ft_malloc(1024);
    ft_free(a);
    a = ft_malloc(1024);
    ft_free(a);
    a = ft_malloc(1024);
    ft_free(a);

    a = ft_malloc(1024);
    a = ft_malloc(1024);
    a = ft_malloc(1024);
    // for (int i = 0; i < 10; i++)
    // {
    //     a = ft_malloc(1);
    //     ft_printf("ft_malloc: %s\n", a == NULL ? "NULL" : "OK");
    // }
    // ft_printf("Hello\n");

    // show_alloc_mem();
}