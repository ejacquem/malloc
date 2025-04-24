#include <stdio.h>

int main()
{
    int a = 4;

    void *ptr = &a;
    printf("%p\n", ptr);
    printf("%p\n", ptr + 1);
    printf("%p\n", ((char *)ptr) + 1);
    printf("%p\n", ++ptr);


    // for (int i = 0; i < 10000; i++)
    //     a = malloc(1025 * 1.7);
    // for (int i = 0; i < 10; i++)
    //     ft_printf("ceil(%d, %d): %d\n", i, 8, align_up(i, 8));
    // for (int i = 0; i < 5; i++)
    // {
    //     prev = a;
    //     a = malloc(1);
    //     ft_printf("pointer %2d: %x\n", i, a);
    //     if (i >= 1)
    //         ft_printf("diff:       %d\n", (size_t)a - (size_t)prev);
    //     ft_printf("malloc data: %x\n", (a - 8));
    // }
    // return 0;
}