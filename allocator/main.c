#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

void test_sequential_allocation();
void test_alloc_free();

int main()
{
    size_t buf_size = 1000;
    void * buf = malloc(buf_size);

    printf("Initial range is [%p, %p]\n", buf, (char*) buf + buf_size);

    mysetup(buf, buf_size);

    test_alloc_free();
    test_sequential_allocation();

    free(buf);

    return 0;
}

#define ATTEMPTS 3

void test_sequential_allocation()
{
    void *pointers[ATTEMPTS] = { NULL };

    for (int i = 0; i < ATTEMPTS; i++)
    {
        pointers[i] = myalloc(i * 100);
        printf("alloc: %p\n", pointers[i]);
    }

    for (int i = 0; i < ATTEMPTS; i++)
    {
        myfree(pointers[i]);
        printf("free: %p\n", pointers[i]);
    }
}

void test_alloc_free()
{
    void *ptr = myalloc(1000000);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);

    ptr = myalloc(500000);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);

    ptr = myalloc(250000);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);
}
