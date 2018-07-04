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
static void * left_ptr;
static void * right_ptr;

size_t busy_unit_extras();
size_t free_unit_extras();

void init_free_unit(FreeUnit *free_unit, size_t available_size);
void init_busy_unit(BusyUnit *busy_unit, size_t size);
FreeUnit * get_free_unit(FreeUnit * header, size_t size);
BusyUnit * split_free_unit(FreeUnit * free_unit, size_t busy_unit_size);

void mysetup(void *buf, size_t size)
{
    left_ptr = buf;
    right_ptr = (char*) buf + size;
    free_units_head = (FreeUnit*) buf;

    size_t real_size = size - busy_unit_extras();
    init_free_unit(free_units_head, real_size);
}

void *myalloc(size_t size)
{
    // regarding the task we do not support allocating less than 16b
    if (size < 16)
    {
        return NULL;
    }

    FreeUnit *free_unit = get_free_unit(free_units_head, size);

    if (!free_unit)
    {
        return NULL;
    }

    BusyUnit * busy_unit = NULL;

    if (free_unit->size <= size + free_unit_extras() - busy_unit_extras())
    {
        if (free_unit->prev)
        {
            free_unit->prev->next = free_unit->next;
        }
        else if (free_unit == free_units_head)
        {
            free_units_head = free_unit->next;
        }

        busy_unit = (BusyUnit *)free_unit;
        init_busy_unit(busy_unit, free_unit->size);
    }
    else
    {
        busy_unit = split_free_unit(free_unit, size);
    }

    return (char*) busy_unit + sizeof(BusyUnit);
}

void myfree(void *p)
{
    BusyUnit *busy_unit = (BusyUnit*)((char*)p - sizeof(BusyUnit));

    if (p > left_ptr)
    {
        EndHeader *left_header = (EndHeader*)((char*)busy_unit - sizeof(EndHeader));

        if (left_header->is_free)
        {
            FreeUnit* free_unit = (FreeUnit*)((char*)left_header - left_header->size);
            if (free_unit->prev)
            {
                free_unit->prev->next = free_unit->next;
            }

            busy_unit = (BusyUnit*)free_unit;
            size_t merged_size = free_unit->size + busy_unit->size + busy_unit_extras();
            init_busy_unit(busy_unit, merged_size);
        }
    }

    if (p < right_ptr)
    {
        EndHeader *right_header = (EndHeader*)((char *)busy_unit + busy_unit->size);

        if (right_header->is_free)
        {
            FreeUnit *free_unit = (FreeUnit*)((char *)right_header - right_header->size +
                sizeof(EndHeader));

            if (free_unit->prev)
            {
                free_unit->prev->next = free_unit->next;
            }

            busy_unit = (BusyUnit *)free_unit;
            size_t merged_size = free_unit->size + busy_unit->size + busy_unit_extras();
            init_busy_unit(busy_unit, merged_size);
        }
    }

    FreeUnit *new_free_unit = (FreeUnit*)busy_unit;
    size_t new_free_unit_size = busy_unit->size;

    new_free_unit->size = busy_unit->size;
    new_free_unit->prev = NULL;
    new_free_unit->next = free_units_head;

    free_units_head->prev = new_free_unit;
    free_units_head = new_free_unit;

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

    char *buffer_end = (char *)free_unit + size + busy_unit_extras();
    create_end_header(buffer_end, size, true);
}

/*
| size | ------------------------- | size | is_free |
*/
void init_busy_unit(BusyUnit * busy_unit, size_t size)
{
    busy_unit->size = size;

    char *buffer_end = (char *)busy_unit + size + busy_unit_extras();
    create_end_header(buffer_end, size, false);
}

FreeUnit * get_free_unit(FreeUnit * free_unit, size_t size)
{
    if (free_unit->size >= size)
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

BusyUnit *split_free_unit(FreeUnit *free_unit, size_t size)
{
    size_t taken_size = size + busy_unit_extras();
    size_t remained_size = free_unit->size - taken_size;

    init_free_unit(free_unit, remained_size);

    BusyUnit *busy_unit = (BusyUnit*)((char*) free_unit + free_unit->size + busy_unit_extras());
    init_busy_unit(busy_unit, size);

    return busy_unit;
}
