#include <stdio.h>
#include <signal.h>

/**
 * print_hello - Signal handler that prints "Hello :)" when SIGINT is received
 * @signum: The signal number (should be SIGINT)
 */
void print_hello(int signum) {
    (void)signum; // Avoid unused parameter warning
    printf("Hello :)\n");
}

/**
 * set_print_hello - Sets print_hello as the handler for SIGINT
 */
void set_print_hello(void) {
    signal(SIGINT, print_hello);
}

