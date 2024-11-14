#include <stdio.h>
#include <signal.h>

#include <unistd.h>  // for pid_t and kill

int pid_exist(pid_t pid)
{
	return (kill(pid, 0) == 0);
}
