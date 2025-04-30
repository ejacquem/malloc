#include "../malloc.h"

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
    *(size_t *)block = block_size + next_block_size;
    // *(size_t *)block = SET_FREE(*(size_t *)block);
}

void *realloc(void *ptr, size_t size)
{
    LOG("--- Realloc called ---");
    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
        return free(ptr), NULL;
    
    void *block = ((size_t *)ptr) - 1L; // step back 8 bytes
    LOG("ptr: %p", ptr);
    LOG("block: %p", block);
    LOG("test");
    size_t block_data = get_block_data(block);
    size_t alloc_size = GET_SIZE(block_data);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);

    LOG("alloc_size: %ld", alloc_size);
    LOG("block_size: %ld", block_size);
    LOG("realloc_size: %ld", size);
    
    if (alloc_size <= SMALL_SIZE && size <= SMALL_SIZE)
    {
        LOG("Realloc SMALL");
        size_t block_usable_size = get_block_usable_size(alloc_size, S_META_DATA_SIZE);
        LOG("block_usable_size: %ld", block_usable_size);
        if (size <= block_usable_size)
            return ptr;
        size_t *next_block = (size_t *)(block + block_size);
        size_t next_block_data = get_block_data(next_block);
        size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
        if (IS_FREE(*next_block) && size <= (block_usable_size + next_block_size))
        {
            join_next_block(block);
            return replace_block(block, size);
        }
        LOG("Can't realloc for small/tiny");
    }
    else
        LOG("Realloc LARGE");

    // can't add more data, creating new pointer, copy data, free old ptr
    void *addr = malloc(size);
    if (addr)
    {
        ft_memcpy(addr, ptr, MIN(alloc_size, size));
        free(ptr);
        LOG("realloc OK");
        return addr;
    }
    LOG("realloc NO");
    return NULL;
}