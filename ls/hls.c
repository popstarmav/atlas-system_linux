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
 *
 * Return: 0 on success, 1 on failure
 */
int list_directory(const char *path, const char *program_name, int print_dirname, int one_per_line)
{
    struct dirent *entry;
    DIR *dir;
    struct stat path_stat;

    if (lstat(path, &path_stat) == -1)
    {
        fprintf(stderr, "%s: cannot access %s: ", program_name, path);
        perror("");  // Print "No such file or directory" error
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        dir = opendir(path);
        if (dir == NULL)
        {
            fprintf(stderr, "%s: cannot open directory %s: ", program_name, path);
            perror("");  // Handle "Permission denied" or other errors
            return 1;
        }

        // Print the directory name (if required)
        if (print_dirname)
        {
            printf("%s:\n", path);
        }

        // Read the directory contents
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')  // Skip hidden files
            {
                if (one_per_line)
                {
                    printf("%s\n", entry->d_name);
                }
                else
                {
                    printf("%s  ", entry->d_name);
                }
            }
        }
        printf("\n");
        closedir(dir);
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
    int start_index = 1;

    // Check for -1 option
    if (argc > 1 && strcmp(argv[1], "-1") == 0)
    {
        one_per_line = 1;
        start_index = 2;  // Skip the -1 option
    }

    // If no file or directory is provided, default to current directory
    if (argc < start_index + 1)
    {
        if (list_directory(".", argv[0], 0, one_per_line) != 0)
        {
            exit_code = 1;
        }
    }
    else
    {
        // Handle each argument (directory or file)
        for (int i = start_index; i < argc; i++)
        {
            // Check if the argument is "-1" (should not be processed as a file or directory)
            if (strcmp(argv[i], "-1") == 0)
            {
                continue;  // Skip the -1 option
            }

            int print_dirname = (argc > start_index + 1) ? 1 : 0;
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
