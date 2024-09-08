#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/**
 * list_directory - Lists contents of a directory or file
 * @path: Directory or file path to list
 * @program_name: Name of the program (argv[0])
 * Return: 0 on success, 1 on failure
 */
int list_directory(const char *path, const char *program_name)
{
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;

    // Use lstat to check if path is a file or directory
    if (lstat(path, &path_stat) == -1)
    {
        fprintf(stderr, "%s: cannot access '%s': ", program_name, path);
        perror("");
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) // Check if it's a directory
    {
        dir = opendir(path);
        if (dir == NULL)
        {
            fprintf(stderr, "%s: cannot open directory '%s': ", program_name, path);
            perror("");
            return 1;
        }

        printf("%s:\n", path);
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')  // Skip hidden files
            {
                printf("%s  ", entry->d_name);
            }
        }
        printf("\n\n");
        closedir(dir);
    }
    else  // If it's a file, print its name
    {
        printf("%s\n", path);
    }

    return 0;  // Success
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector (file/directory names)
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
    int exit_code = 0;

    if (argc < 2)
    {
        // Default to current directory if no arguments are provided
        if (list_directory(".", argv[0]) != 0)
        {
            exit_code = 1;
        }
    }
    else
    {
        // Process each argument
        for (int i = 1; i < argc; i++)
        {
            if (list_directory(argv[i], argv[0]) != 0)
            {
                exit_code = 1;
            }
        }
    }

    return exit_code;
}
