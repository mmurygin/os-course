#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "include/memory-map.h"

#define HASH_TABLE_SIZE 1000

static Nlist *hashtable[HASH_TABLE_SIZE] = {};

void _set_addr_value(Nlist *nlist, uint64_t addr, uint64_t value);
uint64_t _get_memory_value(Nlist *nlist, uint64_t addr);

unsigned _get_hash(uint64_t x)
{
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

unsigned _get_bucket(uint64_t addr)
{
    return _get_hash(addr) % HASH_TABLE_SIZE;
}

short _has_addr(const Nlist *nlist, uint64_t addr)
{
    if (nlist == NULL)
    {
        return OUT;
    }

    if (nlist->addr == addr)
    {
        return IN;
    }

    return _has_addr(nlist->next, addr);
}

short has_addr(uint64_t addr)
{
    unsigned bucket = _get_bucket(addr);

    return _has_addr(hashtable[bucket], addr);
}

uint64_t _get_memory_value(Nlist *nlist, uint64_t addr)
{
    if (!nlist)
    {
        fprintf(stderr, "Try to get not existed addr: %" SCNu64 "\n", addr);
        exit(1);
    }

    if (nlist->addr == addr)
    {
        return nlist->value;
    }

    return _get_memory_value(nlist->next, addr);
}

uint64_t get_memory_value(uint64_t addr)
{
    unsigned bucket = _get_bucket(addr);

    return _get_memory_value(hashtable[bucket], addr);
}

Nlist *_allocnlist(uint64_t addr, uint64_t value)
{
    Nlist *nlist = malloc(sizeof(Nlist));
    nlist->next = NULL;
    nlist->addr = 0;
    nlist->value = 0;

    return nlist;
}

void _set_addr_value(Nlist *nlist, uint64_t addr, uint64_t value)
{
    if (nlist->next)
    {
        _set_addr_value(nlist->next, addr, value);
    }
    else
    {
        nlist->next = _allocnlist(addr, value);
    }
}

void set_addr_value(uint64_t addr, uint64_t value)
{
    unsigned bucket = _get_bucket(addr);
    Nlist *nlist = hashtable[bucket];

    if (!nlist)
    {
        hashtable[bucket] = _allocnlist(addr, value);
        return;
    }

    _set_addr_value(nlist, addr, value);
}
