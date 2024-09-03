#include "_getline.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_SIZE 1024

/**
 * _getline - Reads a line from a file descriptor.
 * @fd: The file descriptor to read from.
 *
 * Return: A null-terminated string containing the line read from the
 *         file descriptor, or NULL if there are no more lines or on error.
 */
char *_getline(const int fd)
{
    static char buffer[READ_SIZE];
    static ssize_t buffer_size = 0;
    static ssize_t buffer_pos = 0;
    char *line = NULL;
    size_t line_length = 0;
    ssize_t i;

    if (fd < 0)
        return NULL;

    while (1) {
        if (buffer_pos >= buffer_size) {
            buffer_size = read(fd, buffer, READ_SIZE);
            buffer_pos = 0;

            if (buffer_size <= 0) {
                if (line) {
                    // If there is any collected line part, return it.
                    return line;
                }
                // No more data or read error.
                return NULL;
            }
        }

        for (i = buffer_pos; i < buffer_size; i++) {
            if (buffer[i] == '\n') {
                line = realloc(line, line_length + (i - buffer_pos) + 1);
                if (!line)
                    return NULL;

                memcpy(line + line_length, buffer + buffer_pos, i - buffer_pos);
                line[line_length + (i - buffer_pos)] = '\0';
                buffer_pos = i + 1;  // Move past the newline character

                return line;
            }
        }

        // No newline found, collect the entire buffer content.
        line = realloc(line, line_length + (buffer_size - buffer_pos));
        if (!line)
            return NULL;

        memcpy(line + line_length, buffer + buffer_pos, buffer_size - buffer_pos);
        line_length += (buffer_size - buffer_pos);
        buffer_pos = buffer_size;
    }
}
