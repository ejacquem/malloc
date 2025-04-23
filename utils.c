#include "malloc.h"

void print_memory_table(int size, int meta_data_size)
{
    int block_size, min_zone_size, zone_size, block_nb, page_nb;

    block_size = size + meta_data_size;
    min_zone_size = block_size * 100;
    zone_size = align_up(min_zone_size, 4096);
    block_nb = zone_size / block_size;
    page_nb = zone_size / 4096;
    ft_printf("%5d|%11d|%14d|%10d|%9d|%8d\n", size, block_size, min_zone_size, zone_size, block_nb, page_nb);
}

void print_memory_tables()
{
    ft_printf("%5s|%11s|%14s|%10s|%9s|%8s\n", "size", "block_size", "min_zone_size", "zone_size", "block_nb", "page_nb");
    print_memory_table(16, 16);
    print_memory_table(32, 16);
    print_memory_table(64, 16);
    print_memory_table(128, 16);
    print_memory_table(256, 16);
    print_memory_table(512, 16);
    print_memory_table(1024, 16);
    // for(int i = 0; i < 64; i++)
    //     print_memory_table(i * 16, 16);
    ft_printf("\n");
}

void print_define()
{
    ft_printf("TINY_BLOCK_SIZE: %d bytes\n", TINY_BLOCK_SIZE);
    ft_printf("TINY_ZONE: %d bytes\n", TINY_ZONE);
    ft_printf("TINY_PAGE_NB: %d pages\n\n", TINY_PAGE_NB);
    ft_printf("SMALL_BLOCK_SIZE: %d bytes\n", SMALL_BLOCK_SIZE);
    ft_printf("SMALL_ZONE: %d bytes\n", SMALL_ZONE);
    ft_printf("SMALL_PAGE_NB: %d pages\n", SMALL_PAGE_NB);
}