#ifndef HNM_H
#define HNM_H

#include <libelf.h>
#include <gelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void print_symbols(const char *filename);
char get_symbol_type(GElf_Shdr shdr, GElf_Sym sym);

#endif
