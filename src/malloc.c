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

// algorithm used for TINY and SMALL zone
void *browse_zone(void *zone, size_t zone_size, size_t size)
{
    void *block;

    int block_count= 0;
    for (int i = 0; i < zone_size - size;)
    {
        block_count++;
        // LOG("i: %d", i);
        // LOG("block_count: %d", block_count);
        block = zone + i;
        size_t block_size = get_block_size(block);
        if (block_size == 0) // reached the end of the heap
        {
            *(size_t *)(block + size) = 0; // set next block size to 0
            *(size_t *)block = size; // set block size (overwrites flag)
            return block + sizeof(size_t);
        }
        if (block_size < size && IS_FREE(block_size)) // found a free block big enough
        {
            LOG("---- Found free block");
            return block + sizeof(size_t);
        }
        i += block_size;
    }
    LOG("End of the zone reached");
    // ((struct zone_data *)(zone) - 1)->is_full = TRUE;
    return NULL;
}

void *browse_all_zones(void **zones, size_t zone_size, size_t size)
{
    struct zone_data *zone;
    void *addr;

    if (!(*zones || init_zone(zones, zone_size)))
        return NULL;
    
    size = align_up(sizeof(size_t) + size, 16);
    LOG("size aligned: %d", size);

    zone = *zones;
    // LOG("zone: %p", zone);
    // LOG("zone + 1: %p", zone + 1);
    // LOG("((void *)zone) + sizeof(struct zone_data): %p", ((void *)zone) + sizeof(struct zone_data));
    // LOG("zdata size: %d", sizeof(struct zone_data));
    while (1)
    {
        if (!zone->is_full)
        {
            addr = browse_zone((void **)(zone + 1), zone_size - sizeof(struct zone_data), size); // + 1 to skip the zone_data
            // LOG("allocation ptr: %p %s", addr, addr != NULL ? OK : FAIL);
            if (addr != NULL)
                return addr;
        }

        // If full or allocation failed, move to the next zone or allocate one
        if (zone->next == NULL)
        {
            if(!init_zone((void **)&(zone->next), zone_size))
                return LOG("New zone allocation failed"), NULL;
        }
        zone = zone->next;
    }
    return NULL;
}
// ZONE_DATA_SIZE

void *browse_heap(void **zone, size_t size)
{
    void *block;
    struct meta_data *block_data;
    
    size = align_up(sizeof(struct meta_data) + size, 16);
    LOG("size aligned: %d", size);
    LOG("zone: %p", zone);
    if (*zone == NULL) // if large zone is null, init its first node
    {
        LOG("Init first node");
        *zone = short_mmap(size);
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
        if (block_data->size < size && IS_FREE(block_data->size)) // found a free block big enough
            return get_user_data_pointer(block_data);
        if(block_data->next == NULL) // end of large zones
            break;
        block = block_data->next;
    }

    LOG("Allocating a new large zone");
    // end of large zone, need a new mmap call
    void *ptr = short_mmap(size);
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
    return addr;
}

void free(void *ptr)
{
    LOG("Free called");
    if (ptr == NULL)
        return;
    size_t *block = ((size_t *)ptr) - 1; // step back 8 bytes
    *block = SET_FREE(*block);
}