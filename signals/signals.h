#ifndef SIGNALS_H
#define SIGNALS_H

int handle_signal(void);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
int trace_signal_sender(void);
int pid_exist(pid_t pid);
void (*current_handler_sigaction(void))(int);

#endif /* SIGNALS_H */

