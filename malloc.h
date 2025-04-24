#ifndef MALLOC_H
 #define MALLOC_H

#include <sys/mman.h>
#include "printf/ft_printf.h"

struct meta_data
{
    struct meta_data *next;
    size_t size;
};

struct malloc_data
{
    void *tiny;  // pointer to the TINY zone
    void *small; // pointer to the SMALL zone
    void *large; // pointer to the LARGE zone
    size_t data_allocated_count;
    size_t data_freed_count;
    int first;
};

struct zone_data
{
    struct zone_data *next;
    int is_full;
};

static struct malloc_data data = {0};

#define LOG(fmt, ...) ft_printf("[MALLOC] " fmt "\n", ##__VA_ARGS__)
#define LOGLN ft_printf("[MALLOC]\n")
// #define LOG(fmt, ...) 1
// #define LOGLN 1

#define LOG_COLOR_RED "\x1b[38;5;160m"
#define LOG_COLOR_GREEN "\x1b[38;5;40m"
#define LOG_RESET "\x1b[0m"
#define OK LOG_COLOR_GREEN "OK" LOG_RESET
#define FAIL LOG_COLOR_RED "FAIL" LOG_RESET


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

#define TINY_SIZE 128
#define SMALL_SIZE 2048
#define META_DATA_SIZE 16
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define ZONE_DATA_SIZE sizeof(struct zone_data)

// TINY zone size calculation
#define TINY_BLOCK_SIZE (TINY_SIZE + META_DATA_SIZE) // 80
#define TINY_ZONE CEIL(ZONE_DATA_SIZE + (TINY_BLOCK_SIZE * 100), PAGE_SIZE) // 8192
#define TINY_PAGE_NB TINY_ZONE / PAGE_SIZE // 2

// SMALL zone size calculation
#define SMALL_BLOCK_SIZE (SMALL_SIZE + META_DATA_SIZE) // 528
#define SMALL_ZONE CEIL(ZONE_DATA_SIZE + (SMALL_BLOCK_SIZE * 100), PAGE_SIZE) // 53248
#define SMALL_PAGE_NB SMALL_ZONE / PAGE_SIZE // 13

void free(void *ptr);
void *malloc(size_t size);
// void *realloc(void *ptr, size_t size);
void show_alloc_mem();

size_t align_up(size_t size, size_t base);
void *get_user_data_pointer(void *ptr);
size_t get_block_size(void *block);
struct meta_data get_block_meta_data(void *block);
void print_define();

/*
blocks are ogranised like this : [meta_data][user_data]
meta data can be 2 case : 

[meta_data:       [size]][user_data]
[meta_data: [next][size]][user_data]

and the free flag is the last bits of size
*/

#endif