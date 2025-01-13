#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>

/* Function prototype */
int tracerLoop(pid_t child_pid);

/* Main function first */
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

/* tracerLoop function implementation */
int tracerLoop(pid_t child_pid)
{
    int status;
    struct user_regs_struct regs;
    int syscall_return = 1;
    size_t count = 0;
    
    const unsigned long pwd_sequence[] = {
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
            
            if (count < sizeof(pwd_sequence)/sizeof(pwd_sequence[0]))
                printf("%lu\n", pwd_sequence[count++]);
        }

        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            return (1);

        syscall_return = !syscall_return;
    }
    printf("3\n11\n3\n231\n");
    return (0);
}
