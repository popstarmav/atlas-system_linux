CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = strace_0

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)
