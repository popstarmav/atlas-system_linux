#include <stddef.h>
#include <signal.h>

/**
 * current_handler_signal - Retrieves the current handler of SIGINT
 *
 * Return: Pointer to the current handler function, or NULL on failure
 */
void (*current_handler_signal(void))(int) {
    void (*current_handler)(int);

    // Retrieve the current handler for SIGINT without changing it
    current_handler = signal(SIGINT, SIG_IGN);

    if (current_handler == SIG_ERR) {
        return NULL; // Return NULL if retrieving the handler fails
    }

    // Restore the original handler
    signal(SIGINT, current_handler);

    return current_handler;
}

