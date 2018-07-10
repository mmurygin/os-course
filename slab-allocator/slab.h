#ifndef SLAB_H
#define SLAB_H

#define PAGE_SIZE 4096

void *alloc_slab(int order);
void free_slab(void *slab);

#endif
