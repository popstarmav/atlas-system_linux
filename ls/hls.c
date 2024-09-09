#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

// Custom case-insensitive string comparison
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

// Comparison function for qsort
int compare_entries(const void *a, const void *b) {
    return custom_strcmp(*(const char **)a, *(const char **)b);
}

// List directory contents
int list_directory(const char *path, const char *program_name, int one_per_line) {
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;
    char **entries = NULL;
    int count = 0;
    int capacity = 10;

    if (lstat(path, &path_stat) == -1) {
        fprintf(stderr, "%s: cannot access %s: ", program_name, path);
        perror("");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        printf("%s:\n", path);
        dir = opendir(path);
        if (dir == NULL) {
            fprintf(stderr, "%s: cannot open directory %s: ", program_name, path);
            perror("");
            return 1;
        }

        entries = malloc(capacity * sizeof(char *));
        if (entries == NULL) {
            perror("malloc");
            closedir(dir);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {  // Skip hidden files
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
            printf("%s\n", entries[i]);
            free(entries[i]);
        }

        free(entries);
    } else if (S_ISREG(path_stat.st_mode)) {  // Handle regular files
        printf("%s\n", path);
    } else {
        fprintf(stderr, "%s: cannot access %s: Not a directory\n", program_name, path);
        return 1;
    }

    return 0;
}

// Main function
int main(int argc, char *argv[]) {
    int exit_code = 0;
    int one_per_line = 0;
    int start_index = 1;

    // Check for -1 option
    if (argc > 1 && custom_strcmp(argv[1], "-1") == 0) {
        one_per_line = 1;
        start_index = 2;  // Skip the -1 option
    }

    // If no file or directory is provided, default to current directory
    if (argc == start_index) {
        if (list_directory(".", argv[0], one_per_line) != 0) {
            exit_code = 1;
        }
    } else {
        // Handle each argument (directory or file)
        for (int i = start_index; i < argc; i++) {
            // Skip -1 as it should not be processed as a directory
            if (custom_strcmp(argv[i], "-1") == 0) {
                continue;
            }

            int print_dirname = (argc > start_index + 1) ? 1 : 0;
            if (list_directory(argv[i], argv[0], one_per_line) != 0) {
                exit_code = 1;
            }

            // Print a blank line between multiple directories
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }

    return exit_code;
}
