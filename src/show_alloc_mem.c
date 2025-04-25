#include "../malloc.h"

void print_zone(size_t *sum, void *zone, size_t zone_size)
{
    void *block;

    for (int i = 0; i < zone_size;)
    {
        block = zone + i;
        size_t block_data = get_block_data(block);
        size_t block_size = GET_SIZE(block_data);
        ft_printf("%p - %p : %ld bytes\n", block, block + block_size, block_size);
        *sum += block_size;
        
        if (block_size == 0)
            break;
        i += block_size;
    }
}

void print_large(size_t *sum, void *zone)
{
    void *block;
    struct l_meta_data block_data;

    block = zone;
    if (block == NULL)
        return;

    do
    {
        block_data = get_block_meta_data(block);
        size_t block_size = GET_SIZE(block_data.size);
        ft_printf("%p - %p : %ld bytes\n", block, block + block_size, block_size);

        *sum += block_size;

        block = block_data.next;
    } while (block_data.next);
}

void show_alloc_mem()
{
    size_t sum = 0;

    ft_printf("TINY : %p\n", data.tiny);
    if (data.tiny)
        print_zone(&sum, get_zone_usr_data_ptr(data.tiny), TINY_ZONE);
    ft_printf("SMALL: %p\n", data.small);
    if (data.small)
        print_zone(&sum, get_zone_usr_data_ptr(data.small), SMALL_ZONE);
    ft_printf("LARGE: %p\n", data.large);
    if (data.large)
        print_large(&sum, data.large);
    ft_printf("Total: %ld bytes\n", sum);
    ft_printf("Total allocated: %ld bytes\n", data.data_allocated_count);
    ft_printf("Total freed: %ld bytes\n", data.data_freed_count);
    return;
}