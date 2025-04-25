# C - Signals

This repository contains examples and information about signal handling in C programming.

## What are Signals?

Signals are software interrupts sent to a program to indicate that an important event has occurred. The events can vary from user requests to illegal memory access errors. Some signals, such as SIGKILL, cannot be caught or ignored.

## Common Signals

- `SIGINT` (2): Interrupt signal (Ctrl+C)
- `SIGTERM` (15): Termination signal
- `SIGKILL` (9): Kill signal (cannot be caught or ignored)
- `SIGSEGV` (11): Segmentation violation
- `SIGALRM` (14): Alarm clock signal
- `SIGUSR1` (10): User-defined signal 1
- `SIGUSR2` (12): User-defined signal 2

## Signal Handling in C

In C, signals can be handled using the `signal()` function or the more advanced `sigaction()` function.

Basic syntax:
```c
#include <signal.h>

void signal_handler(int signum) {
    // Handle the signal
}

int main() {
    signal(SIGINT, signal_handler);
    // Rest of the program
}
```

## Examples

This repository includes examples demonstrating:
- Basic signal handling
- Blocking signals
- Using sigaction()
- Custom signal handlers
- Signal masks

## Resources

- [Linux Signal Manual](https://man7.org/linux/man-pages/man7/signal.7.html)
- [Signal Handling in C](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)
