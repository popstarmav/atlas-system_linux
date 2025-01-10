#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <cstdlib>

int main(int argc, char *argv[]) {
    pid_t child;
    int status;
    struct user_regs_struct regs;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " command [args...]" << std::endl;
        return 1;
    }

    child = fork();
    if (child == 0) {
        // Child process
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execvp(argv[1], argv + 1);
        std::perror("execvp");
        std::exit(1);
    } else if (child > 0) {
        // Parent process
        waitpid(child, &status, 0);
        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        while (true) {
            // Wait for syscall entry
            ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
            if (waitpid(child, &status, 0) == -1) break;
            
            // Check if process exited
            if (WIFEXITED(status)) break;
            
            // Get registers containing syscall number
            ptrace(PTRACE_GETREGS, child, nullptr, &regs);
            std::cout << regs.orig_rax << std::endl;

            // Wait for syscall exit
            ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
            if (waitpid(child, &status, 0) == -1) break;
            
            if (WIFEXITED(status)) break;
        }
    } else {
        std::perror("fork");
        return 1;
    }

    return 0;
}
