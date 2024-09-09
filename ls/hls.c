#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Custom string comparison function for case-insensitive comparison
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

// Compare function for qsort to sort directory entries
int compare_entries(const void *a, const void *b) {
    return custom_strcmp(*(const char **)a, *(const char **)b);
}

// Function to list directory contents
int list_directory(const char *path, int one_per_line) {
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;
    char **entries = NULL;
    int count = 0;
    int capacity = 10;

    printf("Listing directory: %s\n", path); // Debug statement

    if (lstat(path, &path_stat) == -1) {
        perror("Error accessing path");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        dir = opendir(path);
        if (dir == NULL) {
            perror("Error opening directory");
            return 1;
        }

        entries = malloc(capacity * sizeof(char *));
        if (entries == NULL) {
            perror("Error allocating memory");
            closedir(dir);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            // Debug print to show each entry being read
            printf("Read entry: %s\n", entry->d_name);

            if (entry->d_name[0] != '.') { // Skip hidden files (if not including `-a` option)
                if (count == capacity) {
                    capacity *= 2;
                    entries = realloc(entries, capacity * sizeof(char *));
                    if (entries == NULL) {
                        perror("Error reallocating memory");
                        closedir(dir);
                        return 1;
                    }
                }
                entries[count] = malloc(strlen(entry->d_name) + 1);
                if (entries[count] == NULL) {
                    perror("Error allocating memory");
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
            if (one_per_line) {
                printf("%s\n", entries[i]);
            } else {
                printf("%s  ", entries[i]);
            }
            free(entries[i]);
        }
        if (!one_per_line) {
            printf("\n");
        }
        free(entries);
    } else if (S_ISREG(path_stat.st_mode)) {
        printf("%s\n", path);
    } else {
        fprintf(stderr, "Not a directory or regular file: %s\n", path);
        return 1;
    }

    return 0;
}

// Main function to handle command-line arguments and call list_directory
int main(int argc, char *argv[]) {
    int exit_code = 0;
    int one_per_line = 0;
    int start_index = 1;

    // Parse command-line arguments for options
    if (argc > 1 && custom_strcmp(argv[1], "-1") == 0) {
        one_per_line = 1;
        start_index = 2;
    }

    // Handle case where no file or directory is provided
    if (argc == start_index) {
        if (list_directory(".", one_per_line) != 0) {
            exit_code = 1;
        }
    } else {
        // Handle each provided argument
        for (int i = start_index; i < argc; i++) {
            if (list_directory(argv[i], one_per_line) != 0) {
                exit_code = 1;
            }
            if (i < argc - 1) {
                printf("\n"); // Print a blank line between multiple directories
            }
        }
    }

    return exit_code;
}

