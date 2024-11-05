#include <stdio.h>
#include <signal.h>

/**
 * current_handler_sigaction - Retrieves the current handler for SIGINT
 *
 * Return: Pointer to the current handler function, or NULL on failure
 */
void (*current_handler_sigaction(void))(int)
{
	struct sigaction sa;

	if (sigaction(SIGINT, NULL, &sa) == -1)
	{
		return (NULL);
	}
	return (sa.sa_handler);
}

