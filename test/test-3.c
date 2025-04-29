#include <stdio.h>
#include <stdlib.h>
#include "../malloc.h"

// #define START_MALLOC_SIZE 1024*1024*128
// #define STOP_MALLOC_SIZE  1024

#define START_MALLOC_SIZE 1024*1024
#define STOP_MALLOC_SIZE  1024

void dummy() { return; }

void *reduce(void *ptr, int size) {
  if (size > STOP_MALLOC_SIZE) {
    void *ptr1 = ft_realloc(ptr, size / 2);
    void *ptr2 = ft_malloc(size / 2);

    if (ptr1 == NULL || ptr2 == NULL) {
      printf("Memory failed to allocate!\n");
      exit(1);
    }

    ptr1 = reduce(ptr1, size / 2);
    ptr2 = reduce(ptr2, size / 2);

    if (*((int *)ptr1) != size / 2 || *((int *)ptr2) != size / 2) {
        printf("*((int *)ptr1) : %d\n", *((int *)ptr1));
        printf("*((int *)ptr2) : %d\n", *((int *)ptr2));
        printf("size / 2 : %d\n", size / 2);
      printf("Memory failed to contain correct data after many allocations!\n");
      exit(2);
    }

    void *old_ptr1 = ptr1;
    ptr1 = ft_realloc(ptr1, size);
    ft_free(ptr2);

    if (*((int *)ptr1) != size / 2) {
      printf("Memory failed to contain correct data after ft_realloc()!\n");
      printf("Expected %i found %i (old %i)\n", (size/2), *((int *)ptr1), *((int *)old_ptr1));
      dummy();
      exit(3);
    }

    *((int *)ptr1) = size;
    return ptr1;
  } else {
    *((int *)ptr) = size;
    return ptr;
  }
}

int main() {
  ft_malloc(1);
	
  int size = START_MALLOC_SIZE;
  while (size > STOP_MALLOC_SIZE)	{
    void *ptr = ft_malloc(size);
    printf("ptr: %p\n", ptr);fflush(stdout);
    ptr = reduce(ptr, size / 2);
    ft_free(ptr);
		
    size /= 2;
  }

  printf("Memory was allocated, used, and ft_freed!\n");	
  return 0;
}