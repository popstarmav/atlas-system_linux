#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/**
 * main - Sends SIGINT to a specified process by PID
 * @argc: Argument count
 * @argv: Argument vector
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return (EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]);

    if (kill(pid, SIGINT) == -1)
    {
        perror("Error sending signal");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

