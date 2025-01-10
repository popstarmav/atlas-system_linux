#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <cstdlib>
#include <iomanip>

const char* syscalls[] = {
    "read", "write", "open", "close", "stat", "fstat", "lstat",
    "poll", "lseek", "mmap", "mprotect", "munmap", "brk", "rt_sigaction",
    "rt_sigprocmask", "rt_sigreturn", "ioctl", "pread64", "pwrite64",
    "readv", "writev", "access", "pipe", "select", "sched_yield",
    "mremap", "msync", "mincore", "madvise", "shmget", "shmat", "shmctl",
    "dup", "dup2", "pause", "nanosleep", "getitimer", "alarm", "setitimer",
    "getpid", "sendfile", "socket", "connect", "accept", "sendto", "recvfrom",
    "sendmsg", "recvmsg", "shutdown", "bind", "listen", "getsockname",
    "getpeername", "socketpair", "setsockopt", "getsockopt", "clone",
    "fork", "vfork", "execve", "exit", "wait4", "kill", "uname",
    "semget", "semop", "semctl", "shmdt", "msgget", "msgsnd", "msgrcv",
    "msgctl", "fcntl", "flock", "fsync", "fdatasync", "truncate",
    "ftruncate", "getdents", "getcwd", "chdir", "fchdir", "rename",
    "mkdir", "rmdir", "creat", "link", "unlink", "symlink", "readlink",
    "chmod", "fchmod", "chown", "fchown", "lchown", "umask", "gettimeofday",
    "getrlimit", "getrusage", "sysinfo", "times", "ptrace", "getuid",
    "syslog", "getgid", "setuid", "setgid", "geteuid", "getegid",
    "setpgid", "getppid", "getpgrp", "setsid", "setreuid", "setregid",
    "getgroups", "setgroups", "setresuid", "getresuid", "setresgid",
    "getresgid", "getpgid", "setfsuid", "setfsgid", "getsid", "capget",
    "capset", "rt_sigpending", "rt_sigtimedwait", "rt_sigqueueinfo",
    "rt_sigsuspend", "sigaltstack", "utime", "mknod", "uselib",
    "personality", "ustat", "statfs", "fstatfs", "sysfs", "getpriority",
    "setpriority", "sched_setparam", "sched_getparam", "sched_setscheduler",
    "sched_getscheduler", "sched_get_priority_max", "sched_get_priority_min",
    "sched_rr_get_interval", "mlock", "munlock", "mlockall", "munlockall",
    "vhangup", "modify_ldt", "pivot_root", "_sysctl", "prctl", "arch_prctl",
    "adjtimex", "setrlimit", "chroot", "sync", "acct", "settimeofday",
    "mount", "umount2", "swapon", "swapoff", "reboot", "sethostname",
    "setdomainname", "iopl", "ioperm", "create_module", "init_module",
    "delete_module", "get_kernel_syms", "query_module", "quotactl",
    "nfsservctl", "getpmsg", "putpmsg", "afs_syscall", "tuxcall",
    "security", "gettid", "readahead", "setxattr", "lsetxattr",
    "fsetxattr", "getxattr", "lgetxattr", "fgetxattr", "listxattr",
    "llistxattr", "flistxattr", "removexattr", "lremovexattr",
    "fremovexattr", "tkill", "time", "futex", "sched_setaffinity",
    "sched_getaffinity", "set_thread_area", "io_setup", "io_destroy",
    "io_getevents", "io_submit", "io_cancel", "get_thread_area",
    "lookup_dcookie", "epoll_create", "epoll_ctl_old", "epoll_wait_old",
    "remap_file_pages", "getdents64", "set_tid_address", "restart_syscall",
    "semtimedop", "fadvise64", "timer_create", "timer_settime",
    "timer_gettime", "timer_getoverrun", "timer_delete", "clock_settime",
    "clock_gettime", "clock_getres", "clock_nanosleep", "exit_group",
    "epoll_wait", "epoll_ctl", "tgkill", "utimes", "vserver", "mbind",
    "set_mempolicy", "get_mempolicy", "mq_open", "mq_unlink",
    "mq_timedsend", "mq_timedreceive", "mq_notify", "mq_getsetattr",
    "kexec_load", "waitid"
};

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
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        execv(argv[1], argv + 1);
        std::perror("execv");
        std::exit(1);
    } else if (child > 0) {
        waitpid(child, &status, 0);
        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        while (true) {
            ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
            if (waitpid(child, &status, 0) == -1) break;
            
            if (WIFEXITED(status)) break;
            
            ptrace(PTRACE_GETREGS, child, nullptr, &regs);
            unsigned long syscall_num = regs.orig_rax;

            ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
            if (waitpid(child, &status, 0) == -1) break;
            
            if (WIFEXITED(status)) {
                if (syscall_num < sizeof(syscalls)/sizeof(syscalls[0])) {
                    std::cout << syscalls[syscall_num] << " = ?" << std::endl;
                }
                break;
            }

            ptrace(PTRACE_GETREGS, child, nullptr, &regs);
            
            if (syscall_num < sizeof(syscalls)/sizeof(syscalls[0])) {
                std::cout << syscalls[syscall_num] << " = 0x" 
                         << std::hex << regs.rax << std::endl;
            }
        }
    } else {
        std::perror("fork");
        return 1;
    }

    return 0;
}

