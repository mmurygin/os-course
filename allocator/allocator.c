#include <stdbool.h>
#include <stdlib.h>

typedef struct header
{
    size_t size;
    bool is_free;
} Header;

typedef struct free_unit
{
    Header header;
    struct free_unit * prev;
    struct free_unit * next;
} FreeUnit;

static FreeUnit *free_units_head;
static void * left_ptr;
static void * right_ptr;

size_t busy_unit_extras();
size_t free_unit_extras();

void init_free_unit(FreeUnit *free_unit, size_t size);
void init_busy_unit(Header *busy_unit, size_t size);
FreeUnit * get_free_unit(FreeUnit * header, size_t size);
Header * split_free_unit(FreeUnit * free_unit, size_t busy_unit_size);

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
    if (size <= 16)
    {
        return NULL;
    }

    FreeUnit *free_unit = get_free_unit(free_units_head, size);

    if (!free_unit)
    {
        return NULL;
    }

    Header *busy_unit = NULL;

    if (free_unit->header.size <= size + free_unit_extras() - busy_unit_extras())
    {
        if (free_unit->prev)
        {
            free_unit->prev->next = free_unit->next;
        }
        else if (free_unit == free_units_head)
        {
            free_units_head = free_unit->next;
        }

        busy_unit = (Header *)free_unit;
        init_busy_unit(busy_unit, free_unit->header.size);
    }
    else
    {
        busy_unit = split_free_unit(free_unit, size);
    }

    return (char*) busy_unit + sizeof(Header);
}

void myfree(void *p)
{
    if (p == NULL)
    {
        return;
    }

    Header *busy_unit = (Header*)((char*)p - sizeof(Header));
    if (p > left_ptr)
    {
        Header *left_header = (Header*)((char*)busy_unit - sizeof(Header));

        if (left_header->is_free)
        {
            FreeUnit* free_unit = (FreeUnit*)((char*)left_header - left_header->size);
            if (free_unit->prev)
            {
                free_unit->prev->next = free_unit->next;
            }

            size_t merged_size = free_unit->header.size + busy_unit->size + busy_unit_extras();

            busy_unit = (Header*)free_unit;
            init_busy_unit(busy_unit, merged_size);
        }
    }

    if (p < right_ptr)
    {
        Header *right_header = (Header*)((char *)busy_unit + busy_unit->size +
            busy_unit_extras());

        if ((void*)right_header < right_ptr && right_header->is_free)
        {
            FreeUnit *free_unit = (FreeUnit *)right_header;

            if (free_unit->prev)
            {
                free_unit->prev->next = free_unit->next;
            }

            size_t merged_size = free_unit->header.size + busy_unit->size + busy_unit_extras();

            busy_unit = &free_unit->header;
            init_busy_unit(busy_unit, merged_size);
        }
    }

    FreeUnit *new_free_unit = (FreeUnit*)busy_unit;
    init_free_unit(new_free_unit, busy_unit->size);

    new_free_unit->next = free_units_head;
    free_units_head->prev = new_free_unit;
    free_units_head = new_free_unit;
}

void create_end_header(const Header *start_header)
{
    Header *end_header = (Header*)((char*)start_header + sizeof(Header) + start_header->size);
    end_header->size = start_header->size;
    end_header->is_free = start_header->is_free;
}

/*
| *prev | *next | size  | ------------------------- | size | is_free |
*/
void init_free_unit(FreeUnit *free_unit, size_t size)
{
    free_unit->prev = NULL;
    free_unit->next = NULL;
    free_unit->header.size = size;
    free_unit->header.is_free = true;

    create_end_header(&free_unit->header);
}

/*
| size | ------------------------- | size | is_free |
*/
void init_busy_unit(Header *busy_unit, size_t size)
{
    busy_unit->size = size;
    busy_unit->is_free = false;

    create_end_header(busy_unit);
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

size_t free_unit_extras()
{
    return sizeof(FreeUnit) + sizeof(Header);
}

size_t busy_unit_extras()
{
    return 2 * sizeof(Header);
}

Header *split_free_unit(FreeUnit *free_unit, size_t size)
{
    size_t taken_size = size + busy_unit_extras();
    size_t remained_size = free_unit->header.size - taken_size;

    init_free_unit(free_unit, remained_size);

    Header *busy_unit = (Header*)((char*)free_unit + free_unit->header.size + busy_unit_extras());
    init_busy_unit(busy_unit, size);

    return busy_unit;
}
