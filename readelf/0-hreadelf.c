#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

/**
 * print_elf_header - prints the ELF header information.
 * @header: pointer to the ELF header structure.
 */
void print_elf_header(Elf64_Ehdr *header)
{
	int i;

	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x ", header->e_ident[i]);
	printf("\n");
	printf("  Class:                             %s\n",
		header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "Unknown");
	printf("  Data:                              %s\n",
		header->e_ident[EI_DATA] == ELFDATA2LSB ?
		"2's complement, little endian" : "Unknown");
	printf("  Version:                           %d (current)\n",
		header->e_ident[EI_VERSION]);
	printf("  OS/ABI:                            %d\n",
		header->e_ident[EI_OSABI]);
	printf("  ABI Version:                       %d\n",
		header->e_ident[EI_ABIVERSION]);
	printf("  Type:                              %d\n",
		header->e_type);
	printf("  Machine:                           %d\n",
		header->e_machine);
	printf("  Version:                           0x%x\n",
		header->e_version);
	printf("  Entry point address:               0x%lx\n",
		header->e_entry);
	printf("  Start of program headers:          %ld (bytes into file)\n",
		header->e_phoff);
	printf("  Start of section headers:          %ld (bytes into file)\n",
		header->e_shoff);
	printf("  Flags:                             0x%x\n",
		header->e_flags);
	printf("  Size of this header:               %d (bytes)\n",
		header->e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n",
		header->e_phentsize);
	printf("  Number of program headers:         %d\n",
		header->e_phnum);
	printf("  Size of section headers:           %d (bytes)\n",
		header->e_shentsize);
	printf("  Number of section headers:         %d\n",
		header->e_shnum);
	printf("  Section header string table index: %d\n",
		header->e_shstrndx);
}

/**
 * main - main function to display ELF file header.
 * @argc: number of command line arguments.
 * @argv: array of command line arguments.
 *
 * Return: 0 on success, or exit with a failure status.
 */
int main(int argc, char **argv)
{
	const char *filename;
	int fd;
	struct stat st;
	void *map;
	Elf64_Ehdr *header;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	if (fstat(fd, &st) == -1)
	{
		perror("Error getting file size");
		close(fd);
		exit(EXIT_FAILURE);
	}

	map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		perror("Error mapping file");
		close(fd);
		exit(EXIT_FAILURE);
	}

	header = (Elf64_Ehdr *)map;
	if (header->e_ident[EI_MAG0] != ELFMAG0 ||
	    header->e_ident[EI_MAG1] != ELFMAG1 ||
	    header->e_ident[EI_MAG2] != ELFMAG2 ||
	    header->e_ident[EI_MAG3] != ELFMAG3)
	{
		fprintf(stderr, "Not an ELF file\n");
		munmap(map, st.st_size);
		close(fd);
		exit(EXIT_FAILURE);
	}

	print_elf_header(header);

	munmap(map, st.st_size);
	close(fd);

	return (0);
}
