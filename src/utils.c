#include "../malloc.h"

size_t align_up(size_t size, size_t base)
{
    // return ((size - 1) / base + 1) * base;
    return (size + base - 1) & ~(base - 1);
}

// get data (size) for TINY or SMALL block
size_t get_block_data(void *block)
{
    return *(size_t *)(block);
}

//transform user data size into block size for TINY and SMALL blocks
size_t get_block_size(size_t size, size_t meta_data_size)
{
    return align_up(meta_data_size + GET_SIZE(size), 16);
}

size_t get_block_usable_size(size_t size, size_t meta_data_size)
{
    return get_block_size(size, meta_data_size) - meta_data_size;
}

void *get_zone_usr_data_ptr(void *zone)
{
    return ((struct zone_data *)zone) + 1;
}

//get data for LARGE block
struct l_meta_data get_block_meta_data(void *block)
{
    struct l_meta_data data;
    data.size = ((struct l_meta_data *)(block))->size ;
    data.next = ((struct l_meta_data *)(block))->next ;
    return data;
}

void *get_user_data_pointer(void *ptr)
{
    return ((struct l_meta_data*)ptr) + 1;
}

struct zone_data *get_block_zone(struct zone_data *list, size_t zone_size, void *ptr)
{
    while(list)
    {
        if (ptr > (void *)list && ptr < (void *)(list + zone_size))
            return list;
        list = list->next;
    }
    return NULL;
}

// join the next block to the current
// modify current block size
void join_next_block(void *block)
{
    size_t block_data = get_block_data(block);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
    size_t next_block_data = get_block_data(block + block_size);
    size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
    LOG("JOIN next block");
    // LOG("block_data, %ld", block_data);
    // LOG("block_size, %ld", block_size);
    // LOG("next_block_data, %ld", next_block_data);
    // LOG("next_block_size, %ld", next_block_size);
    char is_free = IS_FREE(block_data);
    *(size_t *)block = (block_size - S_META_DATA_SIZE) + next_block_size;
    *(size_t *)block = is_free ? SET_FREE(*(size_t *)block) : SET_NOT_FREE(*(size_t *)block);
}

// append all contiguous free blocks after the current block to the current block
void defragment_memory(void *block)
{
    LOG(" - Defragment memory -");
    size_t block_data = get_block_data(block);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
    void *next_block = block + block_size;
    size_t next_block_data = get_block_data(next_block);
    
	LOG("size before: %ld", GET_SIZE(block_data));
    while (IS_FREE(next_block_data))
    {
		LOG("next block is free");
		size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
        block_size = get_block_size(block_data, S_META_DATA_SIZE);
        if (GET_SIZE(next_block_data) == 0) // reached the end of the allocated blocks
			return;
        join_next_block(block);
        next_block += next_block_size;
        next_block_data = get_block_data(next_block);
    }
	LOG("size after: %ld", GET_SIZE(get_block_data(block)));
}

void print_list(struct l_meta_data *list)
{
    int i = 0;

    while(list)
    {
        LOG(" - node %d: %p", i++, list);
        list = list->next;
    }
}

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
    LOG("----- MALLOC INFO -----");
    LOG("TINY_SIZE: %d bytes", TINY_SIZE);
    LOG("TINY_BLOCK_SIZE: %d bytes", TINY_BLOCK_SIZE);
    LOG("TINY_ZONE: %d bytes", TINY_ZONE);
    LOG("TINY_PAGE_NB: %d pages", TINY_PAGE_NB);
    LOG("Min block number: %d blocks", TINY_ZONE / TINY_BLOCK_SIZE);
    LOG("");
    LOG("SMALL_SIZE: %d bytes", SMALL_SIZE);
    LOG("SMALL_BLOCK_SIZE: %d bytes", SMALL_BLOCK_SIZE);
    LOG("SMALL_ZONE: %d bytes", SMALL_ZONE);
    LOG("SMALL_PAGE_NB: %d pages", SMALL_PAGE_NB);
    LOG("Min block number: %d blocks", SMALL_ZONE / SMALL_BLOCK_SIZE);
    LOG("");
	LOG("%s: %s", ENV_DEBUG, getenv(ENV_DEBUG));
	LOG("%s: %s", ENV_HEXDUMP, getenv(ENV_HEXDUMP));
    LOG("");
    LOG("data.tiny: %d", data.tiny);
    LOG("data.small: %d", data.small);
    LOG("data.large: %d", data.large);
    LOG("data.first: %d", data.first);
    LOG("");
}