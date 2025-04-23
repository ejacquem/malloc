#ifndef MALLOC_H
 #define MALLOC_H

#include <sys/mman.h>
#include "printf/ft_printf.h"

#define LOG(fmt, ...) ft_printf("[MALLOC] " fmt "\n", ##__VA_ARGS__)
// #define LOG(fmt, ...) 1

#define CEIL(x, base) ((x + base - 1) & ~(base - 1))

// flag manipulation
#define SET_FREE(size) ((size) | 0x1)
#define SET_ALLOC(size) ((size) & ~0x1)
#define IS_FREE(size) ((size) & 0x1) == 1

/*
 type| size| block_size| zone_size| z_size(ceil)| page_nb
 TINY|   64|         80|      8000|         8192|       2
SMALL|  512|        528|     52800|        53248|      13
*/

#define TINY_SIZE 64
#define SMALL_SIZE 512
#define META_DATA_SIZE 16
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

// TINY zone size calculation
#define TINY_BLOCK_SIZE (TINY_SIZE + META_DATA_SIZE) // 80
#define TINY_ZONE CEIL((TINY_BLOCK_SIZE * 100), PAGE_SIZE) // 8192
#define TINY_PAGE_NB TINY_ZONE / PAGE_SIZE // 2

// SMALL zone size calculation
#define SMALL_BLOCK_SIZE (SMALL_SIZE + META_DATA_SIZE) // 528
#define SMALL_ZONE CEIL((SMALL_BLOCK_SIZE * 100), PAGE_SIZE) // 53248
#define SMALL_PAGE_NB SMALL_ZONE / PAGE_SIZE // 13

void free(void *ptr);
void *malloc(size_t size);
// void *realloc(void *ptr, size_t size);

/*
blocks are ogranised like this : [meta_data][user_data]
meta data can be 2 case : 

[meta_data:       [size]][user_data]
[meta_data: [next][size]][user_data]

and the free flag is the last bits of size
*/

struct meta_data
{
    struct meta_data *next;
    size_t size;
};

struct malloc_data
{
    void *tiny; // pointer to the TINY zone
    void *small;// pointer to the SMALL zone
    void *big;  // pointer to the LARGE zone
};

#endif