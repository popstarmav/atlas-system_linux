#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

void trace_syscalls(pid_t child_pid) {
    int status;
    struct user_regs_struct regs;

    waitpid(child_pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);

    while (1) {
        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
            perror("ptrace");
            break;
        }
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("waitpid");
            break;
        }
        if (WIFEXITED(status))
            break;

        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1) {
                perror("ptrace");
                break;
            }
#ifdef __x86_64__
            printf("%lld\n", (long long)regs.orig_rax);
#else
            printf("%ld\n", (long)regs.orig_eax);
#endif
            if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
                perror("ptrace");
                break;
            }
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("waitpid");
                break;
            }
            if (WIFEXITED(status))
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return EXIT_FAILURE;
    }

    trace_syscalls(child_pid);
    return EXIT_SUCCESS;
}
