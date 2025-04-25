# libASM

A library of common functions implemented in x86 assembly language.

## Overview

libASM is a collection of reimplemented C standard library functions written in x86 assembly language. This project demonstrates low-level programming concepts and provides an efficient implementation of commonly used functions.

## Functions

The library includes the following functions:

- `ft_strlen`: Calculate the length of a string
- `ft_strcpy`: Copy a string to another
- `ft_strcmp`: Compare two strings
- `ft_write`: Write to a file descriptor
- `ft_read`: Read from a file descriptor
- `ft_strdup`: Duplicate a string

## Building the Library

To build the library, use the provided Makefile:

```bash
make
```

This will generate a `libasm.a` static library file.

## Usage

### Linking with Your Project

To use libASM in your project, include the header file and link against the library:

```c
#include "libasm.h"
```

When compiling your program:

```bash
gcc -o your_program your_program.c -L. -lasm
```

### Example

```c
#include <stdio.h>
#include "libasm.h"

int main() {
    char str[] = "Hello, Assembly!";
    printf("Length: %zu\n", ft_strlen(str));
    
    char dest[50];
    ft_strcpy(dest, str);
    printf("Copied: %s\n", dest);
    
    return 0;
}
```

## Testing

To run tests for the library:

```bash
make test
```
