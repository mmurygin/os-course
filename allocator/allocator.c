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
static size_t min_size;
static void * left_border;
static void * right_border;

size_t busy_unit_extras();
size_t free_unit_extras();

void init_free_unit(FreeUnit *free_unit, size_t size, FreeUnit * prev, FreeUnit *next);
void init_busy_unit(Header *busy_unit, size_t size);
void merge_right_unit(Header *busy_unit);
Header * merge_left_unit(Header *busy_unit);
FreeUnit *get_free_unit(FreeUnit *header, size_t size);
Header * split_free_unit(FreeUnit * free_unit, size_t busy_unit_size);
void unlink_free_unit(const FreeUnit *free_unit);

void mysetup(void *buf, size_t size)
{
    free_units_head = (FreeUnit*) buf;
    min_size = free_unit_extras() - busy_unit_extras();
    left_border = buf;
    right_border = (char*) buf + size;

    size_t real_size = size - busy_unit_extras();
    init_free_unit(free_units_head, real_size, NULL, NULL);
}

void *myalloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }

    if (size < min_size)
    {
        size = min_size;
    }

    FreeUnit *free_unit = get_free_unit(free_units_head, size);

    if (!free_unit)
    {
        return NULL;
    }

    Header *busy_unit = NULL;

    if (free_unit->header.size <= size + min_size + busy_unit_extras())
    {
        unlink_free_unit(free_unit);

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
    busy_unit = merge_left_unit(busy_unit);
    merge_right_unit(busy_unit);

    FreeUnit *new_free_unit = (FreeUnit*)busy_unit;
    init_free_unit(new_free_unit, busy_unit->size, NULL, NULL);

    new_free_unit->next = free_units_head;
    if (free_units_head)
    {
        free_units_head->prev = new_free_unit;
    }
    free_units_head = new_free_unit;
}

Header * merge_left_unit(Header *busy_unit)
{
    Header *left_header = (Header *)((char *)busy_unit - sizeof(Header));
    if ((void*)left_header > left_border && left_header->is_free)
    {
        FreeUnit *free_unit = (FreeUnit *)((char *)left_header - left_header->size - sizeof(Header));
        unlink_free_unit(free_unit);

        size_t merged_size = free_unit->header.size + busy_unit->size + busy_unit_extras();

        Header * merged_unit = (Header *)free_unit;
        init_busy_unit(merged_unit, merged_size);

        return merged_unit;
    }

    return busy_unit;
}

void merge_right_unit(Header * busy_unit)
{
    Header *right_header = (Header *)((char *)busy_unit + busy_unit->size + busy_unit_extras());

    if ((void*)right_header < right_border && right_header->is_free)
    {
        FreeUnit *free_unit = (FreeUnit *)right_header;

        unlink_free_unit(free_unit);

        size_t merged_size = free_unit->header.size + busy_unit->size + busy_unit_extras();

        init_busy_unit(busy_unit, merged_size);
    }
}

void create_end_header(const Header *start_header)
{
    Header *end_header = (Header*)((char*)start_header + sizeof(Header) + start_header->size);
    end_header->size = start_header->size;
    end_header->is_free = start_header->is_free;
}

/*
| size | is_free | *prev | *next |  ------------------------- | size | is_free |
*/
void init_free_unit(FreeUnit *free_unit, size_t size, FreeUnit *prev, FreeUnit *next)
{
    free_unit->prev = prev;
    free_unit->next = next;
    free_unit->header.size = size;
    free_unit->header.is_free = true;

    create_end_header(&(free_unit->header));
}

/*
| size| is_free | ------------------------- | size | is_free |
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

    init_free_unit(free_unit, remained_size, free_unit->prev, free_unit->next);

    Header *busy_unit = (Header*)((char*)free_unit + free_unit->header.size + busy_unit_extras());
    init_busy_unit(busy_unit, size);

    return busy_unit;
}

void unlink_free_unit(const FreeUnit *free_unit)
{
    if (free_unit->next)
    {
        free_unit->next->prev = free_unit->prev;
    }

    if (free_unit->prev)
    {
        free_unit->prev->next = free_unit->next;
    }
    else
    {
        free_units_head = free_unit->next;
    }
}
