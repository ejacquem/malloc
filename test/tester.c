#include "../malloc.h"
#include "../printf/ft_printf.h"

void test_realloc_bigger()
{
	int *a[10];

	a[0] = malloc(300);
	a[1] = malloc(300);
	free(a[1]);
	show_alloc_mem();
	a[0] = realloc(a[0], 600);
	show_alloc_mem();
}

void test_realloc_smaller()
{
	int *a;

	size_t min_size = 16;
	size_t max_size = 2048;

	a = malloc(max_size);

	for (size_t i = max_size; i > min_size; i /= 2)
	{
		a = realloc(a, i);
		show_alloc_mem();
	}

}

int main()
{
	test_realloc_smaller();
}