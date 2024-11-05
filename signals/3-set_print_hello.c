#include <stdio.h>
#include <signal.h>

/* Prototype of the handler */
void print_hello(int signum);

/**
 * set_print_hello - Sets the handler for SIGINT to print_hello
 */
void set_print_hello(void) {
    struct sigaction sa;

    sa.sa_handler = print_hello; // Set the handler function
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask); // No signals to block during handler execution

    // Set the new action for SIGINT
    sigaction(SIGINT, &sa, NULL);
}

/**
 * print_hello - Prints a message when SIGINT is received
 * @signum: Signal number
 */
void print_hello(int signum) {
    (void)signum; // Explicitly ignore the unused parameter
    printf("Hello :)\n");
}

