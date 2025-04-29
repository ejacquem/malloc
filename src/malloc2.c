#include "../malloc2.h"


void * map(size_t size)
{
    LOGLN;
    LOG("MMAP called for size: %ld", size);
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
    LOG("MMAP: %s", addr != NULL ? OK : FAIL);
    return addr;
}

void ft_free(void *ptr);

block_info_t get_block_info(meta_data_t *block)
{
    meta_data_t data = *block;
    block_info_t info;
    
    info.user_size = data.user_size;
    info.block_size = GET_SIZE(data.size);
    info.usable_size = info.block_size - sizeof(meta_data_t);
    info.user_ptr = block + 1;
    info.isft_free = IS_FREE(data.size);
    return info;
}

// asign a new user_size, remove ft_free flag, and return the user pointer to that block
void *reuse_block(meta_data_t *block, size_t size)
{
    block->user_size = size;
    block->size = SET_NOT_FREE(block->size);
    return (void *)(block + 1);
}

// void *new_large_block()
// {

// }

void *browse_blocks(const void *zone, size_t zone_size, size_t size)
{
    meta_data_t *block = zone;
    block_info_t info;

    size_t new_block_size = align_up(sizeof(meta_data_t) + size, 16);
    while (1)
    {
        info = get_block_info(block);
        // return block if ft_free and enough space
        if (info.isft_free && info.usable_size >= size)
            reuse_block(block, size);

        // allocate new memory if null
        if (block->next == NULL)
        {
            meta_data_t *next_block;
            // create new zone if TINY or SMALL
            if (size < SMALL_SIZE)
            {
                // next_block
            }
            else 
            {
                meta_data_t *block_data = map(new_block_size);
                block->next = block_data;
                block_data->next = NULL;
                block_data->size = new_block_size;
                block_data->user_size = size;
                return block_data + 1;
            }
        }

        block = block->next;
    }
    return NULL;
}

void *ft_malloc(size_t size)
{

    print_define();
    void *addr;

    LOG("----- Malloc called -----");
    LOG("size: %ld", size);

    if (size == 0)
        return data.zero_allocation;

    if (size <= TINY_SIZE && LOG("Browse TINY"))
        addr = browse_zones(&data.tiny, TINY_ZONE, size);
    else if (size <= SMALL_SIZE && LOG("Browse SMALL"))
        addr = browse_zones(&data.small, SMALL_ZONE, size);
    else if (LOG("Browse LARGE"))
        addr = browse_zones(&data.large, -1, size);
    
    LOG("Returning pointer: %p", addr);
    LOGLN;
    return addr;
}

void *ft_realloc(void *ptr, size_t size);