#include "syscalls.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>

int main(int argc, char *argv[])
{
    pid_t child;
    int status;
    struct user_regs_struct regs;
    int in_syscall = 0;

    if (argc < 2) {
        return 1;
    }

    child = fork();
    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(argv[1], argv + 1);
    } else {
        while (1) {
            wait(&status);
            if (WIFEXITED(status))
                break;

            if (!in_syscall) {
                ptrace(PTRACE_GETREGS, child, NULL, &regs);
                #ifdef __x86_64__
                printf("%llu\n", (unsigned long long)regs.orig_rax);
                #else
                printf("%llu\n", (unsigned long long)regs.orig_eax);
                #endif
            }
            in_syscall = !in_syscall;
            
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    }

    return 0;
}

