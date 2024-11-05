#ifndef SIGNALS_H
#define SIGNALS_H

int handle_signal(void);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);

#endif /* SIGNALS_H */

