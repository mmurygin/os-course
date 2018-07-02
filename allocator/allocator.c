#include <stdbool.h>
#include "allocator.h"

typedef struct subheader {
    size_t size;
    bool is_free;
} SubHeader;

typedef struct header {
    void * prev;
    void * next;
    SubHeader subheader;
} Header;

static Header * list_header;
Header * setup_new_unit(void * head, size_t size, bool is_free);

// Эта функция будет вызвана перед тем как вызывать myalloc и myfree
// используйте ее чтобы инициализировать ваш аллокатор перед началом
// работы.
//
// buf - указатель на участок логической памяти, который ваш аллокатор
//       должен распределять, все возвращаемые указатели должны быть
//       либо равны NULL, либо быть из этого участка памяти
// size - размер участка памяти, на который указывает buf
void mysetup(void *buf, size_t size)
{
    list_header = setup_new_unit(buf, size, true);
}

// Функция аллокации
void *myalloc(size_t size)
{
}

// Функция освобождения
void myfree(void *p)
{
}

Header * setup_new_unit(void * head, size_t size, bool is_free)
{
    size_t real_size = size - sizeof(Header) - sizeof(SubHeader);

    Header * start_header = head;
    start_header->prev = NULL;
    start_header->next = NULL;
    start_header->subheader.is_free = is_free;
    start_header->subheader.size = real_size;

    SubHeader * end_header = (SubHeader*)((char*) head + size - sizeof(SubHeader));
    end_header->is_free = is_free;
    end_header->size = real_size;

    return start_header;
}
