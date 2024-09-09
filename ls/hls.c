#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// Custom string comparison function without using strcmp
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

// Compare function for qsort
int compare_entries(const void *a, const void *b) {
    return custom_strcmp(*(const char **)a, *(const char **)b);
}

// Function to list directory contents
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
            if (one_per_line) {
                printf("\n"); // Print each entry on a new line
            } else {
                printf("  "); // Print entries separated by space
            }
            free(entries[i]);
        }

        if (!one_per_line) {
            printf("\n"); // Print a new line at the end if not using -1
        }

        free(entries);
    } else if (S_ISREG(path_stat.st_mode)) {
        printf("%s\n", path);
    } else {
        fprintf(stderr, "%s: cannot access %s: Not a directory\n", program_name, path);
        return 1;
    }

    return 0;
}

// Main function to handle command-line arguments and call list_directory
int main(int argc, char *argv[]) {
    int exit_code = 0;
    int one_per_line = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '1' && argv[i][2] == '\0') {
            one_per_line = 1;
        }
    }

    // If no file or directory is provided, default to current directory
    if (argc == 1 || (argc == 2 && one_per_line)) {
        if (list_directory(".", argv[0], one_per_line) != 0) {
            exit_code = 1;
        }
    } else {
        // Handle each argument (directory or file)
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == '1' && argv[i][2] == '\0') {
                continue; // Skip the -1 option
            }
            int print_dirname = (argc > 2) ? 1 : 0; // Print directory name if there are multiple directories/files
            if (list_directory(argv[i], argv[0], one_per_line) != 0) {
                exit_code = 1;
            }
            if (i < argc - 1) {
                printf("\n"); // Print a blank line between multiple directories
            }
        }
    }

    return exit_code;
}
