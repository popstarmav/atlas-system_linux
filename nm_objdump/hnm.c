#include <libelf.h>
#include <gelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void display_symbols(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "ELF library initialization failed.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    Elf *elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
        fprintf(stderr, "Failed to read ELF file.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        if (gelf_getshdr(scn, &shdr) != &shdr) {
            fprintf(stderr, "Failed to get section header.\n");
            exit(EXIT_FAILURE);
        }

        if (shdr.sh_type == SHT_SYMTAB) {
            Elf_Data *data = elf_getdata(scn, NULL);
            int symbol_count = shdr.sh_size / shdr.sh_entsize;
            for (int i = 0; i < symbol_count; ++i) {
                GElf_Sym sym;
                gelf_getsym(data, i, &sym);

                const char *name = elf_strptr(elf, shdr.sh_link, sym.st_name);
                char type = '?';

                switch (GELF_ST_TYPE(sym.st_info)) {
                    case STT_OBJECT: type = 'D'; break;
                    case STT_FUNC:   type = 'T'; break;
                    case STT_SECTION:type = 'S'; break;
                    case STT_FILE:   type = 'F'; break;
                    default:         type = '?'; break;
                }

                printf("%016lx %c %s\n", (long) sym.st_value, type, name ? name : "");
            }
        }
    }

    elf_end(elf);
    close(fd);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [objfile...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        printf("\n%s:\n", argv[i]);
        display_symbols(argv[i]);
    }

    return EXIT_SUCCESS;
}
