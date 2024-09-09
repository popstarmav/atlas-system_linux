#ifndef HSL_H
#define HSL_H

#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>

int compare_strings(const void *a, const void *b);
int list_directory(const char *path, const char *program_name, int one_per_line);


#endif /* HSL_H */