#include "../malloc.h"

#define PADDING "%12"

void print_block(void *block, void *zone)
{
	size_t block_data = get_block_data(block);
	size_t alloc_size = GET_SIZE(block_data);
	size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
	// subject format
	// ft_printf("%p - %p : %ld bytes\n", block, block + block_size, alloc_size);

	// custom format
	void *start_ptr = block;
	void *end_ptr = (block + block_size);
	size_t is_free = IS_FREE(block_data);
	ft_printf("%p - %p : "PADDING"ld| "PADDING"ld| %s\n", start_ptr, end_ptr, alloc_size, block_size, (is_free ? "free" : ""));
}

void print_zone(size_t *sum, void *zone, size_t zone_size)
{
	void *block;
	//         [0x7f5087009010 - 0x7f5087009270] 
	ft_printf("   start ptr   -     end ptr    : "PADDING"s| "PADDING"s| \n", "alloc bytes", "block size");
    for (int i = 0; i < zone_size;)
    {
        block = zone + i;
        size_t block_data = get_block_data(block);
        size_t alloc_size = GET_SIZE(block_data);
        size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
        if (alloc_size == 0)
            break;
        // ft_printf("%p - %p : %ld bytes\n", block, block + block_size, alloc_size);
		print_block(block, zone);
        *sum += block_size;
        
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
        size_t block_size = get_block_size(block_data.size, S_META_DATA_SIZE);
        ft_printf("%p - %p : %ld bytes\n", block, block + block_size, alloc_size);

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
    // ft_printf("Total allocated: %ld bytes\n", data.data_allocated_count);
    // ft_printf("Total freed: %ld bytes\n", data.data_freed_count);
    return;
}