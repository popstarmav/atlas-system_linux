#ifndef SIGNALS_H
#define SIGNALS_H

int handle_signal(void);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
int trace_signal_sender(void);
void (*current_handler_sigaction(void))(int);

#endif /* SIGNALS_H */

