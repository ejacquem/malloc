#include "malloc.h"

static struct malloc_data data;

size_t align_up(size_t size, size_t base)
{
    // return ((size - 1) / base + 1) * base;
    return (size + base - 1) & ~(base - 1);
}

size_t get_block_size(void *block)
{
    if (block == NULL)
        return LOG("block is NULL"), 0;
    return *(size_t *)(block) & ~0b1111; // ignore last 4 bits
}

struct meta_data get_block_meta_data(void *block)
{
    struct meta_data data;
    data.size = ((struct meta_data *)(block))->size ;
    data.next = ((struct meta_data *)(block))->next ;
    return data;
}

void * short_mmap(size_t size)
{
    LOG("MMAP called for size: %ld", size);
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
    LOG("MMAP: %s", addr != NULL ? "OK" : "FAIL");
    return addr;
}

// return true if init successfully
int init_zone(void **zone, size_t size)
{
    LOG("init zone of size: %ld", size);
    *zone = short_mmap(size);
    if (*zone)
    {
        *(size_t *)(*zone) = 0;
    }
    LOG("init state: %s", *zone != NULL ? "OK" : "FAIL");
    return *zone != NULL;
}

// algorithm used for TINY and SMALL zone
void *browse_zone(void **zone, size_t zone_size, size_t size)
{
    void *block;

    // printf("zone: %p", zone); fflush(stdout);
    // if (*zone == NULL)
    //     return printf("*zone NULL"), NULL;
    if (!(*zone || init_zone(zone, zone_size)))
        return NULL;

    size = align_up(sizeof(size_t) + size, 16);
    int block_count= 0;
    for (int i = 0; i < zone_size - size;)
    {
        block_count++;
        // LOG("i: %d", i);
        // LOG("block_count: %d", block_count);
        block = *zone + i;
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
    return NULL;
}

void *browse_heap(void **zone, size_t size)
{
    void *block;
    struct meta_data block_data;
    
    size = align_up(sizeof(struct meta_data) + size, 16);
    LOG("size aligned: %d", size);
    LOG("zone: %p", zone);
    if (*zone == NULL) // if large zone is null, init its first node
    {
        LOG("Init first node");
        *zone = short_mmap(size);
        LOG("allocation %s", *zone != NULL ? "OK" : "FAIL");
        ((struct meta_data *)(*zone))->size = size;
        ((struct meta_data *)(*zone))->next = 0;
        return ((struct meta_data *)*zone) + 1;
    }

    block = *zone;
    do
    {
        block_data = get_block_meta_data(block);
        if (block_data.size < size && IS_FREE(block_data.size)) // found a free block big enough
            return ((struct meta_data *)block) + 1;
        block = block_data.next;
    } while (block_data.next);
    
    // end of large zone, need a new mmap call
    void *ptr = short_mmap(size);
    if(ptr)
    {
        ((struct meta_data *)block)->next = ptr;
        ((struct meta_data *)block)->size = size;
        return ((struct meta_data *)ptr) + 1;
    }
    return NULL;
}

void *malloc(size_t size)
{
    void *addr;

    LOG("My malloc called with size %ld", size);

    if (size < TINY_SIZE && LOG("Browse TINY") &&(addr = browse_zone(&data.tiny, TINY_ZONE, size)))
        return LOG("Returning ptr: %p", addr),addr;
    if (size < SMALL_SIZE && LOG("Browse SMALL") && (addr = browse_zone(&data.small, SMALL_ZONE, size)))
        return LOG("Returning ptr: %p", addr),addr;

    LOG("Browse LARGE");
    return browse_heap(&data.big, size);
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;
    size_t *block = ((size_t *)ptr) - 1; // step back 8 bytes
    *block = SET_FREE(*block);
}