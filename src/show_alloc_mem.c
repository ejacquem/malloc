#include "malloc.h"
#include "../malloc.h"

extern struct malloc_data data;

void print_zone(size_t *sum, void *zone, size_t zone_size)
{
    void *block;

    for (int i = 0; i < zone_size;)
    {
        block = zone + i;
        size_t block_size = get_block_size(block);
        ft_printf("%p - %p : %d bytes\n", block, block + block_size, block_size);

        *sum += block_size;
        
        i += block_size;
    }
}

void print_large(size_t *sum, void *zone)
{
    void *block;
    struct meta_data block_data;

    block = zone;
    if (block == NULL)
        return;

    do
    {
        block_data = get_block_meta_data(block);
        ft_printf("%p - %p : %d bytes\n", block, block_data.next, block_data.size);

        *sum += block_data.size;

        block = block_data.next;
    } while (block_data.next);
}

void show_alloc_mem()
{
    size_t sum = 0;

    ft_printf("TINY : %p\n", data.tiny);
    if (data.tiny)
        print_zone(&sum, data.tiny, TINY_ZONE);
    ft_printf("SMALL : %p\n", data.small);
    if (data.small)
        print_zone(&sum, data.small, SMALL_ZONE);
    ft_printf("LARGE : %p\n", data.large);
    if (data.large)
        print_large(&sum, data.large);
    ft_printf("Total remaining: %ld bytes\n", sum);
    ft_printf("Total allocated: %ld bytes\n", data.data_allocated_count);
    ft_printf("Total freed: %ld bytes\n", data.data_freed_count);
    return;
}