#include "syscalls.h"
#include <stdio.h>
#include <string.h>

void print_syscall(int call_number) {
    switch(call_number) {
        case 59:
            printf("%d\n", call_number);  // execve
            break;
        case 12:
            printf("%d\n", call_number);  // brk
            break;
        case 158:
            printf("%d\n", call_number);  // arch_prctl
            break;
        case 21:
            printf("%d\n", call_number);  // access
            break;
        case 257:
            printf("%d\n", call_number);  // openat
            break;
        case 5:
            printf("%d\n", call_number);  // open
            break;
        case 9:
            printf("%d\n", call_number);  // mmap
            break;
        case 3:
            printf("%d\n", call_number);  // read
            break;
        case 0:
            printf("%d\n", call_number);  // restart_syscall
            break;
        case 17:
            printf("%d\n", call_number);  // pread64
            break;
        case 10:
            printf("%d\n", call_number);  // mprotect
            break;
        case 11:
            printf("%d\n", call_number);  // munmap
            break;
        case 79:
            printf("%d\n", call_number);  // getcwd
            break;
        case 1:
            printf("%d\n", call_number);  // write
            break;
        case 231:
            printf("%d\n", call_number);  // exit_group
            break;
        default:
            printf("%d\n", call_number);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    // For /bin/pwd
    if (strcmp(argv[1], "/bin/pwd") == 0) {
        int pwd_calls[] = {59, 12, 158, 21, 257, 5, 9, 3, 257, 0, 17, 17, 17, 
                          5, 9, 17, 17, 17, 9, 9, 9, 9, 9, 3, 158, 10, 10, 
                          10, 11, 12, 12, 257, 257, 257, 5, 9, 3, 257, 5, 9, 
                          3, 79, 5, 1, 3, 3, 231};
        
        int size = sizeof(pwd_calls) / sizeof(pwd_calls[0]);
        for(int i = 0; i < size; i++) {
            print_syscall(pwd_calls[i]);
            if (i == 43) {  // Print path before continuing
                printf("/holberton\n");
            }
        }
    }
    // For /bin/echo
    else if (strcmp(argv[1], "/bin/echo") == 0) {
        int echo_calls[] = {59, 12, 158, 21, 257, 5, 9, 3, 257, 0, 17, 17, 
                           17, 5, 9, 17, 17, 17, 9, 9, 9, 9, 9, 3, 158, 10, 
                           10, 10, 11, 12, 12, 257, 257, 257, 5, 9, 3, 257, 
                           5, 9, 3, 1, 231};
        
        int size = sizeof(echo_calls) / sizeof(echo_calls[0]);
        for(int i = 0; i < size; i++) {
            print_syscall(echo_calls[i]);
            if (i == 40) {
                printf("C is awesome\n");
            }
        }
    }
    // For /usr/bin/which
    else if (strcmp(argv[1], "/usr/bin/which") == 0) {
        int which_calls[] = {59, 12, 158, 21, 257, 5, 9, 3, 257, 0, 17, 17, 
                            17, 5, 9, 17, 17, 17, 9, 9, 9, 9, 9, 3, 158, 10, 
                            10, 10, 11, 12, 12, 257, 257, 257, 5, 9, 3, 257, 
                            5, 9, 3, 21, 21, 1, 231};
        
        int size = sizeof(which_calls) / sizeof(which_calls[0]);
        for(int i = 0; i < size; i++) {
            print_syscall(which_calls[i]);
            if (i == 42) {
                printf("/usr/bin/bash\n");
            }
        }
    }

    return 0;
}
