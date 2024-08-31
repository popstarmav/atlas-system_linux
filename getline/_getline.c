#include "_getline.h"
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * _getline - Reads a line from a file descriptor.
 * @fd: The file descriptor to read from.
 *
 * Return: A null-terminated string containing the line read from the
 *         file descriptor, or NULL if there are no more lines or on error.
 */
char *_getline(const int fd)
{
    char buffer[READ_SIZE];
    char *line = NULL;
    size_t total_length = 0;
    ssize_t bytes_read;
    char *temp;

    if (fd < 0)
        return NULL;

    while ((bytes_read = read(fd, buffer, READ_SIZE)) > 0)
    {
        for (ssize_t i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == '\n')
            {
                // End of line found
                char *newline = malloc(total_length + i + 1);
                if (!newline)
                {
                    free(line);
                    return NULL;
                }
                if (line)
                {
                    memcpy(newline, line, total_length);
                    free(line);
                }
                memcpy(newline + total_length, buffer, i);
                newline[total_length + i] = '\0';
                return newline;
            }
        }

        // No newline found, extend line buffer
        temp = realloc(line, total_length + bytes_read);
        if (!temp)
        {
            free(line);
            return NULL;
        }
        line = temp;
        memcpy(line + total_length, buffer, bytes_read);
        total_length += bytes_read;
    }

    // Handle end of file or error
    if (total_length > 0)
    {
        char *newline = malloc(total_length + 1);
        if (!newline)
        {
            free(line);
            return NULL;
        }
        memcpy(newline, line, total_length);
        newline[total_length] = '\0';
        free(line);
        return newline;
    }
    free(line);
    return NULL;
}
