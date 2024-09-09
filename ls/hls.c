#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/**
 * string_equals - Compares two strings for equality
 * @str1: First string
 * @str2: Second string
 *
 * Return: 1 if strings are equal, 0 otherwise
 */
int string_equals(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');
}

/**
 * compare_strings - Comparison function for qsort
 * @a: First string
 * @b: Second string
 *
 * Return: Negative if a < b, 0 if a == b, positive if a > b
 */
int compare_strings(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

/**
 * list_directory - Lists contents of a directory or file
 * @path: Directory or file path to list
 * @program_name: Name of the program (argv[0])
 * @print_dirname: Whether to print the directory name
 * @one_per_line: Whether to print one file per line
 *
 * Return: 0 on success, 1 on failure
 */
int list_directory(const char *path, const char *program_name, int print_dirname, int one_per_line)
{
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;
    char **file_list = NULL;
    int file_count = 0;

    if (lstat(path, &path_stat) == -1)
    {
        fprintf(stderr, "%s: cannot access %s: ", program_name, path);
        perror("");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        dir = opendir(path);
        if (dir == NULL)
        {
            fprintf(stderr, "%s: cannot open directory %s: ", program_name, path);
            perror("");
            return 1;
        }

        // Print the directory name (if required)
        if (print_dirname)
        {
            printf("%s:\n", path);
        }

        // Allocate memory for file list
        file_list = malloc(sizeof(char *) * 1024);  // Assume a maximum of 1024 entries
        if (!file_list)
        {
            fprintf(stderr, "Memory allocation error\n");
            closedir(dir);
            return 1;
        }

        // Read the directory contents and store them in an array
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')  // Skip hidden files
            {
                file_list[file_count] = strdup(entry->d_name);  // Duplicate the string
                if (!file_list[file_count])
                {
                    fprintf(stderr, "Memory allocation error\n");
                    closedir(dir);
                    return 1;
                }
                file_count++;
            }
        }
        closedir(dir);

        // Sort the array alphabetically
        qsort(file_list, file_count, sizeof(char *), compare_strings);

        // Print the sorted contents
        for (int i = 0; i < file_count; i++)
        {
            if (one_per_line)
            {
                printf("%s\n", file_list[i]);
            }
            else
            {
                printf("%s  ", file_list[i]);
            }
            free(file_list[i]);  // Free the allocated memory for each string
        }
        printf("\n");
        free(file_list);  // Free the array
    }
    else if (S_ISREG(path_stat.st_mode))  // Handle regular files
    {
        printf("%s\n", path);
    }
    else
    {
        fprintf(stderr, "%s: cannot access %s: Not a directory\n", program_name, path);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int exit_code = 0;
    int one_per_line = 0;

    // First, check if the -1 option is provided anywhere in the arguments
    for (int i = 1; i < argc; i++)
    {
        if (string_equals(argv[i], "-1"))
        {
            one_per_line = 1;
            break;
        }
    }

    // If no file or directory is provided, default to current directory
    if (argc == 1 || (argc == 2 && one_per_line))
    {
        if (list_directory(".", argv[0], 0, one_per_line) != 0)
        {
            exit_code = 1;
        }
    }
    else
    {
        // Handle each argument (directory or file)
        for (int i = 1; i < argc; i++)
        {
            // Skip the -1 option if encountered
            if (string_equals(argv[i], "-1"))
            {
                continue;
            }

            int print_dirname = (argc > 2) ? 1 : 0;
            if (list_directory(argv[i], argv[0], print_dirname, one_per_line) != 0)
            {
                exit_code = 1;
            }

            // Print a blank line between multiple directories
            if (i < argc - 1)
            {
                printf("\n");
            }
        }
    }

    return exit_code;
}
