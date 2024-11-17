#include <signal.h>
#include <unistd.h>

/**
 * pid_exist - Checks if a process with the given PID exists.
 * @pid: The process ID to check.
 *
 * Return: 1 if the process exists, 0 otherwise.
 */
int pid_exist(pid_t pid)
{
	return (kill(pid, 0) == 0);
}
