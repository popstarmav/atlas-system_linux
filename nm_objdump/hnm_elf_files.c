#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <unistd.h>
#include <string.h>

void display_symbols(const char *filename) {
    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    // Map file to memory
    Elf64_Ehdr *ehdr = mmap(NULL, sizeof(Elf64_Ehdr), PROT_READ, MAP_PRIVATE, fd, 0);
    if (ehdr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    // Check if it’s an ELF file
    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s: Not an ELF file\n", filename);
        munmap(ehdr, sizeof(Elf64_Ehdr));
        close(fd);
        return;
    }

    // Locate symbol table and iterate over symbols (details omitted)
    // ...

    munmap(ehdr, sizeof(Elf64_Ehdr));
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        display_symbols(argv[i]);
    }

    return 0;
}

