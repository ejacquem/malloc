#include "../malloc.h"

#define PADDING "%6"

/*
	printf("┌──────┐\n");
	printf("│ MEM  │\n");
	printf("└──────┘\n");
*/

#define SAM_WIDTH 90

// #define SAM_PRINT(fmt, ...) int nb = ft_printf("│ " fmt, ##__VA_ARGS__,); ft_printf("%*s", 50-nb, "│");
#define SAM_PRINT(fmt, ...) do { \
    int nb = ft_printf("│ " fmt, ##__VA_ARGS__); \
	ft_printn(' ', SAM_WIDTH - nb - 3); \
    ft_printf("│\n"); \
} while (0)

#define SAM_HEADER "───── SHOWING ALLOCATION MEMORY "
#define SAM_PRINT_HEADER ft_printf("┌"SAM_HEADER); ft_printsn("─", SAM_WIDTH - 38); ft_printf("┐\n");
#define SAM_PRINT_BOTTOM ft_printf("└"); ft_printsn("─", SAM_WIDTH - 6); ft_printf("┘\n");

// void print_memory(size_t block_size)
// {
// 	ft_prints();
// 	ft_printn(" ", block_size / 8);
// }

//00000000 0000 0000 0000 0000 0000 0000 0000 0000 
// 1 byte 8 bits
// 0000 = 2 bytes

void print_block_hex(void *block, size_t alloc_size)
{
	block += S_META_DATA_SIZE;
	for (size_t i = 0; i < alloc_size;)
	{
		ft_printf ("%p ", block);
		unsigned char *byte = (char *)(block + i);
		// ft_printf ("%x ", byte);
		for(size_t j = 0; j < 16 && i < alloc_size; j+=2, i+=2)
		{
			unsigned char first = *byte++;
			unsigned char second = *byte++;
			ft_printf ("%02X%02X ", first, second);
		}
		ft_printf ("\n");
	}
	ft_printf("");
}

void print_block(void *block, size_t block_size, size_t alloc_size, int is_free)
{
	if (data.sam_format == BASIC)
    	ft_printf("%p - %p : %ld bytes\n\n", block, block + block_size, alloc_size);
	if (data.sam_format == CUSTOM)
		SAM_PRINT(" %p : "PADDING"ld| "PADDING"ld| %s", block, alloc_size, block_size, (is_free ? "free" : ""));
	if (data.sam_format == HEXDUMP)
		print_block_hex(block, alloc_size);
}

size_t count_large_zone(struct l_meta_data *zone)
{
	size_t count = 1;

	if (zone == NULL)
		return 0;

	while(zone->next)
	{
		zone = zone->next;
		count++;
	}
	return count;
}

void show_alloc_mem_ex()
{
    size_t sum = 0;
	data.sam_format = CUSTOM;

	SAM_PRINT_HEADER;

    if (data.tiny)
	{
		struct zone_data *zone = data.tiny;
		SAM_PRINT("TINY: %4ld alloc:", zone->alloc_count);
        print_zone(&sum, get_zone_usr_data_ptr(data.tiny), TINY_ZONE);
	}
		
    if (data.small)
	{
		struct zone_data *zone = data.small;
		SAM_PRINT("SMALL: %3ld alloc:", zone->alloc_count);
		print_zone(&sum, get_zone_usr_data_ptr(data.small), SMALL_ZONE);
	}
	
    if (data.large)
	{
		SAM_PRINT("LARGE: %3ld alloc:", count_large_zone(data.large));
        print_large(&sum, data.large);
	}
    SAM_PRINT("Total: %ld bytes", sum);

	SAM_PRINT_BOTTOM;
    return;
}