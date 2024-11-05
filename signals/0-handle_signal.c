#include <stdio.h>
#include <signal.h>

/**
 * sigint_handler - Handles the SIGINT signal (e.g., when Control-C is pressed)
 * @signum: The signal number received (should be SIGINT)
 *
 * Prints a message indicating that the SIGINT signal was caught.
 */
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}


/**
 * handle_signal - Sets up the SIGINT signal handler
 *
 * Return: 0 on success, or -1 if an error occurred
 *
 * Sets sigint_handler as the handler for SIGINT.
 * If setting the handler fails,
 * it returns -1; otherwise, it returns 0.
 */i
int handle_signal(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		return (-1);
	}

	return (0);
}

