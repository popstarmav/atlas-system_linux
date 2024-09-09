#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

// Define PATH_MAX if not defined
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Custom string comparison function
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
int list_directory(const char *path, int one_per_line) {
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;
    char **entries = NULL;
    int count = 0;
    int capacity = 10;

    // Check if the path exists and is accessible
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

        // Allocate memory for entries
        entries = malloc(capacity * sizeof(char *));
        if (entries == NULL) {
            perror("Error allocating memory");
            closedir(dir);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.' || 1) { // Assuming -a option is not implemented here
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

        // Sort entries
        qsort(entries, count, sizeof(char *), compare_entries);

        // Print entries
        for (int i = 0; i < count; i++) {
            printf("%s\n", entries[i]); // Ensure each entry is printed on a new line
            free(entries[i]);
        }
        free(entries);
    } else if (S_ISREG(path_stat.st_mode)) {
        // Print the file name if it's a regular file
        printf("%s\n", path);
    } else {
        fprintf(stderr, "Not a directory or regular file\n");
        return 1;
    }

    return 0;
}

// Main function to handle command-line arguments and call list_directory
int main(int argc, char *argv[]) {
    int exit_code = 0;
    int one_per_line = 0;
    int start_index = 1;

    // Parse command-line arguments
    if (argc > 1 && custom_strcmp(argv[1], "-1") == 0) {
        one_per_line = 1;
        start_index = 2;
    }

    // If no file or directory is provided, default to current directory
    if (argc == start_index) {
        if (list_directory(".", one_per_line) != 0) {
            exit_code = 1;
        }
    } else {
        // Handle each argument (directory or file)
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
