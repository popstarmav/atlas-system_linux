#include <stdio.h>
#include <signal.h>

/**
 * current_handler_sigaction - Retrieves the current handler for SIGINT
 *
 * Return: Pointer to the current handler function, or NULL on failure
 */
void (*current_handler_sigaction(void))(int) {
    struct sigaction sa;

    // Get the current action for SIGINT
    if (sigaction(SIGINT, NULL, &sa) == -1) {
        return NULL; // Return NULL if sigaction fails
    }
    return sa.sa_handler; // Return the current handler
}

