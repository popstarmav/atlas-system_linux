#include "hnm.h"
#include <stdio.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void print_symbols(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed\n");
        close(fd);
        return;
    }

    Elf *e = elf_begin(fd, ELF_C_READ, NULL);
    if (!e) {
        perror("elf_begin");
        close(fd);
        return;
    }

    size_t shstrndx;
    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        perror("elf_getshdrstrndx");
        elf_end(e);
        close(fd);
        return;
    }

    Elf_Scn *scn = NULL;
    while ((scn = elf_nextscn(e, scn)) != NULL) {
        GElf_Shdr shdr;
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            perror("gelf_getshdr");
            continue;
        }

        if (shdr.sh_type == SHT_SYMTAB || shdr.sh_type == SHT_DYNSYM) {
            Elf_Data *data = elf_getdata(scn, NULL);
            int count = shdr.sh_size / shdr.sh_entsize;

            for (int i = 0; i < count; i++) {
                GElf_Sym sym;
                if (gelf_getsym(data, i, &sym) != &sym) {
                    perror("gelf_getsym");
                    continue;
                }

                char type = get_symbol_type(shdr, sym);
                if (type == 'U') {
                    printf("         %c %s\n", type, elf_strptr(e, shdr.sh_link, sym.st_name));
                } else {
                    printf("%08lx %c %s\n", (unsigned long)sym.st_value, type, elf_strptr(e, shdr.sh_link, sym.st_name));
                }
            }
        }
    }

    elf_end(e);
    close(fd);
}

char get_symbol_type(GElf_Shdr shdr, GElf_Sym sym) {
    switch (GELF_ST_TYPE(sym.st_info)) {
        case STT_FUNC:
            return (shdr.sh_flags & SHF_EXECINSTR) ? 'T' : 't';
        case STT_OBJECT:
            return (shdr.sh_flags & SHF_WRITE) ? 'D' : 'R';
        default:
            return 'U';
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        print_symbols(argv[i]);
    }

    return 0;
}

