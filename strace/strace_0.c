#include "syscalls.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>

int tracerLoop(pid_t child_pid)
{
    int status;
    struct user_regs_struct regs;
    int first_syscall = 1;
    int syscall_return = 1;

    while (1)
    {
        if (wait(&status) == -1)
            return (1);
        if (WIFEXITED(status))
            break;

        if (!syscall_return || first_syscall)
        {
            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
                return (1);
            if (first_syscall)
            {
                printf("59\n");
                first_syscall = 0;
            }
            else
                printf("%lu\n", (unsigned long)regs.orig_rax);
        }

        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            return (1);

        syscall_return = !syscall_return;
    }
    return (0);
}

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;

    if (argc < 2 || !argv)
    {
        fprintf(stderr, "usage: %s <prog> <prog args>...\n", argv[0]);
        return (1);
    }

    switch (pid = fork())
    {
        case -1:
            return (1);
        case 0:
            if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
                return (1);
            if (execve(argv[1], argv + 1, envp) == -1)
                return (1);
        default:
            break;
    }

    return (tracerLoop(pid));
}
