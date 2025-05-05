#include "../malloc.h"

#define PADDING "%6"

/*
	printf("┌──────┐\n");
	printf("│ MEM  │\n");
	printf("└──────┘\n");
*/

#define SAM_WIDTH 90

#define SAM_PRINT_END(nb) ft_printn(' ', SAM_WIDTH - nb - 3); ft_printf("│\n");
// #define SAM_PRINT(fmt, ...) int nb = ft_printf("│ " fmt, ##__VA_ARGS__,); ft_printf("%*s", 50-nb, "│");
#define SAM_PRINT(fmt, ...) do { \
    int nb = ft_printf("│ " fmt, ##__VA_ARGS__); \
	SAM_PRINT_END(nb); \
} while (0)


#define SAM_HEADER "───── SHOWING ALLOCATION MEMORY "
#define SAM_PRINT_HEADER ft_printf("┌"SAM_HEADER); ft_printsn("─", SAM_WIDTH - 38); ft_printf("┐\n");
#define SAM_PRINT_BOTTOM ft_printf("└"); ft_printsn("─", SAM_WIDTH - 6); ft_printf("┘\n");

// void print_memory(size_t block_size)
// {
// 	ft_prints();
// 	ft_printn(" ", block_size / 8);
// }

//hexdump format
//0000000 0000 0000 0000 0000 0000 0000 0000 0000 
//hexdump -C format 
//00000000  23 69 6e 63 6c 75 64 65  20 3c 73 74 64 69 6f 2e  |#include <stdio.|

size_t print_hexdump_char(void *ptr, size_t len)
{
	size_t count = 0;

	count += ft_printf(" |");
	for (size_t i = 0; i < len; i++)
	{
		char c = *(char *)(ptr + i);
		c = ft_isprint(c) ? c : '.';
		count += ft_printf("%c", c);
	}
	count += ft_printf("|");
	return count;
}

size_t print_hexdump(void *ptr, size_t len)
{
	size_t count = 0;

	for (size_t i = 0; i < len; i++)
	{
		if (i == 8)
			count += ft_printf(" ");
		char c = *(char *)(ptr + i);
		
		count += ft_printf("%02X", c);

	}
	return count;
}

void print_block_hex(void *block, size_t alloc_size)
{
	block += S_META_DATA_SIZE;
	// SAM_PRINT ("%p : %ld bytes", block, alloc_size);
	// SAM_PRINT ("%p : %ld bytes", block, alloc_size);
	for (size_t i = 0; i < alloc_size; i += 16)
	{
		unsigned char *byte = (unsigned char *)(block + i);
		int count = ft_printf ("│         %07x ", (void *)byte - block);
		count += print_hexdump(byte, ft_min(alloc_size - i, 16));
		count += print_hexdump_char(byte, ft_min(alloc_size - i, 16));
		
		SAM_PRINT_END(count);
	}
}

void print_block(void *block, size_t block_size, size_t alloc_size, int is_free)
{
	if (data.sam_format == BASIC)
    	ft_printf("%p - %p : %ld bytes\n", block, block + block_size, alloc_size);
	if (data.sam_format == CUSTOM)
		SAM_PRINT(" %p : "PADDING"ld| "PADDING"ld| %s", block, alloc_size, block_size, (is_free ? "free" : ""));
	if (data.sam_format == HEXDUMP)
	{
		SAM_PRINT(" %p : "PADDING"ld| "PADDING"ld| %s", block, alloc_size, block_size, (is_free ? "free" : ""));
		print_block_hex(block, alloc_size);
	}
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
    pthread_mutex_lock(&data.samexlock);

    size_t sum = 0;
	data.sam_format = CUSTOM;
	if (data.hexdump)
		data.sam_format = HEXDUMP;

	SAM_PRINT_HEADER;
	SAM_PRINT("[   pointer   ] :  alloc|  block|");
	SAM_PRINT("");

    if (data.tiny)
	{
		struct zone_data *zone = data.tiny;
		size_t count = zone_count(zone);
		SAM_PRINT("TINY: %4ld zones:", count);
		for (int i = 0; zone; i++)
		{
			SAM_PRINT("Zone: %ld: %4ld allocs:", i, zone->alloc_count);
			print_zone(&sum, get_zone_usr_data_ptr(data.tiny), TINY_ZONE);
			zone = zone->next;
		}
	}
		
    if (data.small)
	{
		struct zone_data *zone = data.small;
		size_t count = zone_count(zone);
		SAM_PRINT("SMALL: %3ld zones:", count);
		for (int i = 0; zone; i++)
		{
			SAM_PRINT("Zone: %ld: %4ld allocs:", i, zone->alloc_count);
			print_zone(&sum, get_zone_usr_data_ptr(data.small), SMALL_ZONE);
			zone = zone->next;
		}
	}
	
    if (data.large)
	{
		SAM_PRINT("LARGE: %3ld alloc:", count_large_zone(data.large));
        print_large(&sum, data.large);
	}
    SAM_PRINT("Total: %ld bytes", sum);

	SAM_PRINT_BOTTOM;
    pthread_mutex_unlock(&data.samexlock);
}