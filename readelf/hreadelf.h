#ifndef HREADELF_H
#define HREADELF_H

#include <stdint.h>  /* For fixed-width data types */

/* Error Messages */
#define USAGE "Usage: ./hreadelf <elf_file>\n"
#define ERR_NOT_MAGIC "Error: Not a valid ELF file.\n"

/* Macros to check ELF class (32-bit or 64-bit) */
#define IS_32(e64) ((e64).e_ident[EI_CLASS] == ELFCLASS32)
#define IS_64(e64) ((e64).e_ident[EI_CLASS] == ELFCLASS64)

/* ELF header structure for 64-bit */
typedef struct {
    unsigned char e_ident[16];   /* ELF identification */
    uint16_t e_type;             /* Object file type */
    uint16_t e_machine;          /* Machine type */
    uint32_t e_version;          /* Object file version */
    uint64_t e_entry;            /* Entry point address */
    uint64_t e_phoff;            /* Program header offset */
    uint64_t e_shoff;            /* Section header offset */
    uint32_t e_flags;            /* Processor-specific flags */
    uint16_t e_ehsize;           /* ELF header size */
    uint16_t e_phentsize;        /* Size of program header entry */
    uint16_t e_phnum;            /* Number of program header entries */
    uint16_t e_shentsize;        /* Size of section header entry */
    uint16_t e_shnum;            /* Number of section header entries */
    uint16_t e_shstrndx;         /* Section name string table index */
} elf64_hdr;

/* ELF header structure for 32-bit */
typedef struct {
    unsigned char e_ident[16];   /* ELF identification */
    uint16_t e_type;             /* Object file type */
    uint16_t e_machine;          /* Machine type */
    uint32_t e_version;          /* Object file version */
    uint32_t e_entry;            /* Entry point address */
    uint32_t e_phoff;            /* Program header offset */
    uint32_t e_shoff;            /* Section header offset */
    uint32_t e_flags;            /* Processor-specific flags */
    uint16_t e_ehsize;           /* ELF header size */
    uint16_t e_phentsize;        /* Size of program header entry */
    uint16_t e_phnum;            /* Number of program header entries */
    uint16_t e_shentsize;        /* Size of section header entry */
    uint16_t e_shnum;            /* Number of section header entries */
    uint16_t e_shstrndx;         /* Section name string table index */
} elf32_hdr;

/* Union to store either a 32-bit or 64-bit ELF header */
typedef union {
    elf64_hdr e64;
    elf32_hdr e32;
} elf_t;

/* Function Prototypes */

/* Open an ELF file and return the file descriptor */
int open_file(const char *filename);

/* Check if the file is a valid ELF file */
int check_elf(unsigned char *e_ident);

/* Switch endianess for cross-platform ELF file reading */
void switch_all_endian(elf_t *elf_header);

/* Print ELF header information */
int print_header(elf_t *elf_header);

#endif /* HREADELF_H */
