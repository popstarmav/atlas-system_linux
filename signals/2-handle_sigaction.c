#include <stdio.h>
#include <signal.h>

/**
 * sigint_handler - Prints a message when SIGINT (Ctrl+C) is received
 * @signum: The signal number (should be SIGINT)
 */
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}

/**
 * handle_sigaction - Sets sigint_handler as
 * the handler for SIGINT using sigaction
 *
 * Return: 0 on success, -1 on error
 */
int handle_sigaction(void)
{
	struct sigaction sa;

	/*Initialize sigaction struct*/
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		return (-1);
	}
	return (0);
}

