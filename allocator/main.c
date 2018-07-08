#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

void test_sequential_allocation();
void test_alloc_free();
void test_random_alloc_malloc();

int main()
{
    size_t buf_size = 100000;
    void * buf = malloc(buf_size);

    printf("Initial range is [%p, %p]\n", buf, (char*) buf + buf_size);

    mysetup(buf, buf_size);

    // test_alloc_free();
    // test_sequential_allocation();
    test_random_alloc_malloc();

    free(buf);

    return 0;
}

#define SEQUENTIAL_ATTEMPTS 10
void test_sequential_allocation()
{
    void *pointers[SEQUENTIAL_ATTEMPTS] = { NULL };

    for (int i = 1; i < SEQUENTIAL_ATTEMPTS; i++)
    {
        pointers[i] = myalloc(i * 100);
        printf("alloc: %p\n", pointers[i]);
    }

    for (int i = 0; i < SEQUENTIAL_ATTEMPTS; i++)
    {
        int index = rand() % SEQUENTIAL_ATTEMPTS;
        myfree(pointers[index]);
        printf("free: [%d: %p]\n", index, pointers[index]);
        pointers[index] = NULL;
    }
}

void test_alloc_free()
{
    void *ptr = myalloc(10000);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);

    ptr = myalloc(5000);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);

    ptr = myalloc(2500);
    printf("alloc: %p\n", ptr);
    myfree(ptr);
    printf("free: %p\n", ptr);
}

#define RANDOM_ATTEMPTS 100
static void * allocated[RANDOM_ATTEMPTS] = { NULL };

void free_by_index(int index)
{
    printf("Started Freeing: %p\n", allocated[index]);
    myfree(allocated[index]);
    printf("Finished Freeing: %p\n", allocated[index]);
    allocated[index] = NULL;
}

void test_random_alloc_malloc()
{
    for (int i = 0; i < RANDOM_ATTEMPTS; i++)
    {
        int alloc_number = rand() % 4;
        int index = rand() % RANDOM_ATTEMPTS;

        if (allocated[index])
        {
            free_by_index(index);
        }

        if (alloc_number > 0)
        {
            size_t size = rand() % 1000;
            printf("Started Allocating: %d bytes\n", (int)size);
            allocated[index] = myalloc(size);
            printf("Finished Allocating %d bytes. Result: [%p]\n", (int)size, allocated[index]);
        }
    }
}
