#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>   // For strerror() to interpret error numbers
#include <ctype.h>    // For tolower()

int custom_strcmp(const char *a, const char *b) {
    while (*a && *b) {
        char lower_a = tolower(*a);
        char lower_b = tolower(*b);
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

int list_directory(const char *path, int one_per_line) {
    DIR *dir;
    struct dirent *entry;
    char **entries = NULL;
    int count = 0, capacity = 10;

    // Open the directory
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

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            // Reallocate memory if necessary
            if (count == capacity) {
                capacity *= 2;
                char **new_entries = realloc(entries, capacity * sizeof(char *));
                if (new_entries == NULL) {
                    perror("Error reallocating memory");
                    closedir(dir);
                    for (int i = 0; i < count; i++) {
                        free(entries[i]);
                    }
                    free(entries);
                    return 1;
                }
                entries = new_entries;
            }
            // Allocate memory for the current entry and copy the entry's name
            entries[count] = malloc(strlen(entry->d_name) + 1);
            if (entries[count] == NULL) {
                perror("Error allocating memory");
                closedir(dir);
                for (int i = 0; i < count; i++) {
                    free(entries[i]);
                }
                free(entries);
                return 1;
            }
            strcpy(entries[count], entry->d_name);
            count++;
        }
    }

    // Close the directory
    closedir(dir);

    // Sort the entries using custom comparison
    qsort(entries, count, sizeof(char *), compare_entries);

    // Print the entries
    for (int i = 0; i < count; i++) {
        if (one_per_line) {
            printf("%s\n", entries[i]);
        } else {
            printf("%s  ", entries[i]);
        }
        free(entries[i]); // Free the memory for each entry
    }

    if (!one_per_line) {
        printf("\n"); // Newline if not printing one entry per line
    }

    free(entries); // Free the list of entries
    return 0;
}

int main(int argc, char *argv[]) {
    int one_per_line = 0;  // Flag for the `-1` option
    char *path = ".";      // Default to the current directory

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '1' && argv[i][2] == '\0') {
            one_per_line = 1;  // Set flag if `-1` is passed
        } else if (argv[i][0] != '-') {
            path = argv[i];  // Set path to the first non-option argument
        } else {
            printf("Unrecognized option: %s\n", argv[i]);
        }
    }

    return list_directory(path, one_per_line);
}
