#include "../malloc.h"

typedef enum ZONE_TYPE
{
	TINY,
	SMALL,
	LARGE
} zone_type;

zone_type get_zone_type_from_size(size_t size)
{
	if (size <= TINY_SIZE)
		return TINY;
	if (size <= SMALL_SIZE)
		return SMALL;
	return LARGE;
}

void *realloc(void *ptr, size_t size)
{
	LOG("----- Free called -----");

    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
        return free(ptr), NULL;

		
	LOG("Pointer %p", ptr);
	LOG("Size: %ld", size);
    
    void *block = ((size_t *)ptr) - 1L; // step back 8 bytes
    LOG("block: %p", block);
    size_t block_data = get_block_data(block);
    size_t alloc_size = GET_SIZE(block_data);
    size_t block_size = get_block_size(block_data, S_META_DATA_SIZE);

    LOG("alloc_size: %ld", alloc_size);
    LOG("block_size: %ld", block_size);
    LOG("realloc_size: %ld", size);

	char realloc_in_same_zone = get_zone_type_from_size(alloc_size) == get_zone_type_from_size(size);

	if (realloc_in_same_zone)
		LOG("Changing zone");
    
    if (size <= SMALL_SIZE && realloc_in_same_zone)
    {
        LOG("Realloc SMALL or TINY");
        size_t block_usable_size = get_block_usable_size(alloc_size, S_META_DATA_SIZE);

        if (size <= block_usable_size)
			return replace_block(block, size);
            
        defragment_memory(block); // defragment memory, will change alloc_size

		if (size <= GET_SIZE(get_block_data(block)))
            return replace_block(block, size);
        // replace_block(block, alloc_size); // put back alloc_size

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