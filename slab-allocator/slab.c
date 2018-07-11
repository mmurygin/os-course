#include <stdio.h>
#include <math.h>
#include "slab.h"

/**
 * Аллоцирует участок размером 4096 * 2^order байт,
 * выровненный на границу 4096 * 2^order байт. order
 * должен быть в интервале [0; 10] (обе границы
 * включительно), т. е. вы не можете аллоцировать больше
 * 4Mb за раз.
 **/
void *alloc_slab(int order)
{
    if (order < 0 || order > 10)
    {
        puts("Order should be between [0, 10]");
        exit(1);
    }

    return malloc(PAGE_SIZE * pow(2, order));
}

/**
 * Освобождает участок ранее аллоцированный с помощью
 * функции alloc_slab.
 **/
void free_slab(void *slab)
{
    free(slab);
}

size_t get_objects_in_slab(int order, size_t object_size)
{
    return PAGE_SIZE * pow(2, order) / object_size;
}
