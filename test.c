#include <stdio.h>
#include <stdlib.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

int main()
{
    void *a, *b;
    // void *prev;

    // printf("double : %ld\n", sizeof(double));
    // printf("long double : %ld\n", sizeof(long double));
    // printf("long double : %ld\n", sizeof(long long));
    // printf("long double : %ld\n", sizeof(long));
    // for (int i = 0; i < 10; i++)
    // {
    //     prev = a;
    //     a = malloc(41);
    //     printf("a : %p\n", a);
    //     printf("diff : %ld\n", (a - prev));
    // }
    int n = 1000000;
    a = malloc(n);
    b = malloc(n);
    printf("a : %p\n", a);
    printf("b : %p\n", b);
    printf("diff : %ld\n", (b - a));
    free(a);
    free(b);

    a = malloc(n);
    printf("ptr : %p\n", (size_t *)(a));
    printf("ptr : %p\n", (size_t *)(a) - 1);
    printf("*ptr : %ld\n", *((size_t *)(a) - 1));
    char value = *((size_t *)(a) - 1);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(value));
    free(a);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(value));
    a = malloc(n);
    printf("ptr : %p\n", (size_t *)(a));
    printf("ptr : %p\n", (size_t *)(a) - 1);
    printf("*ptr : %ld\n", *((size_t *)(a) - 1));
    value = *((size_t *)(a) - 1);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(value));
    free(a);
    printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(value));
    // printf("*ptr : %b\n", *((size_t *)(a) - 1));
}