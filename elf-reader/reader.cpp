#include <cstdint>
#include <cstdio>
#include <cstdlib>

#define ELF_NIDENT 16

// program header-ы такого типа нужно загрузить в
// память при загрузке приложения
#define PT_LOAD 1

// Эта структура описывает формат заголовока ELF файла
struct elf_hdr
{
    std::uint8_t e_ident[ELF_NIDENT];
    std::uint16_t e_type;
    std::uint16_t e_machine;
    std::uint32_t e_version;
    std::uint64_t e_entry;
    std::uint64_t e_phoff;
    std::uint64_t e_shoff;
    std::uint32_t e_flags;
    std::uint16_t e_ehsize;
    std::uint16_t e_phentsize;
    std::uint16_t e_phnum;
    std::uint16_t e_shentsize;
    std::uint16_t e_shnum;
    std::uint16_t e_shstrndx;
} __attribute__((packed));

// структура записи в таблице program header-ов
struct elf_phdr
{
    std::uint32_t p_type;
    std::uint32_t p_flags;
    std::uint64_t p_offset;
    std::uint64_t p_vaddr;
    std::uint64_t p_paddr;
    std::uint64_t p_filesz;
    std::uint64_t p_memsz;
    std::uint64_t p_align;
} __attribute__((packed));

std::uintptr_t entry_point(const char *name)
{
    std::FILE *file = std::fopen(name, "r");

    if (!file)
    {
        std::printf("Error trying to open file %s", name);
        std::exit(1);
    }

    std::size_t bites_to_read = sizeof(elf_hdr);

    struct elf_hdr * header = (struct elf_hdr*)std::malloc(bites_to_read);

    std::fread(header, bites_to_read, 1, file);

    uintptr_t entry = header->e_entry;

    std::free(header);
    std::fclose(file);

    return entry;
}

std::size_t space(const char *name)
{
    std::FILE *file = std::fopen(name, "r");

    if (!file)
    {
        std::printf("Error trying to open file %s", name);
        std::exit(1);
    }

    std::size_t bites_to_read = sizeof(elf_hdr);

    struct elf_hdr *header = (struct elf_hdr *)std::malloc(bites_to_read);

    std::fread(header, bites_to_read, 1, file);

    std::fseek(file, header->e_phoff, SEEK_SET);

    struct elf_phdr *pg_header = (struct elf_phdr *)std::malloc(
        sizeof(struct elf_phdr) * header->e_phnum);

    std::fread(pg_header, sizeof(struct elf_phdr), header->e_phnum, file);

    size_t sum = 0;
    for (int i = 0; i < header->e_phnum; i++)
    {
        struct elf_phdr *cur_pg_header = pg_header + i;

        if (cur_pg_header->p_type == PT_LOAD)
        {
            sum += cur_pg_header->p_memsz;
        }
    }

    std::free(header);
    std::free(pg_header);
    std::fclose(file);

    return sum;
}
