#include <stdio.h>
#include <signal.h>

/**
 * sigint_handler - Prints a message when SIGINT (Ctrl+C) is recived
 * @signnum: the signal number
 */
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}

/**
 * handle_signal - Sets sigint_handler to handle SIGINT (CTRL+C)
 *
 * Return: 0 on success, -1 on failure
 */
int handle_signal(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		return (-1);
	}

	return (0);
}

