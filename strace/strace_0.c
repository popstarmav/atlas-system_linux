#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>

/**
 * Program to trace system calls made by a given command.
 * Usage: ./strace_0 command [args...]
 */
int main(int argc, char *argv[])
{
    pid_t child;                   // Process ID of the child
    int status;                    // Status for wait()
    struct user_regs_struct regs;  // Structure to store registers

    // Ensure the program is called with at least one argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    child = fork(); // Fork the process
    if (child == 0) {
        // Child process: Enable tracing and execute the given command
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execv(argv[1], argv + 1);
        perror("execv"); // If execv fails, print an error
        exit(1);
    } else if (child > 0) {
        // Parent process: Trace the child process
        while (1) {
            // Wait for child state change
            wait(&status);

            // Break if the child process exits
            if (WIFEXITED(status)) {
                break;
            }

            // Get the syscall number (orig_rax) from the child process
            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            printf("%lld\n", (long long)regs.orig_rax);

            // Continue to the next syscall entry
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    } else {
        // Fork failed
        perror("fork");
        return 1;
    }

    return 0;
}
