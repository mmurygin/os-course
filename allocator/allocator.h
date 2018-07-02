#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

void mysetup(void *buf, size_t size);

void *myalloc(size_t size);

void myfree(void *p);

#endif
