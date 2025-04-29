#ifndef MALLOC2_H
 #define MALLOC2_H

#include <sys/mman.h>
#include "printf/ft_printf.h"

struct meta_data
{
    size_t size;            // total size of the block (meta + usable) - contains free flag in first bit 
    size_t user_size;       // size given to malloc
    struct meta_data *next;
}typedef meta_data_t;


struct block_info
{
    size_t block_size;
    size_t user_size;
    size_t usable_size;
    void* user_ptr;
    char isfree;
}typedef block_info_t;

struct malloc_data
{
    void *tiny;  // pointer to the TINY zone
    void *small; // pointer to the SMALL zone
    void *large; // pointer to the LARGE zone
    size_t data_allocated_count;
    size_t data_freed_count;
    void *zero_allocation;
    int first;
};

struct zone_data
{
    struct zone_data *next;
    int is_full;
};

extern struct malloc_data data;

#define DEBUG

#ifdef DEBUG
    #define LOG(fmt, ...) ft_printf("[MALLOC] " fmt "\n", ##__VA_ARGS__)
    #define LOGLN ft_printf("[MALLOC]\n")
#else
    #define LOG(fmt, ...) 1
    #define LOGLN 1
#endif

#define RED "\x1b[38;5;160m"
#define GREEN "\x1b[38;5;40m"
#define RESET "\x1b[0m"
#define OK GREEN "OK" RESET
#define FAIL RED "FAIL" RESET

#define CEIL(x, base) ((x + base - 1) & ~(base - 1))

// flag manipulation

#define SIZE_MASK       (__SIZE_MAX__ >> 1) // == 0b011111...
#define SET_NOT_FREE(size)  ((size) & SIZE_MASK)
#define SET_FREE(size)  ((size) | ~SIZE_MASK)
#define GET_SIZE(size)  ((size) &  SIZE_MASK)
#define IS_FREE(size)   (((size) & ~SIZE_MASK) == ~SIZE_MASK)

/*
 type| size| block_size| zone_size| z_size(ceil)| page_nb
 TINY|   64|         80|      8000|         8192|       2
SMALL|  512|        528|     52800|        53248|      13
*/

#define TINY_SIZE 128
#define SMALL_SIZE 512
#define META_DATA_SIZE sizeof(struct meta_data)
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define ZONE_DATA_SIZE sizeof(struct zone_data)

// TINY zone size calculation
#define TINY_BLOCK_SIZE (TINY_SIZE + META_DATA_SIZE)
#define TINY_ZONE CEIL(ZONE_DATA_SIZE + (TINY_BLOCK_SIZE * 100), PAGE_SIZE)
#define TINY_PAGE_NB TINY_ZONE / PAGE_SIZE

// SMALL zone size calculation
#define SMALL_BLOCK_SIZE (SMALL_SIZE + META_DATA_SIZE)
#define SMALL_ZONE CEIL(ZONE_DATA_SIZE + (SMALL_BLOCK_SIZE * 100), PAGE_SIZE)
#define SMALL_PAGE_NB SMALL_ZONE / PAGE_SIZE

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();

size_t align_up(size_t size, size_t base);
void print_define();

/*
blocks are ogranised like this : [meta_data][user_data]
meta data can be 2 case : 

[meta_data:       [size]][user_data]
[meta_data: [next][size]][user_data]

and the free flag is the last bits of size
*/

#endif