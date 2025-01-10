#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    pid_t child;                   // Child process ID
    int status;                    // Status variable for wait()
    struct user_regs_struct regs;  // Structure to store registers

    // Check for valid input
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " command [args...]" << std::endl;
        return 1;
    }

    // Fork the process
    child = fork();
    if (child == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr); // Enable tracing
        execv(argv[1], argv + 1);                   // Execute the command
        perror("execv");                            // If execv fails
        exit(1);
    } else if (child > 0) {
        // Parent process
        while (true) {
            // Wait for child state change
            wait(&status);
            if (WIFEXITED(status)) break; // Exit loop if the child process exits

            // Fetch the system call number
            ptrace(PTRACE_GETREGS, child, nullptr, &regs);
            std::cout << regs.orig_rax << std::endl;

            // Move to the next syscall
            ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
        }
    } else {
        // Fork failed
        perror("fork");
        return 1;
    }

    return 0;
}

