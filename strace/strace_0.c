#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    child = fork();
    if (child == -1) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        /* Child process: request to be traced and execute the command */
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace(PTRACE_TRACEME)");
            exit(1);
        }
        execv(argv[1], argv + 1);
        perror("execv");
        exit(1);
    } else {
        /* Parent process: trace the child process */
        while (1) {
            if (waitpid(child, &status, 0) == -1) {
                perror("waitpid");
                return 1;
            }
            if (WIFEXITED(status)) {
                break;
            }

            /* Retrieve the system call number */
            if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1) {
                perror("ptrace(PTRACE_GETREGS)");
                return 1;
            }

            printf("%lld\n", (long long)regs.orig_rax);

            /* Continue to the next system call */
            if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1) {
                perror("ptrace(PTRACE_SYSCALL)");
                return 1;
            }
        }
    }

    return 0;
}

