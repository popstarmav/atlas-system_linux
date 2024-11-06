#include <stdlib.h>
#include <stdio.h>
#include <string.h>  // Correct header for strsignal
#include <signal.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <signum>\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int signum = atoi(argv[1]);
    if (signum < 1 || signum >= NSIG)
    {
        printf("Invalid signal number\n");
        return (EXIT_FAILURE);
    }

    // Ensure strsignal() is available by including <string.h>
    const char *signal_description = strsignal(signum);
    
    if (signal_description != NULL)
    {
        printf("Signal %d: %s\n", signum, signal_description);
    }
    else
    {
        printf("Signal %d: Unknown signal\n", signum);
    }

    return (EXIT_SUCCESS);
}

