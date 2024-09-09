#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

/**
 * custom_strcmp - Case-insensitive string comparison
 * @a: First string
 * @b: Second string
 * 
 * Return: Negative if a < b, positive if a > b, 0 if equal
 */
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

/**
 * compare_entries - Comparison function for qsort
 * @a: First entry
 * @b: Second entry
 * 
 * Return: Comparison result
 */
int compare_entries(const void *a, const void *b) {
    return custom_strcmp(*(const char **)a, *(const char **)b);
}

/**
 * list_directory - Lists contents of a directory or file
 * @path: Directory or file path to list
 * @program_name: Name of the program (argv[0])
 * @one_per_line: Print each entry on a new line if true
 * 
 * Return: 0 on success, 1 on failure
 */
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
            printf("%s\n", entries[i]); // Print each entry on a new line
            free(entries[i]);
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

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector (file/directory names)
 * 
 * Return: 0 on success, 1 on failure
 */
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
        for (int i = start_index; i < argc; i++) {
            // Skip if the argument is "-1" (should not be processed as a file or directory)
            if (custom_strcmp(argv[i], "-1") == 0) {
                continue;
            }

            if (list_directory(argv[i], argv[0], one_per_line) != 0) {
                exit_code = 1;
            }
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }

    return exit_code;
}
