#include "../malloc.h"

struct malloc_data data = {0};

void * short_mmap(size_t size)
{
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
        // ((struct zone_data *)(*zone))->is_full = FALSE;
    }
    LOG("init state: %s", *zone != NULL ? OK : FAIL);
    return *zone != NULL;
}

// algorithm used for TINY and SMALL zone
void *browse_zone(struct zone_data *zone, size_t zone_size, size_t size)
{
    size_t new_block_size = get_block_size(size, S_META_DATA_SIZE);
    const size_t zone_limit = zone_size - new_block_size; 
    void *block;
    void *zone_start = zone + 1L;
    size_t block_data, alloc_size, block_size;

    for (size_t i = 0; i < zone_limit;)
    {
        block = zone_start + i;
        block_data = get_block_data(block);
        alloc_size = GET_SIZE(block_data);
        block_size = get_block_size(block_data, S_META_DATA_SIZE);
        if (alloc_size == 0) // reached the end of the allocated blocks
        {
            *(size_t *)(block + new_block_size) = 0; // set next block size to 0
            *(size_t *)block = size; // set block size (overwrites flag)
            zone->alloc_count++;
            return block + S_META_DATA_SIZE;
        }
        if (size <= get_block_usable_size(alloc_size, S_META_DATA_SIZE) && IS_FREE(block_data)) // found a free block big enough
        {
            LOG(" - Found free block");
            return replace_block(block, size);
        }
        i += block_size;
    }
    // LOG("End of the zone reached");
    // zone->is_full = TRUE;
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
        // if (!zone->is_full)
        // {
        addr = browse_zone(zone, zone_size - sizeof(struct zone_data), size);
        // LOG("allocation ptr: %p %s", addr, addr != NULL ? OK : FAIL);
        if (addr != NULL)
        {
            LOG("Found space in zone index %d", zone_index);
            return addr;
        }
        // }

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

void *malloc_large(size_t size)
{
    void *new_block;
    struct l_meta_data *block = data.large;
    size_t new_block_size = align_up(sizeof(struct l_meta_data) + size, 16);

    new_block = new_large_block(new_block_size, size);
    LOG("created new zone at: %p", new_block);

    add_back(&data.large, new_block);

    return new_block + sizeof(struct l_meta_data);
}

int is_env_set(const char *env_name)
{
	const char *value = getenv(env_name);
	if (value)
		return value[0] == '1' || ft_strncmp(value, "true", 5) == 0 || ft_strncmp(value, "TRUE", 5) == 0;
	return FALSE;
}

void	*calloc(size_t n, size_t size)
{
	void	*ptr;

	if (n == 0 || size == 0)
		return (malloc(0));
	if (n > INT_MAX || size > INT_MAX || n * size > INT_MAX)
		return (NULL);
	ptr = (void *)malloc(n * size);
	if (ptr)
		ft_bzero(ptr, n * size);
	return (ptr);
}

void init()
{
    if (data.first)
        return;
    data.first = 1;
	data.debug = is_env_set(ENV_DEBUG);
	data.hexdump = is_env_set(ENV_HEXDUMP);

	pthread_mutex_init(&data.malock, NULL);
	pthread_mutex_init(&data.realock, NULL);
	pthread_mutex_init(&data.freelock, NULL);
	pthread_mutex_init(&data.samlock, NULL);
	pthread_mutex_init(&data.samexlock, NULL);

	print_define();
}

void *malloc(size_t size)
{
    void *addr;

    pthread_mutex_lock(&data.malock);

    init();

    LOG("----- Malloc called -----");
    LOG("size: %ld", size);

	size += 32;

    if (size == 0)
    {
		LOG("Malloc ZERO");
        addr = &data.zero_allocation;
    }
    else if (size <= TINY_SIZE)
	{
		LOG("Malloc TINY");
		addr = browse_all_zones(&data.tiny, TINY_ZONE, size);
	}
    else if (size <= SMALL_SIZE)
	{
		LOG("Malloc SMALL");
		addr = browse_all_zones(&data.small, SMALL_ZONE, size);
	}
    else
	{
		LOG("Malloc LARGE");
		addr = malloc_large(size);
	}
    
    LOG("Returning pointer: %p", addr);
    LOG("");
    pthread_mutex_unlock(&data.malock);

    return addr;
}