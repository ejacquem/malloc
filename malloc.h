#ifndef MALLOC_H
 #define MALLOC_H

#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "printf/ft_printf.h"

enum SAM_format
{
	BASIC,
	CUSTOM,
	HEXDUMP
};

struct s_meta_data
{
    // size is the user allocated size
    // the first bit is used as a flag for free
    // needs to be at the end of the l_meta_data
    size_t size;
};

struct l_meta_data
{
    struct l_meta_data *next;

    // size is the user allocated size
    // the first bit is used as a flag for free
    // needs to be at the end of the l_meta_data
    size_t size;
};

struct malloc_data
{
    void *tiny;  // pointer to the TINY zone
    void *small; // pointer to the SMALL zone
    void *large; // pointer to the LARGE zone
    struct l_meta_data *last_large; // pointer to the LARGE zone
    void *zero_allocation; // dummy pointer if malloc(0) is called
	enum SAM_format sam_format; // show_alloc_mem format 
    char first;   // bool to init once
	char debug;   // bool to print debug
	char hexdump; // bool to print hexdump
    // mutex lock
    pthread_mutex_t malock;
    pthread_mutex_t realock;
    pthread_mutex_t freelock;
    pthread_mutex_t samlock;
    pthread_mutex_t samexlock;
};

struct zone_data
{
    struct zone_data *next;
    size_t alloc_count;
    // char is_full; // tells if the zone can't have more blocks. but it can be full and have free space
};

extern struct malloc_data data;

#define DEBUG

#ifdef DEBUG
    // #define LOG(fmt, ...) ft_printf("[MALLOC] " fmt "\n", ##__VA_ARGS__)
	#define LOG(fmt, ...) ((data.debug) ? ft_printf("[MALLOC] " fmt "\n", ##__VA_ARGS__) : 1)
#else
    #define LOG(fmt, ...) 1
#endif

#define LOG_COLOR_RED "\x1b[38;5;160m"
#define LOG_COLOR_GREEN "\x1b[38;5;40m"
#define LOG_RESET "\x1b[0m"
#define OK LOG_COLOR_GREEN "OK" LOG_RESET
#define FAIL LOG_COLOR_RED "FAIL" LOG_RESET

#define ENV_DEBUG "MALLOC_DEBUG"
#define ENV_HEXDUMP "MALLOC_HEXDUMP"

#define CEIL(x, base) ((x + base - 1) & ~(base - 1))
#define MIN(a, b) (a < b ? a : b)

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
#define SMALL_SIZE 1024
#define S_META_DATA_SIZE sizeof(struct s_meta_data)
#define L_META_DATA_SIZE sizeof(struct l_meta_data)
#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define ZONE_DATA_SIZE sizeof(struct zone_data)

// TINY zone size calculation
#define TINY_BLOCK_SIZE align_up(TINY_SIZE + S_META_DATA_SIZE, 16)
#define TINY_ZONE CEIL(ZONE_DATA_SIZE + (TINY_BLOCK_SIZE * 100), PAGE_SIZE)
#define TINY_PAGE_NB TINY_ZONE / PAGE_SIZE

// SMALL zone size calculation
#define SMALL_BLOCK_SIZE align_up(SMALL_SIZE + S_META_DATA_SIZE, 16)
#define SMALL_ZONE CEIL(ZONE_DATA_SIZE + (SMALL_BLOCK_SIZE * 100), PAGE_SIZE)
#define SMALL_PAGE_NB SMALL_ZONE / PAGE_SIZE

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

void show_alloc_mem();
void show_alloc_mem_ex();
void print_block(void *block, size_t block_size, size_t alloc_size, int is_free);
void print_large(size_t *sum, void *zone);
void print_zone(size_t *sum, void *zone, size_t zone_size);

void *replace_block(void *block, size_t size);
void join_next_block(void *block);
void defragment_memory(void *block);
struct zone_data *get_block_zone(struct zone_data *list, size_t zone_size, void *ptr);


size_t align_up(size_t size, size_t base);
void *get_user_data_pointer(void *ptr);
size_t get_block_data(void *block);
size_t get_block_size(size_t size, size_t meta_data_size);
size_t get_block_usable_size(size_t size, size_t meta_data_size);
struct l_meta_data get_block_meta_data(void *block);
void *get_zone_usr_data_ptr(void *zone);
void print_define();

/*
blocks are ogranised like this : [meta_data][user_data]
meta data can be 2 case : 

[meta_data:       [size]][user_data]
[meta_data: [next][size]][user_data]

and the free flag is the last bits of size
*/

#endif