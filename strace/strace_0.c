#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <errno.h>

void trace_syscalls(pid_t child_pid) {
    int status;
    struct user_regs_struct regs;

    // Wait for the child to stop
    waitpid(child_pid, &status, 0);

    while (1) {
        // Wait for syscall entry
        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
            perror("ptrace");
            break;
        }
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) break;

        // Get syscall number
        if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1) {
            perror("ptrace");
            break;
        }
#ifdef __x86_64__
        printf("Syscall: %lld\n", regs.orig_rax); // x86-64: syscall number in orig_rax
#elif __i386__
        printf("Syscall: %ld\n", regs.orig_eax);  // x86: syscall number in orig_eax
#else
#error "Unsupported architecture"
#endif

        // Wait for syscall exit
        if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1) {
            perror("ptrace");
            break;
        }
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) break;
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
        // Child process: trace me
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace");
            return EXIT_FAILURE;
        }
        // Replace the current process image with the given command
        execvp(argv[1], &argv[1]);
        perror("execvp");
        return EXIT_FAILURE;
    } else {
        // Parent process: trace the child
        trace_syscalls(child_pid);
    }

    return EXIT_SUCCESS;
}
