#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

int main()
{
    size_t buf_size = 100 * 1024;
    void * buf = malloc(buf_size);

    printf("Initial range is [%p, %p]\n", buf, (char*) buf + buf_size);

    void *pointers[10] = { NULL };

    for (int i = 0; i < 10; i++)
    {
        pointers[i] = myalloc(i * 100);
        printf("%p\n", pointers[i]);
    }

    for (int i = 0; i < 10; i++)
    {
        myfree(pointers[i]);
    }

    free(buf);

    return 0;
}
