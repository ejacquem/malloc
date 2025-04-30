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
    LOG("JOIN next block");
    LOG("block_data, %ld", block_data);
    LOG("block_size, %ld", block_size);
    LOG("next_block_data, %ld", next_block_data);
    LOG("next_block_size, %ld", next_block_size);
    *(size_t *)block = block_size + next_block_size;
    // *(size_t *)block = SET_FREE(*(size_t *)block);
}

// assign new size to the current block, split into 2 blocks if there is enough space for a new block
// return ptr to user data
void *replace_block(void *block, size_t size)
{
    LOG("replace block");
    size_t new_block_size = get_block_size(size, S_META_DATA_SIZE);
    size_t block_size = get_block_size(get_block_data(block), S_META_DATA_SIZE);
    // LOG("block_user_size: %ld", GET_SIZE(get_block_data(block)));
    // LOG("size: %ld", size);
    // LOG("block_size: %ld", block_size);
    // LOG("new_block_size: %ld", new_block_size);
    if (new_block_size > block_size)
        return LOG("ERROR: can't replace a block with a bigger size"), exit(1), NULL;
    *(size_t *)block = size;
    if (new_block_size < block_size) // if new block is smaller, split current block into one full and one ft_free
    {
        // LOG("Splitting block");
        size_t *next_block = (size_t *)(block + new_block_size); 
        *next_block = block_size - new_block_size - S_META_DATA_SIZE; // set next block size to (current - new)
        *next_block = SET_FREE(*next_block);
    }
    // LOG("returning pointer: %p", block + S_META_DATA_SIZE);
    // LOG("block_user_size: %ld", GET_SIZE(get_block_data(block)));
    return block + S_META_DATA_SIZE;
}

// algorithm used for TINY and SMALL zone
void *browse_zone(struct zone_data *zone, size_t zone_size, size_t size)
{
    size_t new_block_size = get_block_size(size, S_META_DATA_SIZE);
    const int zone_limit = zone_size - new_block_size; 
    void *block;
    void *zone_start = zone + 1L;
    size_t block_data, alloc_size, block_size;

    for (int i = 0; i < zone_limit;)
    {
        block = zone_start + i;
        block_data = get_block_data(block);
        alloc_size = GET_SIZE(block_data);
        block_size = get_block_size(block_data, S_META_DATA_SIZE);
        // if (alloc_size > SMALL_SIZE)
        // {
        //     LOG("block: %p", block);
        //     LOG("block_size: %ld", block_size);
        //     return LOG("ERROR: found size of %d in SMALL or TINY zone", alloc_size), exit(1), NULL;
        // }
        // LOG("ptr: %p", block);
        // LOG("size: %ld", size);
        // LOG("block_data: %ld", block_data);
        // LOG("alloc_size: %ld", alloc_size);
        // LOG("block_size: %ld", block_size);
        // LOG("*(size_t *)block: %ld", *(size_t *)block);
        if (alloc_size == 0) // reached the end of the allocated blocks
        {
            *(size_t *)(block + new_block_size) = 0; // set next block size to 0
            *(size_t *)block = size; // set block size (overwrites flag)
            // LOG("size aligned: %ld", new_block_size);
            // LOG("(size_t *)block: %p", (size_t *)block);
            // LOG("*(size_t *)block: %ld", *(size_t *)block);
            zone->alloc_count++;
            return block + S_META_DATA_SIZE;
        }
        // LOG("ptr: %p", block);
        // LOG("alloc_size: %ld", alloc_size);
        // LOG("IS_FREE(alloc_size): %d", IS_FREE(block_data));
        // LOG("get_block_usable_size(alloc_size, S_META_DATA_SIZE): %d", get_block_usable_size(alloc_size, S_META_DATA_SIZE));
        if (size <= get_block_usable_size(alloc_size, S_META_DATA_SIZE) && IS_FREE(block_data)) // found a ft_free block big enough
        {
            LOG(" - Found ft_free block");
            // zone->allocated_data_count += size;
            // zone->free_data_count -= size;
            return replace_block(block, size);
        }
        i += block_size;
    }
    LOG("End of the zone reached");
    zone->is_full = TRUE;
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
            addr = browse_zone(zone, zone_size - sizeof(struct zone_data), size);
            // LOG("allocation ptr: %p %s", addr, addr != NULL ? OK : FAIL);
            if (addr != NULL)
            {
                LOG("Found space in zone index %d", zone_index);
                return addr;
            }
        }

        // If full or can't allocate, move to the next zone or allocate one
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

                if(curr->next == NULL) // if removing last node, set last_large to prev
                    data.last_large = prev;
            }
            else
            {
                LOG("Removing first node: %p", curr);
                *head = curr->next;
                LOG("new first: %p", *head);

                if(curr->next == NULL) // if removing only node, set last_large to NULL
                    data.last_large = curr->next;
            }

            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void add_back(void **list, void *node)
{
    struct l_meta_data *block = *list;

    if (*list == NULL)
    {
        *list = node;
        data.last_large = node;
        return ;
    }

    data.last_large->next = node;
    data.last_large = node;
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
    LOG("realloc_size: %ld", size);
    
    if (alloc_size <= SMALL_SIZE && size <= SMALL_SIZE)
    {
        LOG("Realloc SMALL");
        size_t block_usable_size = get_block_usable_size(alloc_size, S_META_DATA_SIZE);
        LOG("block_usable_size: %ld", block_usable_size);
        // if (size <= block_usable_size)
        //     return ptr;
        // size_t *next_block = (size_t *)(block + block_size);
        // size_t next_block_data = get_block_data(next_block);
        // size_t next_block_size = get_block_size(next_block_data, S_META_DATA_SIZE);
        // if (IS_FREE(*next_block) && size <= (block_usable_size + next_block_size))
        // {
        //     join_next_block(block);
        //     return replace_block(block, size);
        // }
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

void print_list(struct l_meta_data *list)
{
    int i = 0;

    while(list)
    {
        LOG(" - node %d: %p", i++, list);
        list = list->next;
    }
}

void *get_block_zone(struct zone_data *list, size_t zone_size, void *ptr)
{
    while(list)
    {
        if (ptr > (void *)list && ptr < (void *)(list + zone_size))
            return list;
        list = list->next;
    }
    return NULL;
}

size_t empty_zone_count(struct zone_data *list)
{
    size_t count = 0;

    while(list)
    {
        if (list->alloc_count == 0)
            count++;
        list = list->next;
    }
    return count;
}

void remove_zone_node(struct zone_data **head, void *ptr)
{
    if (!head || !*head)
        return;

    struct zone_data *curr = *head;
    struct zone_data *prev = NULL;

    while (curr)
    {
        if (curr == ptr)
        {
            if (prev)
                prev->next = curr->next;
            else
                *head = curr->next;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void free_small(void *ptr, void **zone_list, size_t zone_size)
{
    size_t *block = ((size_t *)ptr) - 1UL; // step back 8 bytes

    if (IS_FREE(*block))
        return LOG("Pointer already free !"), (void)0;

    *block = SET_FREE(*block);
    struct zone_data *zone = get_block_zone(*zone_list, zone_size, block);
    zone->alloc_count--;
    zone->is_full = FALSE;
    if (zone->alloc_count == 0 && empty_zone_count(*zone_list) >= 2UL) // if zone is empty and at least one other empty zone exists
    {
        LOG(" ------------------------------------------ Removing a small zone");
        LOG("empty_zone_count(*zone_list): %ld", empty_zone_count(*zone_list));
        remove_zone_node((struct zone_data **)zone_list, zone);
        if (munmap(zone, zone_size) == 0)
            LOG("munmap succesful");
        else
            LOG("munmap failed: %s", strerror(errno));
    }
}

void free_large(void *ptr)
{
    struct l_meta_data *large_block = ((struct l_meta_data *)ptr) - 1UL;
    remove_node((void *)&data.large, large_block);
    // if (munmap(large_block, align_up(sizeof(struct l_meta_data) + GET_SIZE(large_block->size), 16)) == 0)
    if (munmap(large_block, get_block_size(large_block->size, L_META_DATA_SIZE)) == 0)
        LOG("munmap succesful");
    else
        LOG("munmap failed: %s", strerror(errno));
}

void ft_free(void *ptr)
{
    if (ptr == NULL)
        return LOG("Free called on NULL"), (void)0;

    LOG("Free called on pointer %p", ptr);
    size_t size = GET_SIZE(*(((size_t *)ptr) - 1UL));
    LOG("Free of size: %ld", size);

    if (size <= TINY_SIZE)
        free_small(ptr, &data.tiny, TINY_ZONE);
    else if (size <= SMALL_SIZE)
        free_small(ptr, &data.small, SMALL_ZONE);
    else
        free_large(ptr);

    LOG("free succesful");
}