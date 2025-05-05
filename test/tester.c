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

	show_alloc_mem();

	a = malloc(max_size);
	show_alloc_mem();

	for (size_t i = max_size; i > min_size; i /= 2)
	{
		ft_printf("reallocating %ld bytes\n", i);
		a = realloc(a, i);
		show_alloc_mem();
	}
}

void test_malloc_random()
{
	const int malloc_nb = 15;
	const int min_size = 256;
	const int max_size = 512;
	for (int i = 0; i <= malloc_nb; i++)
	{
		int size = (rand() % (max_size - min_size + 1)) + min_size;
		malloc(size);
	}
	show_alloc_mem();
}

void test_malloc_hexdump()
{
	char *a = malloc(8);
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	a[3] = 4;
	a[4] = 5;
	a[5] = 6;
	a[6] = 7;
	a[7] = 8;
	void *b = malloc(17);
	malloc(137);
	malloc(88);
	malloc(1370);
	free(b);
	show_alloc_mem_ex();
}

void test_malloc_chardump()
{
	char *a = malloc(14);
	ft_memcpy(a, "Hello world !", 14);
	show_alloc_mem_ex();
}

void test_malloc_multiple_zone()
{
	for (int i = 0; i < 200; i++)
	{
		malloc(128);
	}
	show_alloc_mem();
	show_alloc_mem_ex();
}

void test_malloc_multiple_zone_fill()
{
	for (int i = 0; i < 2000; i++)
	{
		size_t size = 128;
		char *a = malloc(size);
		for (size_t j = 0; j < size; j++)
		{
			a[j] = 1;
		}
		realloc(a, 64);
	}
	show_alloc_mem_ex();
}

int main()
{
	// test_realloc_smaller();
	// test_malloc_random();
	// test_malloc_chardump();
	// test_malloc_multiple_zone();
	test_malloc_multiple_zone_fill();
}