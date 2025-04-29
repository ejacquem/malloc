#include "../malloc.h"

struct ft_malloc_data data = {0};

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
    if (new_block_size < block_size) // if new block is smaller, split current block into one full and one ft_free
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
        if (size <= get_block_usable_size(alloc_size, S_META_DATA_SIZE) && IS_FREE(block_data)) // found a ft_free block big enough
        {
            LOG(" - Found ft_free block");
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
            addr = browse_zone((void **)(zone + 1L), zone_size - sizeof(struct zone_data), size);
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

void *new_large_block(size_t block_size, size_t size)
{
    struct l_meta_data *block_data;
    block_data = short_mmap(block_size);

    if (block_data == NULL)
        return NULL;

    block_data->size = size;
    block_data->next = NULL;
    return block_data;
}

void add_back(void **list, void *node)
{
    struct l_meta_data *block = *list;

    if (*list == NULL)
    {
        *list = node;
        return ;
    }

    while(block->next)
    {
        if(block == block->next)
            return LOG("WTF ?"), (void)0;
        block = block->next;
        
        LOG("block, %p", block);
    }

    block->next = node;
}

void *ft_malloc_large(void **zone, size_t size)
{
    void *new_block;
    struct l_meta_data *block = *zone;
    size_t new_block_size = align_up(sizeof(struct l_meta_data) + size, 16);

    new_block = new_large_block(new_block_size, size);
    LOG("created new zone at: %p", new_block);

    add_back(zone, new_block);

    return new_block + sizeof(struct l_meta_data);
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
        addr = browse_all_zones(&data.tiny, TINY_ZONE, size);
    else if (size <= SMALL_SIZE && LOG("Browse SMALL"))
        addr = browse_all_zones(&data.small, SMALL_ZONE, size);
    else if (LOG("Browse LARGE"))
        addr = ft_malloc_large(&data.large, size);
    
    LOG("Returning pointer: %p", addr);
    LOGLN;
    return addr;
}

void *ft_realloc(void *ptr, size_t size)
{
    LOG("--- Realloc called ---");
    if (ptr == NULL)
        return ft_malloc(size);
    if (size == 0)
        return ft_free(ptr), NULL;
    
    void *block = ((size_t *)ptr) - 1L; // step back 8 bytes
    LOG("ptr: %p", ptr);
    LOG("block: %p", block);
    LOG("test");
    size_t block_data = get_block_data(block);
    size_t alloc_size = GET_SIZE(block_data);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);

    LOG("alloc_size: %ld", alloc_size);
    LOG("block_size: %ld", block_size);
    
    if (alloc_size <= SMALL_SIZE && size <= SMALL_SIZE)
    {
        LOG("Realloc SMALL");
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
        LOG("Can't ft_realloc for small/tiny");
    }
    else
        LOG("Realloc LARGE");

    // can't add more data, creating new pointer, copy data, ft_free old ptr
    void *addr = ft_malloc(size);
    if (addr)
    {
        ft_memcpy(addr, ptr, MIN(alloc_size, size));
        ft_free(ptr);
        LOG("ft_realloc OK");
        return addr;
    }
    LOG("ft_realloc NO");
    return NULL;
}

void remove_node(struct l_meta_data **head, void *ptr) {
    if (!head || !*head) return;

    struct l_meta_data *curr = *head;
    struct l_meta_data *prev = NULL;

    while (curr) {
        if (curr == ptr) {
            if (prev)
            {
                LOG("Removing node: %p", curr);
                prev->next = curr->next;
            }
            else
            {
                LOG("Removing first node: %p", curr);
                *head = curr->next;
                LOG("new first: %p", *head);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void print_list(struct l_meta_data *list)
{
    int i = 0;

    while(list)
    {
        LOG(" - - - - - - - -node %d: %p", i++, list);
        list = list->next;
    }
}

void ft_free(void *ptr)
{
    if (ptr == NULL)
        return LOG("Free called on NULL"), (void)0;
    size_t *block = ((size_t *)ptr) - 1L; // step back 8 bytes
    LOG("Free called on pointer %p", block);
    if (IS_FREE(*block))
        return LOG("Pointer already ft_free !"), (void)0;

    if (GET_SIZE(*block) > SMALL_SIZE)
    {
        struct l_meta_data *large_block = ((struct l_meta_data *)ptr) - 1L;
        remove_node((void *)&data.large, large_block);
        if (munmap(large_block, align_up(sizeof(struct l_meta_data) + GET_SIZE(*block), 16)) == 0)
            LOG("munmap succesful");
        else
            LOG("munmap failed: %s", strerror(errno));
    }
    else
        *block = SET_FREE(*block);
}