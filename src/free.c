#include "../malloc.h"

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

void free_small(void *ptr, void **zone_list, size_t zone_size)
{
    size_t *block = ((size_t *)ptr) - 1UL; // step back 8 bytes

    if (IS_FREE(*block))
        return LOG("Pointer already free !"), (void)0;

    *block = SET_FREE(*block);
    struct zone_data *zone = get_block_zone(*zone_list, zone_size, block);
    zone->alloc_count--;
    // zone->is_full = FALSE;
    if (zone->alloc_count == 0 && empty_zone_count(*zone_list) >= 2UL) // if zone is empty and at least one other empty zone exists
    // if (zone->alloc_count == 0 && (void *)zone != *zone_list) // if its not the first zone
    {
        LOG(" ------------------------------------------ Removing a small zone");
        // LOG("empty_zone_count(*zone_list): %ld", empty_zone_count(*zone_list));
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

void free(void *ptr)
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