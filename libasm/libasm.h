#ifndef LIBASM_H
#define LIBASM_H

#include <stddef.h>

int asm_strcmp(const char *s1, const char *s2);
int asm_strncmp(const char *s1, const char *s2, size_t n);
char *asm_strchr(const char *s, int c);
char *asm_strstr(const char *haystack, const char *needle);
void *asm_memcpy(void *dest, const void *src, size_t n);
size_t asm_strlen(const char *str);
size_t asm_putc(int c);
size_t asm_puts(const char *str);
size_t asm_strspn(const char *s, const char *accept);
size_t asm_strcspn(const char *s, const char *reject);
int asm_strcasecmp(const char *s1, const char *s2);
int asm_strncasecmp(const char *s1, const char *s2, size_t n);

#endif /* LIBASM_H */
