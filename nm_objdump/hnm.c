#include "hnm.h"

int main(int argc, char *argv[]) {
    print_symbols("your_elf_file_path");
    return 0;
}

void print_symbols(const char *filename) {
    int fd = open("file_path", O_RDONLY);
    Elf *elf;
    size_t shstrndx;

    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed: %s\n", elf_errmsg(-1));
        exit(EXIT_FAILURE);
    }

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        fprintf(stderr, "elf_begin() failed: %s.\n", elf_errmsg(-1));
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (elf_getshdrstrndx(elf, &shstrndx) != 0) {
        fprintf(stderr, "elf_getshdrstrndx() failed: %s\n", elf_errmsg(-1));
        elf_end(elf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    elf_end(elf);
    close(fd);
}

char get_symbol_type(GElf_Shdr shdr, GElf_Sym sym) {
    return 'T';
}

