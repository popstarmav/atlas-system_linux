#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>

int main(int argc, char *argv[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;

    if (argc < 2) {
        printf("Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    child = fork();
    if (child == 0) {
        /* Child process */
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(argv[1], argv + 1);
        exit(1);
    }
    else {
        /* Parent process */
        while (1) {
            wait(&status);
            if (WIFEXITED(status))
                break;

            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            
            /* Print syscall number */
            printf("%lld\n", (long long)regs.orig_rax);

            /* Continue to next syscall */
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            wait(&status);
            if (WIFEXITED(status))
                break;
            
            /* Continue to next syscall entry */
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }

    return 0;
}

