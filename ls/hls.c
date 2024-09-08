#include "hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/**
 * list_directory - Lists contents of a directory or file
 * @path: Directory or file path to list
 * @program_name: Name of the program (argv[0])
 */
void list_directory(const char *path, const char *program_name)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        // Error handling for files and directories
        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            // Print error to stderr
            fprintf(stderr, "%s: cannot access '%s': %s\n", program_name, path, strerror(errno));
            exit(EXIT_FAILURE);  // Non-zero exit on error
        }
        else
        {
            printf("%s\n", path);  // If it's a file, print the filename
            fclose(fp);
        }
        return;
    }

    printf("%s:\n", path);  // Print directory name
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
        list_directory(".", argv[0]);  // Default to current directory if no arguments
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            list_directory(argv[i], argv[0]);
        }
    }

    return EXIT_SUCCESS;
}
