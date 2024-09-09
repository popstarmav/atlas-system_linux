#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>  // Include for stat and lstat
#include <errno.h>

// Custom case-insensitive string comparison function
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

int list_directory(const char *path, int force_one_per_line) {
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

    // Read directory contents and store in entries array
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {  // Skip hidden files
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

    // Close the directory
    closedir(dir);

    // Sort the entries using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (custom_strcmp(entries[j], entries[j + 1]) > 0) {
                char *temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }

    // Print the entries based on the `-1` option
    for (int i = 0; i < count; i++) {
        if (force_one_per_line) {
            printf("%s\n", entries[i]);
        } else {
            printf("%s  ", entries[i]);
        }
        free(entries[i]);  // Free each entry after printing
    }
    if (!force_one_per_line) {
        printf("\n");
    }

    // Free the entries array
    free(entries);
    return 0;
}

int main(int argc, char *argv[]) {
    int force_one_per_line = 0;
    char *path = ".";

    // Handle command-line arguments
    if (argc > 1) {
        if (custom_strcmp(argv[1], "-1") == 0) {
            force_one_per_line = 1;
            if (argc > 2) {
                path = argv[2];  // If path is provided after `-1`
            }
        } else {
            path = argv[1];  // If only path is provided
        }
    }

    // Check if the given path is a directory or a file
    struct stat path_stat;
    if (lstat(path, &path_stat) == -1) {
        perror("lstat");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        return list_directory(path, force_one_per_line);
    } else {
        perror("Error opening directory: Not a directory");
        return 1;
    }
}

