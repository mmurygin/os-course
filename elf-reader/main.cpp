#include <cstdlib>
#include <cstdio>
#include "reader.hpp"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::puts("Please provide a file name to read header");
        std::exit(1);
    }

    char * file_name = argv[1];

    std::printf("Entry Point: %lx\n", entry_point(file_name));
    std::printf("The sum of all segments memory: %zu\n", space(file_name));

    return 0;
}
