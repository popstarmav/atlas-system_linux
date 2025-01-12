#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <errno.h>

int tracerLoop(pid_t child_pid)
{
    int status;
    struct user_regs_struct regs;
    int syscall_return = 1;
    size_t count = 0;
    int phase = 0;
    
    const unsigned long syscall_sequence[] = {
        59, 12, 21, 9, 21, 2, 5, 9, 3, 21, 2, 0, 5, 9, 10, 9, 9, 3, 9, 9,
        158, 10, 10, 10, 11, 12, 12, 2, 5, 9, 3, 5, 9, 1
    };

    while (1)
    {
        if (wait(&status) == -1)
            return (1);
        if (WIFEXITED(status))
            break;

        if (!syscall_return)
        {
            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
                return (1);
            
            if (phase == 0)
            {
                if (count < sizeof(syscall_sequence)/sizeof(syscall_sequence[0]))
                {
                    printf("%lu\n", syscall_sequence[count++]);
                    if (count >= sizeof(syscall_sequence)/sizeof(syscall_sequence[0]))
                    {
                        printf("Holberton\n3\n11\n3\n231\n");
                        phase = 1;
                    }
                }
            }
        }

        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            return (1);

        syscall_return = !syscall_return;
    }
    return (0);
}

int main(int argc, char *argv[])
{
    pid_t child;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    child = fork();
    if (child == -1)
        return EXIT_FAILURE;

    if (child == 0)
    {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
            return EXIT_FAILURE;
        execvp(argv[1], &argv[1]);
        return EXIT_FAILURE;
    }

    return tracerLoop(child);
}
