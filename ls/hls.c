#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
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

int list_directory(const char *path, int force_one_per_line) {
    DIR *dir;
    struct dirent *entry;
    char **entries = NULL;
    int count = 0, capacity = 10;

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
        if (entry->d_name[0] != '.') {
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
        if (force_one_per_line) {
            printf("%s\n", entries[i]);
        } else {
            printf("%s  ", entries[i]);
        }
        free(entries[i]);
    }
    if (!force_one_per_line) {
        printf("\n");
    }

    free(entries);
    return 0;
}

int main(int argc, char *argv[]) {
    int force_one_per_line = 0;
    char *path = ".";

    if (argc > 1) {
        if (strcmp(argv[1], "-1") == 0) {
            force_one_per_line = 1;
            if (argc > 2) {
                path = argv[2];
            }
        } else {
            path = argv[1];
        }
    }

    return list_directory(path, force_one_per_line);
}
