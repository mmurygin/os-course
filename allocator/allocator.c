#include <stdbool.h>
#include "allocator.h"

typedef struct free_unit {
    FreeUnit * prev;
    FreeUnit * next;
    Header header;
} FreeUnit;

typedef struct header
{
    size_t size;
    bool is_free;
} Header;

static FreeUnit *free_units_head;

size_t get_extra_size();
void init_free_unit(FreeUnit * free_unit, size_t size);
FreeUnit * get_free_unit(FreeUnit * header, size_t size);
Header * split_unit(FreeUnit * free_unit, size_t new_unit_size);

void mysetup(void *buf, size_t size)
{
    free_units_head = (FreeUnit*) buf;
    init_free_unit(free_units_head, size);
}

void *myalloc(size_t size)
{
    FreeUnit * free_unit = get_free_unit(free_units_head, size);

    if (!free_unit)
    {
        return NULL;
    }

    if (free_unit->header.size = size)
    {
        // TODO(MM): handle if free_unit == free_units_head

    }

    Header * new_unit = split_unit(free_unit, size);
    return (char*) new_unit + sizeof(Header);
}

void myfree(void *p)
{

}

/*
| *prev | *next | size | is_free | ------------------------- | size | is_free |
*/
void init_free_unit(FreeUnit * free_unit, size_t size)
{
    size_t real_size = size - get_extra_size();

    free_unit->prev = NULL;
    free_unit->next = NULL;
    free_unit->header.is_free = true;
    free_unit->header.size = real_size;

    Header *header = (Header *)((char *)free_unit + size - sizeof(FreeUnit));
    header->is_free = true;
    header->size = real_size;
}

/*
| size | is_free | ------------------------- | size | is_free |
*/
void init_busy_unit(Header * busy_unit, size_t size)
{
    busy_unit->is_free = false;
    busy_unit->size = size;

    Header *end_header = (Header*)((char*)busy_unit + size + sizeof(Header));
    end_header->is_free = false;
    end_header->size = size;
}

FreeUnit * get_free_unit(FreeUnit * free_unit, size_t size)
{
    if (free_unit->header.size >= size)
    {
        return free_unit;
    }

    if (free_unit->next)
    {
        return get_free_unit(free_unit->next, size);
    }

    return NULL;
}

size_t get_extra_size()
{
    return sizeof(FreeUnit) + sizeof(Header);
}

Header *split_unit(FreeUnit *free_unit, size_t new_unit_size)
{
    size_t taken_size = new_unit_size + get_extra_size();
    size_t remained_size = free_unit->header.size - taken_size;

    init_free_unit(free_unit, remained_size);

    Header *busy_unit = (Header*)((char *)free_unit + free_unit->header.size - taken_size);
    init_busy_unit(busy_unit, new_unit_size);

    return busy_unit;
}
