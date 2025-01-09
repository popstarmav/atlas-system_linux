#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

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
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(argv[1], argv + 1);
        exit(1);
    }

    wait(&status);
    while (1) {
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        wait(&status);
        
        if (WIFEXITED(status))
            break;
            
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        printf("%lld\n", (long long)regs.orig_rax);
        
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        wait(&status);
        
        if (WIFEXITED(status))
            break;
    }
    return 0;
}

