#include "../malloc.h"

struct malloc_data data = {0};

void * short_mmap(size_t size)
{
    LOGLN;
    LOG("MMAP called for size: %ld", size);
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
    LOG("MMAP: %s", addr != NULL ? OK : FAIL);
    return addr;
}

// return true if init successfully
int init_zone(void **zone, size_t size)
{
    LOG("init zone of size: %ld", size);
    *zone = short_mmap(size);
    if (*zone)
    {
        ((struct zone_data *)(*zone))->next = NULL;
        ((struct zone_data *)(*zone))->is_full = FALSE;
    }
    LOG("init state: %s", *zone != NULL ? OK : FAIL);
    return *zone != NULL;
}

void join_next_block(void *block)
{
    size_t block_data = get_block_data(block);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
    size_t next_block_data = get_block_data(block + block_size);
    size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
    *(size_t *)block = block_size + next_block_size;
    // *(size_t *)block = SET_FREE(*(size_t *)block);
}

// assign new size to the current block, split into 2 blocks if there is enough space for a new block
// return ptr to user data
void *replace_block(void *block, size_t size)
{
    size_t new_block_size = get_block_size(size, S_META_DATA_SIZE);
    size_t block_size = get_block_size(get_block_data(block), S_META_DATA_SIZE);
    if (new_block_size > block_size)
        return LOG("ERROR: can't replace a block with a bigger size"), NULL;
    *(size_t *)block = size;
    if (new_block_size < block_size) // if new block is smaller, split current block into one full and one free
    {
        size_t *next_block = (size_t *)(block + new_block_size); 
        *next_block = block_size - new_block_size; // set next block size to (current - new)
        *next_block = SET_FREE(*next_block);
    }
    return block + S_META_DATA_SIZE;
}

// algorithm used for TINY and SMALL zone
void *browse_zone(void *zone, size_t zone_size, size_t size)
{
    size_t new_block_size = get_block_size(size, S_META_DATA_SIZE);
    const int zone_limit = zone_size - new_block_size; 
    void *block;
    size_t block_data, alloc_size, block_size;

    for (int i = 0; i < zone_limit;)
    {
        block = zone + i;
        block_data = get_block_data(block);
        alloc_size = GET_SIZE(block_data);
        block_size = get_block_size(block_data, S_META_DATA_SIZE);
        // LOG("ptr: %p", block);
        // LOG("size: %ld", size);
        // LOG("block_data: %ld", block_data);
        // LOG("alloc_size: %ld", alloc_size);
        // LOG("block_size: %ld", block_size);
        // LOG("*(size_t *)block: %ld", *(size_t *)block);
        if (alloc_size == 0) // reached the end of the heap
        {
            *(size_t *)(block + new_block_size) = 0; // set next block size to 0
            *(size_t *)block = size; // set block size (overwrites flag)
            LOG("size aligned: %ld", new_block_size);
            LOG("(size_t *)block: %p", (size_t *)block);
            LOG("*(size_t *)block: %ld", *(size_t *)block);
            return block + S_META_DATA_SIZE;
        }
        // LOG("ptr: %p", block);
        // LOG("alloc_size: %ld", alloc_size);
        // LOG("IS_FREE(alloc_size): %d", IS_FREE(block_data));
        // LOG("get_block_usable_size(alloc_size, S_META_DATA_SIZE): %d", get_block_usable_size(alloc_size, S_META_DATA_SIZE));
        if (size <= get_block_usable_size(alloc_size, S_META_DATA_SIZE) && IS_FREE(block_data)) // found a free block big enough
        {
            LOG(" - Found free block");
            return replace_block(block, size);
        }
        i += block_size;
    }
    // LOG("End of the zone reached");
    // ((struct zone_data *)(zone) - 1)->is_full = TRUE;
    return NULL;
}

void *browse_all_zones(void **zones, size_t zone_size, size_t size)
{
    int zone_index = 0;
    struct zone_data *zone;
    void *addr;

    if (!(*zones || init_zone(zones, zone_size)))
        return NULL;

    zone = *zones;
    while (1)
    {
        if (!zone->is_full)
        {
            addr = browse_zone((void **)(zone + 1), zone_size - sizeof(struct zone_data), size);
            // LOG("allocation ptr: %p %s", addr, addr != NULL ? OK : FAIL);
            if (addr != NULL)
            {
                LOG("Found space in zone index %d", zone_index);
                return addr;
            }
        }

        // If full or allocation failed, move to the next zone or allocate one
        if (zone->next == NULL)
        {
            if(!init_zone((void **)&(zone->next), zone_size))
                return LOG("New zone allocation failed"), NULL;
        }
        zone = zone->next;
        zone_index++;
    }
    return NULL;
}
// ZONE_DATA_SIZE

void *browse_heap(void **zone, size_t size)
{
    void *block;
    struct l_meta_data *block_data;
    
    size_t block_size = get_block_size(size, L_META_DATA_SIZE);
    LOG("size aligned: %ld", block_size);
    // LOG("zone: %p", zone);
    if (*zone == NULL) // if large zone is null, init its first node
    {
        LOG("Init first node");
        *zone = short_mmap(block_size);
        LOG("allocation ptr: %p %s", *zone, *zone != NULL ? OK : FAIL);
        block_data = *zone;
        block_data->size = size;
        block_data->next = 0;
        return get_user_data_pointer(block_data);
    }

    LOG("Searching free space");
    block = *zone;
    while (1)
    {
        block_data = block;
        // LOG("Test1 --------");
        // LOG("block_data->size, %ld", GET_SIZE(block_data->size));
        // LOG("IS_FREE(block_data->size), %d", IS_FREE(block_data->size));
        // LOG("size, %ld", size);
        // LOG("usable data, %ld", get_block_usable_size(block_data->size, L_META_DATA_SIZE));
        // LOG("block_data->next, %p", block_data->next);
        if (size <= get_block_usable_size(block_data->size, L_META_DATA_SIZE) && IS_FREE(block_data->size)) // found a free block big enough
        {
            LOG("Found some nice free space :)");
            block_data->size = SET_NOT_FREE(block_data->size);
            return get_user_data_pointer(block_data);
        }
        // LOG("Test2");
        if(block_data->next == NULL) // end of large zones
            break;
        block = block_data->next;
    }

    LOG("Allocating a new large zone");
    void *ptr = short_mmap(block_size); // end of large zone, need a new mmap call
    LOG("allocation ptr: %p %s", *zone, *zone != NULL ? OK : FAIL);
    if(ptr)
    {
        block_data = block;
        block_data->next = ptr;
        block_data->size = size;
        return get_user_data_pointer(block_data);
    }
    return NULL;
}

void *malloc(size_t size)
{
    print_define();
    void *addr;

    LOG("----- Malloc called -----");
    LOG("size: %ld", size);

    if (size == 0)
        return data.zero_allocation;

    if (size <= TINY_SIZE && LOG("Browse TINY"))
        addr = browse_all_zones(&data.tiny, TINY_ZONE, size);
    else if (size <= SMALL_SIZE && LOG("Browse SMALL"))
        addr = browse_all_zones(&data.small, SMALL_ZONE, size);
    else if (LOG("Browse LARGE"))
        addr = browse_heap(&data.large, size);
    
    LOG("Returning pointer: %p", addr);
    LOGLN;
    return addr;
}

void *realloc(void *ptr, size_t size)
{
    LOG("--- Realloc called ---");
    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
        return free(ptr), NULL;
    
    
    void *block = ((size_t *)ptr) - 1; // step back 8 bytes
    size_t block_data = get_block_data(block);
    size_t alloc_size = GET_SIZE(block_data);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);
    
    if (alloc_size <= SMALL_SIZE)
    {
        LOG("Realloc called");
        size_t block_usable_size = get_block_usable_size(alloc_size, S_META_DATA_SIZE);
        if (block_usable_size >= size)
            return ptr;
        size_t *next_block = (size_t *)(block + block_size);
        size_t next_block_data = get_block_data(next_block);
        size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
        if (IS_FREE(*next_block) && (block_usable_size + next_block_size) <= size)
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
        int n = 100;
        ft_memcpy(addr, ptr, alloc_size);
        free(ptr);
        return addr;
    }
    return NULL;
}


void free(void *ptr)
{
    if (ptr == NULL)
        return LOG("Free called on NULL"), (void)0;
    size_t *block = ((size_t *)ptr) - 1; // step back 8 bytes
    LOG("Free called on pointer %p", block);
    if (IS_FREE(*block))
        return LOG("Pointer already free !"), (void)0;
    *block = SET_FREE(*block);
}