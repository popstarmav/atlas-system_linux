#include <stdio.h>
#include "hreadelf.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>
#include <errno.h>

#define ERR_NOT_ELF "Error: Not a valid ELF file\n"

/**
 * check_elf - checks if a file is a valid ELF file
 * @e_ident: pointer to the ELF identification array
 * Return: 0 if valid, -1 if not
 */
int check_elf(unsigned char *e_ident)
{
	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
	    e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
		return (-1); /* Not a valid ELF file */
	return (0);
}

/**
 * open_file - opens a file and returns its file descriptor
 * @filename: path to the file
 * Return: file descriptor on success, -1 on failure
 */
int open_file(const char *filename)
{
	int fd = open(filename, O_RDONLY);

	if (fd == -1)
	{
		fprintf(stderr, "Error: Cannot open file %s: %s\n",
			filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

/**
 * print_elf_header - prints the ELF header information
 * @header: pointer to ELF header
 */
void print_elf_header(Elf64_Ehdr *header)
{
	printf("ELF Header:\n");
	printf("  Magic:   %02x %02x %02x %02x\n",
		header->e_ident[EI_MAG0], header->e_ident[EI_MAG1],
		header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
	printf("  Class:                             %s\n",
		header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "ELF32");
	printf("  Data:                              %s\n",
		header->e_ident[EI_DATA] == ELFDATA2LSB ?
		"2's complement, little endian" : "Big endian");
	printf("  Version:                           0x%x\n",
		header->e_version);  /* Use %x for unsigned int */
	printf("  OS/ABI:                            %d\n",
		header->e_ident[EI_OSABI]);
	printf("  Type:                              %d\n",
		header->e_type);
	printf("  Machine:                           %d\n",
		header->e_machine);
	printf("  Entry point address:               0x%lx\n",
		header->e_entry);  /* Keep %lx here since e_entry is long */
}

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	int fd;
	ssize_t r;
	Elf64_Ehdr elf_header;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	fd = open_file(argv[1]);
	if (fd == -1)
		return (EXIT_FAILURE);

	r = read(fd, &elf_header, sizeof(Elf64_Ehdr));
	if (r != sizeof(Elf64_Ehdr) || check_elf(elf_header.e_ident))
	{
		fprintf(stderr, ERR_NOT_ELF);
		close(fd);
		return (EXIT_FAILURE);
	}

	print_elf_header(&elf_header);
	close(fd);

	return (EXIT_SUCCESS);
}

