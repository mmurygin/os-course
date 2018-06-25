#include "include/mmu.h"
#include "include/memory-map.h"

static uint64_t _root_table_ptr;

void set_root_table_addr(uint64_t root_table_ptr)
{
    _root_table_ptr = root_table_ptr;
}

uint64_t get_bits(uint64_t value, unsigned from, unsigned to)
{
    return (value >> from) & ~(~0 << (to - from + 1));
}

uint64_t get_table_record(uint64_t base_ptr, uint64_t index)
{
    uint64_t record_ptr = base_ptr + 8 * index;

    return get_memory_value(record_ptr);
}

uint64_t get_next_table_ptr(uint64_t previous_table_record)
{
    return previous_table_record >> 12 << 12;
}

bool check_record_is_valid(uint64_t record)
{
    // check the first bit
    return record & 1;
}

bool get_physical_addr(uint64_t logical_addr, uint64_t *physical_addr)
{
    uint64_t root_table_index = get_bits(logical_addr, 39, 47);
    uint64_t root_record = get_table_record(_root_table_ptr, root_table_index);

    if (!check_record_is_valid(root_record)) return false;

    uint64_t second_table_ptr = get_next_table_ptr(root_record);
    uint64_t second_table_index = get_bits(logical_addr, 30, 38);
    uint64_t second_record = get_table_record(second_table_ptr, second_table_index);

    if (!check_record_is_valid(second_record)) return false;

    uint64_t third_table_ptr = get_next_table_ptr(second_record);
    uint64_t third_table_index = get_bits(logical_addr, 21, 29);
    uint64_t third_record = get_table_record(third_table_ptr, third_table_index);

    if (!check_record_is_valid(third_record)) return false;

    uint64_t fourth_table_ptr = get_next_table_ptr(third_record);
    uint64_t fourth_table_index = get_bits(logical_addr, 12, 20);
    uint64_t fourth_record = get_table_record(fourth_table_ptr, fourth_table_index);

    if (!check_record_is_valid(fourth_record)) return false;

    uint64_t page_ptr = get_next_table_ptr(fourth_record);
    *physical_addr = page_ptr + get_bits(logical_addr, 0, 11);
    return true;
}
