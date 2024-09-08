#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/**
 * list_directory - Lists contents of a directory
 * @path: Directory path to list
 * @program_name: Name of the program (argv[0])
 */
void list_directory(const char *path, const char *program_name)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        fprintf(stderr, "%s: cannot access '%s': ", program_name, path);
        perror("");
        return;
    }

    printf("%s:\n", path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            printf("%s  ", entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}

/**
 * list_file - Prints a file's name
 * @path: File path to print
 */
void list_file(const char *path)
{
    printf("%s\n", path);
}

/**
 * handle_path - Handles files and directories
 * @path: Path to handle (file or directory)
 * @program_name: Name of the program (argv[0])
 */
void handle_path(const char *path, const char *program_name)
{
    struct stat path_stat;

    if (lstat(path, &path_stat) == -1)
    {
        fprintf(stderr, "%s: cannot access '%s': ", program_name, path);
        perror("");
        return;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        list_directory(path, program_name);
    }
    else
    {
        list_file(path);
    }
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
    if (argc < 2)
    {
        handle_path(".", argv[0]);  // Default to current directory
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            handle_path(argv[i], argv[0]);
        }
    }

    return EXIT_SUCCESS;
}
