#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int custom_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        char lower_a = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        char lower_b = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (lower_a != lower_b)
            return lower_a - lower_b;
        a++;
        b++;
    }
    return *a - *b;
}

int compare_entries(const void *a, const void *b) {
    return custom_strcmp(*(const char **)a, *(const char **)b);
}

int list_directory(const char *path, const char *program_name, int one_per_line) {
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;
    char **entries = NULL;
    int count = 0;
    int capacity = 10;

    entries = malloc(capacity * sizeof(char *));
    if (entries == NULL) {
        perror("malloc");
        return 1;
    }

    if (lstat(path, &path_stat) == -1) {
        fprintf(stderr, "%s: cannot access %s: ", program_name, path);
        perror("");
        free(entries);
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        dir = opendir(path);
        if (dir == NULL) {
            fprintf(stderr, "%s: cannot open directory %s: ", program_name, path);
            perror("");
            free(entries);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                if (count == capacity) {
                    capacity *= 2;
                    entries = realloc(entries, capacity * sizeof(char *));
                    if (entries == NULL) {
                        perror("realloc");
                        closedir(dir);
                        return 1;
                    }
                }
                entries[count] = malloc(strlen(entry->d_name) + 1);
                if (entries[count] == NULL) {
                    perror("malloc");
                    closedir(dir);
                    return 1;
                }
                strcpy(entries[count], entry->d_name);
                count++;
            }
        }
        closedir(dir);

        qsort(entries, count, sizeof(char *), compare_entries);

        for (int i = 0; i < count; i++) {
            printf("%s", entries[i]);
            if (one_per_line || i == count - 1) {
                printf("\n");
            } else {
                printf("  ");
            }
            free(entries[i]);
        }
    } else if (S_ISREG(path_stat.st_mode)) {
        printf("%s\n", path);
    } else {
        fprintf(stderr, "%s: cannot access %s: Not a directory\n", program_name, path);
        free(entries);
        return 1;
    }

    free(entries);
    return 0;
}

int main(int argc, char *argv[]) {
    int exit_code = 0;
    int one_per_line = 0;

    for (int i = 1; i < argc; i++) {
        if (custom_strcmp(argv[i], "-1") == 0) {
            one_per_line = 1;
            break;
        }
    }

    if (argc == 1 || (argc == 2 && one_per_line)) {
        if (list_directory(".", argv[0], one_per_line) != 0) {
            exit_code = 1;
        }
    } else {
        for (int i = 1; i < argc; i++) {
            if (custom_strcmp(argv[i], "-1") == 0) {
                continue;
            }

            if (list_directory(argv[i], argv[0], one_per_line) != 0) {
                exit_code = 1;
            }
        }
    }

    return exit_code;
}
