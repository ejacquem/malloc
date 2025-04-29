#include <stdio.h>
#include <stdlib.h>
#include "../malloc.h"

int main() {
  int *ptr = ft_malloc(sizeof(int));
  if (ptr == NULL) { 
    printf("Failed to ft_malloc a single int\n");
    return 1;
  }

  *ptr = 1;
  *ptr = 100;

  ft_free(ptr);

  printf("ft_malloc'd an int, assigned to it, and ft_free'd it\n");

  int *ptr2 = ft_malloc(sizeof(int));
  if (ptr2 == NULL) { 
    printf("Failed to ft_malloc a single int\n");
    return 1;
  }

  *ptr2 = 2;
  *ptr2 = 200;

  ft_free(ptr2);
  printf("ft_malloc'd an int, assigned to it, and ft_free'd it #2\n");

  ft_malloc(1); // Screw up alignment.

  int *ptr3 = ft_malloc(sizeof(int));
  if (ptr3 == NULL) { 
    printf("Failed to ft_malloc a single int\n");
    return 1;
  }

  *ptr3 = 3;
  *ptr3 = 300;

  ft_free(ptr3);
  printf("ft_malloc'd an int, assigned to it, and ft_free'd it #3\n");

  return 0;
}