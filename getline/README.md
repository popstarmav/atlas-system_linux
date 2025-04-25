# C Programming - Static Variables and getline

## Static Variables
Static variables in C have a property of preserving their value even after they are out of their scope. This means that static variables preserve their previous value in their previous scope and are not initialized again in the new scope.

## getline Function
The `getline` function in C is used to read an entire line from a stream, storing the address of the buffer containing the text into a pointer. It's part of the POSIX standard and is available in GNU C Library.

### Syntax
```c
ssize_t getline(char **lineptr, size_t *n, FILE *stream);


Parameters
lineptr: A pointer to the variable that will point to the buffer containing the read line.
n: A pointer to the variable that holds the size of the buffer.
stream: The input stream to read from.
Return Value
Returns the number of characters read, including the newline character but excluding the terminating null byte.
Returns -1 on failure to read a line (including EOF) or if an error occurs.
Example Usage
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("Enter a line: ");
    read = getline(&line, &len, stdin);
    
    if (read != -1) {
        printf("Retrieved line of length %zu: %s", read, line);
    }
    
    free(line);
    return 0;
}



Notes
The buffer is dynamically allocated and should be freed by the caller when no longer needed.
If *lineptr is NULL and *n is 0 before the call, then getline will allocate a buffer for storing the line.
The buffer is null-terminated and includes the newline character if one was found. EOF
