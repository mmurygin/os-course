#include <stdint.h>

#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define IN 1
#define OUT 0

typedef struct nlist
{
    struct nlist * next;
    uint64_t addr;
    uint64_t value;
} Nlist;

void set_addr_value(uint64_t addr, uint64_t value);
short has_addr(uint64_t addr);
uint64_t get_memory_value(uint64_t  addr);

#endif // MEMORY_MAP_H
