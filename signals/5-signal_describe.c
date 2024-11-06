#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/**
 * main - Prints description of a signal given by signum
 * @argc: Argument count
 * @argv: Argument vector
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char *argv[])
{
    int signum;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <signum>\n", argv[0]);
        return (EXIT_FAILURE);
    }
    signum = atoi(argv[1]);
    if (signum < 1 || signum >= NSIG)
    {
        fprintf(stderr, "Invalid signal number: %d\n", signum);
        return (EXIT_FAILURE);
    }
    printf("Signal %d: %s\n", signum, sys_siglist[signum]);
    return (EXIT_SUCCESS);
}

