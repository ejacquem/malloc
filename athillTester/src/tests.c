#include "my_malloc_tester.h"
#include <errno.h>
#include <string.h>
#include <time.h>

long timespec_diff_millis(struct timespec begin, struct timespec end) {
    return ((long)end.tv_nsec - (long)begin.tv_nsec) / 1000000L
        + ((long)end.tv_sec  - (long)begin.tv_sec) * 1000L;
}

int run_test(int size, int count) {
    struct timespec begin, end;
    void *ptrs[count];
    int total = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    for (int i = 0; i < count; ++i) {
        void *ptr = malloc(size);
        if (ptr == NULL) {
            ft_printf("failed to alloc: %s\n", strerror(errno));
            return 1;
        }
        if (size >= (int)sizeof(int)) {
            *(int *)ptr = i;
            ptrs[i] = ptr;
        }
    }

    for (int i = 0; i < count; ++i) {
        total += *(int *)ptrs[i];
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    int time = timespec_diff_millis(begin, end);
    ft_printf("%i allocs of size %i took %i ms\n", count, size, time);
    ft_printf("sum = %i\n", total);

    return 0;
}
