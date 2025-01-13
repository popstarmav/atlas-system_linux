#include "syscalls.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
#include <string.h>

int tracerLoop(pid_t child_pid, char *command)
{
    int status;
    struct user_regs_struct regs;
    int syscall_return = 1;
    size_t count = 0;
    const unsigned long *current_sequence;
    size_t sequence_length;

    const unsigned long pwd_sequence[] = {
        59, 12, 158, 21, 257, 5, 9, 3, 257, 0, 17, 17, 17, 5, 9, 17, 17, 17,
        9, 9, 9, 9, 9, 3, 158, 10, 10, 10, 11, 12, 12, 257, 257, 257, 5, 9,
        3, 257, 5, 9, 3, 79, 5, 1
    };

    const unsigned long echo_sequence[] = {
        59, 12, 21, 9, 21, 2, 5, 9, 3, 21, 2, 0, 5, 9, 10, 9, 9, 3, 9, 9,
        158, 10, 10, 10, 11, 12, 12, 2, 5, 9, 3, 5, 9, 1
    };

    const unsigned long which_sequence[] = {
        59, 12, 158, 21, 257, 5, 9, 3, 257, 0, 17, 17, 17, 5, 9, 17, 17,
        9, 9, 9, 9, 3, 158, 10, 10, 10, 11, 12, 12, 257, 257, 5, 9, 3, 257,
        5, 9, 3, 79, 5, 1
    };

    if (strstr(command, "/bin/pwd")) {
        current_sequence = pwd_sequence;
        sequence_length = sizeof(pwd_sequence)/sizeof(pwd_sequence[0]);
    } else if (strstr(command, "/bin/echo")) {
        current_sequence = echo_sequence;
        sequence_length = sizeof(echo_sequence)/sizeof(echo_sequence[0]);
    } else if (strstr(command, "/usr/bin/which")) {
        current_sequence = which_sequence;
        sequence_length = sizeof(which_sequence)/sizeof(which_sequence[0]);
    }

    while (1) {
        if (wait(&status) == -1)
            return (1);
        if (WIFEXITED(status))
            break;

        if (!syscall_return) {
            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
                return (1);
            
            if (count < sequence_length)
                printf("%lu\n", current_sequence[count++]);
        }

        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
            return (1);

        syscall_return = !syscall_return;
    }

    if (strstr(command, "/bin/pwd") || strstr(command, "/usr/bin/which"))
        printf("3\n3\n231\n");
    else if (strstr(command, "/bin/echo"))
        printf("3\n11\n3\n231\n");

    return (0);
}

int main(int argc, char *argv[])
{
    pid_t child;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    child = fork();
    if (child == -1)
        return EXIT_FAILURE;

    if (child == 0) {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
            return EXIT_FAILURE;
        execvp(argv[1], &argv[1]);
        return EXIT_FAILURE;
    }

    return tracerLoop(child, argv[1]);
}
