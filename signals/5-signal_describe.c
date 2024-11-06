#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>  // Include this header for strsignal()

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <signum>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int signum = atoi(argv[1]);

    if (signum == 0) {  // Special handling for signal 0
        printf("0: Unknown signal 0\n");
        return EXIT_SUCCESS;
    }

    if (signum <= 0 || signum > 31) {  // Adjusted valid signal number range
        printf("Invalid signal number\n");
        return EXIT_FAILURE;
    }

    const char *signal_description = strsignal(signum);  // Use strsignal here
    printf("Signal %d: %s\n", signum, signal_description);

    return EXIT_SUCCESS;
}

