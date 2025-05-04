#include "../malloc.h"

void print_zone(size_t *sum, void *zone, size_t zone_size)
{
    void *block;

    for (int i = 0; i < zone_size;)
    {
        block = zone + i;
        size_t block_data = get_block_data(block);
        size_t alloc_size = GET_SIZE(block_data);
        size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
        int is_free = IS_FREE(block_data);
        if (alloc_size == 0)
            break;
        print_block(block, block_size, alloc_size, is_free);
        *sum += alloc_size;
        
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
        size_t alloc_size = GET_SIZE(block_data.size);
        int is_free = IS_FREE(block_data.size);
        size_t block_size = get_block_size(block_data.size, L_META_DATA_SIZE);
        print_block(block, block_size, alloc_size, is_free);

        *sum += alloc_size;

        block = block_data.next;
    } while (block_data.next);
}

void show_alloc_mem()
{
	data.sam_format = BASIC;
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
    return;
}