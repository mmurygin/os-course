#ifndef SLAB_H
#define SLAB_H

#include <stddef.h>

#define PAGE_SIZE 4096

void *alloc_slab(int order);
void free_slab(void *slab);
size_t get_objects_in_slab(int order);

#endif
