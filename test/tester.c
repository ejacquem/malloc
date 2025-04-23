#include "../malloc.h"
#include "../printf/ft_printf.h"

int main()
{
    ft_printf("Hello\n");
    int *a;
    a = malloc(1);
    ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    a = malloc(1024);
    ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = malloc(63);
    // ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = malloc(64);
    // ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = malloc(512);
    // ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // a = malloc(1024);
    // ft_printf("malloc: %s\n", a == NULL ? "NULL" : "OK");
    // int *b = malloc2(1);
    // printf("malloc: %s\n", b == NULL ? "NULL" : "OK");FLUSH
}