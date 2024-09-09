#ifndef HSL_H
#define HSL_H

#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>

int string_equals(const char *str1, const char *str2);
int compare_strings(const void *a, const void *b);
int list_directory(const char *path, const char *program_name, int print_dirname, int one_per_line);


#endif /* HSL_H */