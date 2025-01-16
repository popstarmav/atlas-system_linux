#include "syscalls.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/reg.h>

void child_process(char **argv, char **env) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    execve(argv[1], &argv[1], env);
}

void parent_process(pid_t child_pid) {
    int status;
    struct user_regs_struct regs;
    int in_syscall = 0;
    
    waitpid(child_pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

    while(1) {
        ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
        waitpid(child_pid, &status, 0);
        
        if (WIFEXITED(status))
            break;

        if (!in_syscall) {
            ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
            printf("%lld\n", (long long)regs.orig_rax);
        }
        in_syscall = !in_syscall;
    }
}

int main(int argc, char **argv, char **env) {
    if (argc < 2) {
        return 1;
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        child_process(argv, env);
    } else {
        parent_process(child_pid);
    }

    return 0;
}
