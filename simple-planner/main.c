#include <stdlib.h>
#include <stdio.h>

void quick_sort(unsigned *from, unsigned *to, unsigned *ifrom, unsigned *ito);
void read_array_from_stdin(unsigned *arr, unsigned size);
void init_indexes_array(unsigned *arr, unsigned size);
void print_array(const unsigned *arr, unsigned len);

int main()
{
    unsigned length;
    scanf("%u\n", &length);

    unsigned *arr = calloc(length, sizeof(unsigned));
    read_array_from_stdin(arr, length);

    unsigned *indexes_array = calloc(length, sizeof(unsigned));
    init_indexes_array(indexes_array, length);

    quick_sort(arr, arr + length, indexes_array, indexes_array + length);
    print_array(indexes_array, length);

    free(arr);
    free(indexes_array);

    return 0;
}

void swap(unsigned *arr, unsigned from, unsigned to)
{
    unsigned arr_tmp = arr[from];
    arr[from] = arr[to];
    arr[to] = arr_tmp;
}

void print_array(const unsigned *arr, unsigned len)
{
    for (unsigned i = 0; i < len; i++)
    {
        printf("%u ", arr[i]);
    }

    putchar('\n');
}

void read_array_from_stdin(unsigned *arr, unsigned size)
{
    for (unsigned i = 0; i < size; i++)
    {
        scanf("%u", &arr[i]);
    }
}

void init_indexes_array(unsigned *arr, unsigned size)
{
    for (unsigned i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

void quick_sort(unsigned *from, unsigned *to, unsigned *ifrom, unsigned *ito)
{
    if (from >= to)
    {
        return;
    }

    int len = to - from;
    int pivot = len - 1;

    for (int i = len - 1; i > 0; --i)
    {
        if (from[i - 1] > from[pivot])
        {
            swap(from, i - 1, pivot);
            swap(ifrom, i - 1, pivot);
            pivot = i - 1;
        }
    }

    quick_sort(from, from + pivot, ifrom, ifrom + pivot);
    quick_sort(from + pivot + 1, to, ifrom + pivot + 1, ito);
}
