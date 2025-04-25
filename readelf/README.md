# C - ELF: readelf

This project implements a simplified version of the `readelf` utility, which displays information about ELF (Executable and Linkable Format) files. ELF is a common standard file format for executables, object code, shared libraries, and core dumps.

## Description

This implementation focuses on displaying information from the ELF header of a file, similar to the `-h` option of the standard `readelf` utility.

## Features

- Parse and display ELF header information
- Support for both 32-bit and 64-bit ELF files
- Display file type, architecture, version, entry point, and other header information

## Usage

```
./readelf -h elf_file
```

## Building the Project

To compile the project, use:

```bash
gcc -Wall -Werror -Wextra -pedantic *.c -o readelf
```

## Requirements

- GCC compiler
- Linux environment (as ELF is primarily used in Linux/Unix systems)

## Resources

- [ELF Format Specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)
- [Man page for readelf](https://linux.die.net/man/1/readelf)
