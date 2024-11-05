#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int trace_signal_sender(void);

/**
 * sigquit_handler - Handles SIGQUIT signal
 * @signum: The signal number (SIGQUIT)
 */
void sigquit_handler(int signum)
{
    (void)signum;  // Explicitly mark 'signum' as unused
    pid_t sender_pid = getpid();
    printf("SIGQUIT sent by %d\n", sender_pid);
}

/**
 * trace_signal_sender - Sets the handler for SIGQUIT signal
 *
 * Return: 0 on success, -1 on error
 */
int trace_signal_sender(void)
{
    struct sigaction sa;

    sa.sa_handler = sigquit_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        return -1;  // Error setting the handler
    }

    return 0;  // Successfully set the handler
}

