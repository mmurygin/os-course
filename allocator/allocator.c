#include <stdbool.h>
#include "allocator.h"

typedef struct free_unit {
    struct free_unit * prev;
    struct free_unit * next;
    size_t size;
} FreeUnit;

typedef struct end_header {
    size_t size;
    bool is_free;
} EndHeader;

typedef struct busy_unit {
    size_t size;
} BusyUnit;

static FreeUnit *free_units_head;

size_t free_unit_extras();
size_t busy_unit_extras();

void init_free_unit(FreeUnit *free_unit, size_t available_size);
void init_busy_unit(BusyUnit *busy_unit, size_t size);
FreeUnit * get_free_unit(FreeUnit * header, size_t size);
BusyUnit * split_free_unit(FreeUnit * free_unit, size_t busy_unit_size);

void mysetup(void *buf, size_t size)
{
    free_units_head = (FreeUnit*) buf;

    size_t real_size = size - free_unit_extras();
    init_free_unit(free_units_head, real_size);
}

void *myalloc(size_t size)
{
    size_t required_size = size - free_unit_extras() + busy_unit_extras();
    FreeUnit *free_unit = get_free_unit(free_units_head, required_size);

    if (!free_unit)
    {
        return NULL;
    }

    if (free_unit->size = required_size)
    {
        if (free_unit->prev)
        {
            free_unit->prev->next = free_unit->next;
        }
        else if (free_unit == free_units_head)
        {
            free_units_head = NULL;
        }

        init_busy_unit((BusyUnit*)free_unit, size);
    }

    BusyUnit * busy_unit = split_free_unit(free_unit, size);

    return (char*) busy_unit + sizeof(BusyUnit);
}

void myfree(void *p)
{

}

void create_end_header(void *buffer_end, size_t size, bool is_free)
{
    EndHeader *end_header = (EndHeader*)((char *)buffer_end - sizeof(EndHeader));
    end_header->is_free = is_free;
    end_header->size = size;
}

/*
| *prev | *next | size  | ------------------------- | size | is_free |
*/
void init_free_unit(FreeUnit *free_unit, size_t size)
{
    free_unit->prev = NULL;
    free_unit->next = NULL;
    free_unit->size = size;

    char *buffer_end = (char *)free_unit + size + free_unit_extras();
    create_end_header(buffer_end, size, true);
}

/*
| size | ------------------------- | size | is_free |
*/
void init_busy_unit(BusyUnit * busy_unit, size_t size)
{
    busy_unit->size = size;

    char *buffer_end = (char *)busy_unit + size + busy_unit_extras();
    create_end_header(buffer_end, size, true);
}

FreeUnit * get_free_unit(FreeUnit * free_unit, size_t size)
{
    if (free_unit->size>= size)
    {
        return free_unit;
    }

    if (free_unit->next)
    {
        return get_free_unit(free_unit->next, size);
    }

    return NULL;
}

size_t free_unit_extras()
{
    return sizeof(FreeUnit) + sizeof(EndHeader);
}

size_t busy_unit_extras()
{
    return sizeof(BusyUnit) + sizeof(EndHeader);
}

BusyUnit *split_free_unit(FreeUnit *free_unit, size_t busy_unit_size)
{
    size_t taken_size = busy_unit_size + busy_unit_extras();
    size_t remained_size = free_unit->size - taken_size;

    init_free_unit(free_unit, remained_size);

    BusyUnit *busy_unit = (BusyUnit*)((char*) free_unit + free_unit->size + free_unit_extras());
    init_busy_unit(busy_unit, busy_unit_size);

    return busy_unit;
}
