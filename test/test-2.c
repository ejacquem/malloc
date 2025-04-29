#include <stdio.h>
#include <stdlib.h>
#include "../malloc.h"

#define TOTAL_ALLOCS 200000
#define ALLOC_SIZE 1024*1024

int main() {
  ft_malloc(1);
	
  int i;
  void *ptr = NULL;

  for (i = 0; i < TOTAL_ALLOCS; i++) {
    ptr = ft_malloc(ALLOC_SIZE);
    if (ptr == NULL) {
      printf("Memory failed to allocate!\n");
      return 1;
    }
		
    ft_free(ptr);
  }

  printf("Memory was allocated and ft_freed!\n");	
  return 0;
}