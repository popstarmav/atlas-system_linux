#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <signum>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	
	int signum = atoi(argv[1]);
	
	if (signum == 0)
	{
		printf("0: Unknown signal 0\n");
		return (EXIT_SUCCESS);
	}
	
	if (signum <= 0 || signum > 100)
	{
		printf("Invalid signal number\n");
		return (EXIT_FAILURE);
	}
	
	const char *signal_description = strsignal(signum);
	printf("%d: %s\n", signum, signal_description);
	
	return (EXIT_SUCCESS);
}

