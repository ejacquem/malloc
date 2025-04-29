# include "../ft_malloc.h"

#define M (1024 * 1024)

int main()
{
    void* a = ft_malloc(1);
    void* b = ft_malloc(2);
    void* c = ft_malloc(4);
    void* d = ft_malloc(8);
    void* e = ft_malloc(16);
    void* f = ft_malloc(32);
    void* g = ft_malloc(64);
    void* h = ft_malloc(128);
    void* i = ft_malloc(256);
    void* j = ft_malloc(512);
    void* k = ft_malloc(1024);
    void* l = ft_malloc(1024 * 2);
    void* m = ft_malloc(1024 * 4); 
    void* n = ft_malloc(1024 * 32);
    void* o = ft_malloc(M);
    void* p = ft_malloc(16*M);
    void* q = ft_malloc(128*M);
    show_alloc_mem(); 
    return (0); 
}