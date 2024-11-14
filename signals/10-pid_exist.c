#include <signal.h>
#include <unistd.h>

int pid_exist(pid_t pid)
{
	return (kill(pid, 0) == 0);
}
