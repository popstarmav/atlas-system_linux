#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

void print_elf_header(Elf64_Ehdr *header);
void print_magic(unsigned char *e_ident);

/**
 * main - Entry point to display ELF header information
 * @argc: Argument count
 * @argv: Argument vector
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	int fd;
	ssize_t read_size;
	Elf64_Ehdr header;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		return (1);
	}

	/* Open the ELF file */
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error: Cannot open file");
		return (1);
	}

	/* Read ELF header */
	read_size = read(fd, &header, sizeof(header));
	if (read_size != sizeof(header))
	{
		perror("Error: Cannot read ELF header");
		close(fd);
		return (1);
	}

	/* Verify if it's an ELF file by checking magic bytes */
	if (memcmp(header.e_ident, ELFMAG, SELFMAG) != 0)
	{
		fprintf(stderr, "Error: Not an ELF file\n");
		close(fd);
		return (1);
	}

	/* Print ELF header */
	print_elf_header(&header);

	close(fd);
	return (0);
}

/**
 * print_elf_header - Prints ELF header information
 * @header: Pointer to the ELF header
 */
void print_elf_header(Elf64_Ehdr *header)
{
	printf("ELF Header:\n");
	print_magic(header->e_ident);

	/* Class */
	printf("  Class:                             %s\n",
		header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" :
		header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "Invalid class");

	/* Data */
	printf("  Data:                              %s\n",
		header->e_ident[EI_DATA] == ELFDATA2LSB ?
		"2's complement, little endian" :
		header->e_ident[EI_DATA] == ELFDATA2MSB ?
		"2's complement, big endian" :
		"Invalid data encoding");

	/* Version */
	printf("  Version:                           %d (current)\n",
		header->e_ident[EI_VERSION]);

	/* OS/ABI */
	printf("  OS/ABI:                            %s\n",
		header->e_ident[EI_OSABI] == ELFOSABI_SYSV ? "UNIX - System V" :
		header->e_ident[EI_OSABI] == ELFOSABI_HPUX ? "HP-UX" :
		header->e_ident[EI_OSABI] == ELFOSABI_NETBSD ? "NetBSD" :
		header->e_ident[EI_OSABI] == ELFOSABI_SOLARIS ? "UNIX - Solaris" :
		"Unknown");

	/* ABI Version */
	printf("  ABI Version:                       %d\n", header->e_ident[EI_ABIVERSION]);

	/* Type */
	printf("  Type:                              %s\n",
		header->e_type == ET_EXEC ? "EXEC (Executable file)" :
		header->e_type == ET_REL ? "REL (Relocatable file)" :
		header->e_type == ET_DYN ? "DYN (Shared object file)" :
		header->e_type == ET_CORE ? "CORE (Core file)" : "Unknown");

	/* Machine */
	printf("  Machine:                           %s\n",
		header->e_machine == EM_386 ? "Intel 80386" :
		header->e_machine == EM_X86_64 ? "AMD x86-64" : "Unknown");

	/* Entry point address */
	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		printf("  Entry point address:               0x%x\n", (unsigned int)header->e_entry);
	else
		printf("  Entry point address:               0x%lx\n", header->e_entry);

	/* Start of program headers */
	printf("  Start of program headers:          %ld (bytes into file)\n", header->e_phoff);

	/* Start of section headers */
	printf("  Start of section headers:          %ld (bytes into file)\n", header->e_shoff);

	/* Flags */
	printf("  Flags:                             0x%x\n", header->e_flags);

	/* Size of ELF header */
	printf("  Size of this header:               %d (bytes)\n", header->e_ehsize);

	/* Size of program headers */
	printf("  Size of program headers:           %d (bytes)\n", header->e_phentsize);

	/* Number of program headers */
	printf("  Number of program headers:         %d\n", header->e_phnum);

	/* Size of section headers */
	printf("  Size of section headers:           %d (bytes)\n", header->e_shentsize);

	/* Number of section headers */
	printf("  Number of section headers:         %d\n", header->e_shnum);

	/* Section header string table index */
	printf("  Section header string table index: %d\n", header->e_shstrndx);
}

/**
 * print_magic - Prints the ELF magic bytes
 * @e_ident: Pointer to the ELF magic bytes
 */
void print_magic(unsigned char *e_ident)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x ", e_ident[i]);
	printf("\n");
}


