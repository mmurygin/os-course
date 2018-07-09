#include <stdlib.h>
#include <stdio.h>

#define MAX_ARRAY_SIZE 10000

void quick_sort(int *arr, int *indexes, int len);
int init_array_from_line(int *arr, const char *line);
void init_indexes_array(int *arr, int size);
void print_array(const int *arr, int len);

int main()
{
    int requests;
    scanf("%d\n", &requests);

    char *buffer = NULL;
    size_t len = 0;

    int * arr = calloc(MAX_ARRAY_SIZE, sizeof(int));
    int * indexes_array = calloc(MAX_ARRAY_SIZE, sizeof(int));
    int real_length = 0;
    while (requests--)
    {
        getline(&buffer, &len, stdin);
        real_length = init_array_from_line(arr, buffer);
        init_indexes_array(indexes_array, real_length);

        quick_sort(arr, indexes_array, real_length);
        print_array(indexes_array, real_length);

        free(buffer);
        buffer = NULL;
        real_length = 0;
    }

    free(arr);
    free(indexes_array);

    return 0;
}

void swap(int *arr, int *indexes, int from, int to)
{
    int arr_tmp = arr[from];
    arr[from] = arr[to];
    arr[to] = arr_tmp;

    arr_tmp = indexes[from];
    indexes[from] = indexes[to];
    indexes[to] = arr_tmp;
}

void print_array(const int *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }

    putchar('\n');
}

int init_array_from_line(int *arr, const char *line)
{
    int el;
    int index = 0;

    while (sscanf(line, "%d", &el))
    {
        arr[index++] = el;
    }

    return index;
}

void init_indexes_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

void quick_sort(int *arr, int *indexes, int len)
{
    return;
}
