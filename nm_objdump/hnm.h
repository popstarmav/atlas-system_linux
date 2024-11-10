#ifndef HNM_H
#define HNM_H

#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For close()

// Function prototypes
void print_symbols(const char *filename);
char get_symbol_type(GElf_Shdr shdr, GElf_Sym sym);

#endif /* HNM_H */

