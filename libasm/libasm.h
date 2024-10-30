#ifndef LIBASM_H
#define LIBASM_H

#include <stddef.h>

int asm_strcmp(const char *s1, const char *s2);
int asm_strncmp(const char *s1, const char *s2, size_t n);
char *asm_strchr(const char *s, int c);
char *asm_strstr(const char *haystack, const char *needle);
size_t asm_strlen(const char *str);

#endif /* LIBASM_H */
