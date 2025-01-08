CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
TARGET = strace_0

all: $(TARGET)

$(TARGET): strace_0.c
	$(CC) $(CFLAGS) -o $(TARGET) strace_0.c

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean re

